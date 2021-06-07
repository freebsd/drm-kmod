/* SPDX-License-Identifier: GPL-2.0 OR MIT */
/**************************************************************************
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#define pr_fmt(fmt) "[TTM] " fmt

#include <drm/ttm/ttm_module.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/drm_vma_manager.h>
#include <linux/mm.h>
#include <linux/pfn_t.h>
#include <linux/rbtree.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/mem_encrypt.h>

static vm_fault_t ttm_bo_vm_fault_idle(struct ttm_buffer_object *bo,
				struct vm_fault *vmf)
{
	vm_fault_t ret = 0;
	int err = 0;

	if (likely(!bo->moving))
		goto out_unlock;

	/*
	 * Quick non-stalling check for idle.
	 */
	if (dma_fence_is_signaled(bo->moving))
		goto out_clear;

	/*
	 * If possible, avoid waiting for GPU with mmap_sem
	 * held.
	 */
	if (vmf->flags & FAULT_FLAG_ALLOW_RETRY) {
		ret = VM_FAULT_RETRY;
		if (vmf->flags & FAULT_FLAG_RETRY_NOWAIT)
			goto out_unlock;

		ttm_bo_get(bo);
		up_read(&vmf->vma->vm_mm->mmap_sem);
		(void) dma_fence_wait(bo->moving, true);
		dma_resv_unlock(bo->base.resv);
		ttm_bo_put(bo);
		goto out_unlock;
	}

	/*
	 * Ordinary wait.
	 */
	err = dma_fence_wait(bo->moving, true);
	if (unlikely(err != 0)) {
		ret = (err != -ERESTARTSYS) ? VM_FAULT_SIGBUS :
			VM_FAULT_NOPAGE;
		goto out_unlock;
	}

out_clear:
	dma_fence_put(bo->moving);
	bo->moving = NULL;

out_unlock:
	return ret;
}

static unsigned long ttm_bo_io_mem_pfn(struct ttm_buffer_object *bo,
				       unsigned long page_offset)
{
	struct ttm_bo_device *bdev = bo->bdev;

	if (bdev->driver->io_mem_pfn)
		return bdev->driver->io_mem_pfn(bo, page_offset);

	return ((bo->mem.bus.base + bo->mem.bus.offset) >> PAGE_SHIFT)
		+ page_offset;
}

/**
 * ttm_bo_vm_reserve - Reserve a buffer object in a retryable vm callback
 * @bo: The buffer object
 * @vmf: The fault structure handed to the callback
 *
 * vm callbacks like fault() and *_mkwrite() allow for the mm_sem to be dropped
 * during long waits, and after the wait the callback will be restarted. This
 * is to allow other threads using the same virtual memory space concurrent
 * access to map(), unmap() completely unrelated buffer objects. TTM buffer
 * object reservations sometimes wait for GPU and should therefore be
 * considered long waits. This function reserves the buffer object interruptibly
 * taking this into account. Starvation is avoided by the vm system not
 * allowing too many repeated restarts.
 * This function is intended to be used in customized fault() and _mkwrite()
 * handlers.
 *
 * Return:
 *    0 on success and the bo was reserved.
 *    VM_FAULT_RETRY if blocking wait.
 *    VM_FAULT_NOPAGE if blocking wait and retrying was not allowed.
 */
vm_fault_t ttm_bo_vm_reserve(struct ttm_buffer_object *bo,
			     struct vm_fault *vmf)
{
	if (unlikely(!dma_resv_trylock(bo->base.resv))) {
		if (vmf->flags & FAULT_FLAG_ALLOW_RETRY) {
			if (!(vmf->flags & FAULT_FLAG_RETRY_NOWAIT)) {
				ttm_bo_get(bo);
				up_read(&vmf->vma->vm_mm->mmap_sem);
				if (!dma_resv_lock_interruptible(bo->base.resv,
								 NULL))
					dma_resv_unlock(bo->base.resv);
				ttm_bo_put(bo);
			}

			return VM_FAULT_RETRY;
		}

		if (dma_resv_lock_interruptible(bo->base.resv, NULL))
			return VM_FAULT_NOPAGE;
	}

	return 0;
}
EXPORT_SYMBOL(ttm_bo_vm_reserve);

/**
 * ttm_bo_vm_fault_reserved - TTM fault helper
 * @vmf: The struct vm_fault given as argument to the fault callback
 * @prot: The page protection to be used for this memory area.
 * @num_prefault: Maximum number of prefault pages. The caller may want to
 * specify this based on madvice settings and the size of the GPU object
 * backed by the memory.
 *
 * This function inserts one or more page table entries pointing to the
 * memory backing the buffer object, and then returns a return code
 * instructing the caller to retry the page access.
 *
 * Return:
 *   VM_FAULT_NOPAGE on success or pending signal
 *   VM_FAULT_SIGBUS on unspecified error
 *   VM_FAULT_OOM on out-of-memory
 *   VM_FAULT_RETRY if retryable wait
 */
vm_fault_t ttm_bo_vm_fault_reserved(struct vm_fault *vmf,
				    pgprot_t prot,
				    pgoff_t num_prefault)
{
	struct vm_area_struct *vma = vmf->vma;
	struct vm_area_struct cvma = *vma;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	struct ttm_bo_device *bdev = bo->bdev;
	unsigned long page_offset;
	unsigned long page_last;
	unsigned long pfn;
	struct ttm_tt *ttm = NULL;
	struct page *page;
	int err;
	pgoff_t i;
	vm_fault_t ret = VM_FAULT_NOPAGE;
	unsigned long address = vmf->address;
	struct ttm_mem_type_manager *man =
		&bdev->man[bo->mem.mem_type];

	/*
	 * Refuse to fault imported pages. This should be handled
	 * (if at all) by redirecting mmap to the exporter.
	 */
	if (bo->ttm && (bo->ttm->page_flags & TTM_PAGE_FLAG_SG))
		return VM_FAULT_SIGBUS;

	if (bdev->driver->fault_reserve_notify) {
		struct dma_fence *moving = dma_fence_get(bo->moving);

		err = bdev->driver->fault_reserve_notify(bo);
		switch (err) {
		case 0:
			break;
		case -EBUSY:
		case -ERESTARTSYS:
			return VM_FAULT_NOPAGE;
		default:
			return VM_FAULT_SIGBUS;
		}

		if (bo->moving != moving) {
			spin_lock(&ttm_bo_glob.lru_lock);
			ttm_bo_move_to_lru_tail(bo, NULL);
			spin_unlock(&ttm_bo_glob.lru_lock);
		}
		dma_fence_put(moving);
	}

	/*
	 * Wait for buffer data in transit, due to a pipelined
	 * move.
	 */
	ret = ttm_bo_vm_fault_idle(bo, vmf);
	if (unlikely(ret != 0))
		return ret;

	err = ttm_mem_io_lock(man, true);
	if (unlikely(err != 0))
		return VM_FAULT_NOPAGE;
	err = ttm_mem_io_reserve_vm(bo);
	if (unlikely(err != 0)) {
		ret = VM_FAULT_SIGBUS;
		goto out_io_unlock;
	}

	page_offset = ((address - vma->vm_start) >> PAGE_SHIFT) +
		vma->vm_pgoff - drm_vma_node_start(&bo->base.vma_node);
	page_last = vma_pages(vma) + vma->vm_pgoff -
		drm_vma_node_start(&bo->base.vma_node);

	if (unlikely(page_offset >= bo->num_pages)) {
		ret = VM_FAULT_SIGBUS;
		goto out_io_unlock;
	}

	cvma.vm_page_prot = ttm_io_prot(bo->mem.placement, prot);
	if (!bo->mem.bus.is_iomem) {
		struct ttm_operation_ctx ctx = {
			.interruptible = false,
			.no_wait_gpu = false,
			.flags = TTM_OPT_FLAG_FORCE_ALLOC

		};

		ttm = bo->ttm;
		if (ttm_tt_populate(bo->ttm, &ctx)) {
			ret = VM_FAULT_OOM;
			goto out_io_unlock;
		}
	} else {
		/* Iomem should not be marked encrypted */
#ifdef __linux__
		cvma.vm_page_prot = pgprot_decrypted(cvma.vm_page_prot);
#endif
	}

	/*
	 * Speculatively prefault a number of pages. Only error on
	 * first page.
	 */
#ifdef __linux__
	for (i = 0; i < num_prefault; ++i) {
		if (bo->mem.bus.is_iomem) {
			pfn = ttm_bo_io_mem_pfn(bo, page_offset);
		} else {
			page = ttm->pages[page_offset];
			if (unlikely(!page && i == 0)) {
				ret = VM_FAULT_OOM;
				goto out_io_unlock;
			} else if (unlikely(!page)) {
				break;
			}
			page->index = drm_vma_node_start(&bo->base.vma_node) +
				page_offset;
			pfn = page_to_pfn(page);
		}

		if (vma->vm_flags & VM_MIXEDMAP)
			ret = vmf_insert_mixed(&cvma, address,
					__pfn_to_pfn_t(pfn, PFN_DEV));
		else
			ret = vmf_insert_pfn(&cvma, address, pfn);

		/* Never error on prefaulted PTEs */
		if (unlikely((ret & VM_FAULT_ERROR))) {
			if (i == 0)
				goto out_io_unlock;
			else
				break;
		}

		address += PAGE_SIZE;
		if (unlikely(++page_offset >= page_last))
			break;
	}
#elif defined(__FreeBSD__)
	vm_object_t obj;
	vm_pindex_t pidx;

	ret = VM_FAULT_NOPAGE;
	obj = vma->vm_obj;
	pidx = OFF_TO_IDX(address);
	vma->vm_pfn_first = pidx;

	VM_OBJECT_WLOCK(obj);
	for (i = 0; i < TTM_BO_VM_NUM_PREFAULT && page_offset < page_last;
	    i++, page_offset++, pidx++) {
retry:
		page = vm_page_grab(obj, pidx, VM_ALLOC_NOCREAT);
		if (page == NULL) {
			if (bo->mem.bus.is_iomem) {
				pfn = ttm_bo_io_mem_pfn(bo, page_offset);
				page = PHYS_TO_VM_PAGE(IDX_TO_OFF(pfn));
			} else {
				page = ttm->pages[page_offset];
				if (page == NULL)
					goto fail;
				page->oflags &= ~VPO_UNMANAGED;
			}
			if (!vm_page_busy_acquire(page, VM_ALLOC_WAITFAIL))
				goto retry;
			if (vm_page_insert(page, obj, pidx)) {
				vm_page_xunbusy(page);
				goto fail;
			}
			vm_page_valid(page);
		}
		pmap_page_set_memattr(page,
		    pgprot2cachemode(cvma.vm_page_prot));
		vma->vm_pfn_count++;
		continue;
fail:
		if (i == 0)
			ret = VM_FAULT_OOM;
		break;
	}
	VM_OBJECT_WUNLOCK(obj);
#endif
out_io_unlock:
	ttm_mem_io_unlock(man);
	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_fault_reserved);

#ifdef __linux__
static vm_fault_t ttm_bo_vm_fault(struct vm_fault *vmf)
#elif defined (__FreeBSD__)
static vm_fault_t ttm_bo_vm_fault(struct vm_area_struct *dummy, struct vm_fault *vmf)
#endif
{
	struct vm_area_struct *vma = vmf->vma;
	pgprot_t prot;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	vm_fault_t ret;

	ret = ttm_bo_vm_reserve(bo, vmf);
	if (ret)
		return ret;

	prot = vm_get_page_prot(vma->vm_flags);
	ret = ttm_bo_vm_fault_reserved(vmf, prot, TTM_BO_VM_NUM_PREFAULT);
	if (ret == VM_FAULT_RETRY && !(vmf->flags & FAULT_FLAG_RETRY_NOWAIT))
		return ret;

	dma_resv_unlock(bo->base.resv);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_fault);

void ttm_bo_vm_open(struct vm_area_struct *vma)
{
	struct ttm_buffer_object *bo = vma->vm_private_data;

#ifdef __linux__
	WARN_ON(bo->bdev->dev_mapping != vma->vm_file->f_mapping);
#endif

	ttm_bo_get(bo);
}
EXPORT_SYMBOL(ttm_bo_vm_open);

void ttm_bo_vm_close(struct vm_area_struct *vma)
{
	struct ttm_buffer_object *bo = vma->vm_private_data;

	ttm_bo_put(bo);
	vma->vm_private_data = NULL;
}
EXPORT_SYMBOL(ttm_bo_vm_close);

static int ttm_bo_vm_access_kmap(struct ttm_buffer_object *bo,
				 unsigned long offset,
				 uint8_t *buf, int len, int write)
{
	unsigned long page = offset >> PAGE_SHIFT;
	unsigned long bytes_left = len;
	int ret;

	/* Copy a page at a time, that way no extra virtual address
	 * mapping is needed
	 */
	offset -= page << PAGE_SHIFT;
	do {
		unsigned long bytes = min(bytes_left, PAGE_SIZE - offset);
		struct ttm_bo_kmap_obj map;
		void *ptr;
		bool is_iomem;

		ret = ttm_bo_kmap(bo, page, 1, &map);
		if (ret)
			return ret;

		ptr = (uint8_t *)ttm_kmap_obj_virtual(&map, &is_iomem) + offset;
		WARN_ON_ONCE(is_iomem);
		if (write)
			memcpy(ptr, buf, bytes);
		else
			memcpy(buf, ptr, bytes);
		ttm_bo_kunmap(&map);

		page++;
		buf += bytes;
		bytes_left -= bytes;
		offset = 0;
	} while (bytes_left);

	return len;
}

int ttm_bo_vm_access(struct vm_area_struct *vma, unsigned long addr,
		     void *buf, int len, int write)
{
	unsigned long offset = (addr) - vma->vm_start;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	int ret;

	if (len < 1 || (offset + len) >> PAGE_SHIFT > bo->num_pages)
		return -EIO;

	ret = ttm_bo_reserve(bo, true, false, NULL);
	if (ret)
		return ret;

	switch (bo->mem.mem_type) {
	case TTM_PL_SYSTEM:
		if (unlikely(bo->ttm->page_flags & TTM_PAGE_FLAG_SWAPPED)) {
			ret = ttm_tt_swapin(bo->ttm);
			if (unlikely(ret != 0))
				return ret;
		}
		/* fall through */
	case TTM_PL_TT:
		ret = ttm_bo_vm_access_kmap(bo, offset, buf, len, write);
		break;
	default:
		if (bo->bdev->driver->access_memory)
			ret = bo->bdev->driver->access_memory(
				bo, offset, buf, len, write);
		else
			ret = -EIO;
	}

	ttm_bo_unreserve(bo);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_access);

static const struct vm_operations_struct ttm_bo_vm_ops = {
	.fault = ttm_bo_vm_fault,
	.open = ttm_bo_vm_open,
	.close = ttm_bo_vm_close,
	.access = ttm_bo_vm_access
};

static struct ttm_buffer_object *ttm_bo_vm_lookup(struct ttm_bo_device *bdev,
						  unsigned long offset,
						  unsigned long pages)
{
	struct drm_vma_offset_node *node;
	struct ttm_buffer_object *bo = NULL;

	drm_vma_offset_lock_lookup(bdev->vma_manager);

	node = drm_vma_offset_lookup_locked(bdev->vma_manager, offset, pages);
	if (likely(node)) {
		bo = container_of(node, struct ttm_buffer_object,
				  base.vma_node);
		bo = ttm_bo_get_unless_zero(bo);
	}

	drm_vma_offset_unlock_lookup(bdev->vma_manager);

	if (!bo)
		pr_err("Could not find buffer object to map\n");

	return bo;
}

static void ttm_bo_mmap_vma_setup(struct ttm_buffer_object *bo, struct vm_area_struct *vma)
{
	vma->vm_ops = &ttm_bo_vm_ops;

	/*
	 * Note: We're transferring the bo reference to
	 * vma->vm_private_data here.
	 */

	vma->vm_private_data = bo;

	/*
	 * We'd like to use VM_PFNMAP on shared mappings, where
	 * (vma->vm_flags & VM_SHARED) != 0, for performance reasons,
	 * but for some reason VM_PFNMAP + x86 PAT + write-combine is very
	 * bad for performance. Until that has been sorted out, use
	 * VM_MIXEDMAP on all mappings. See freedesktop.org bug #75719
	 */
	vma->vm_flags |= VM_MIXEDMAP;
	vma->vm_flags |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP;
}

int ttm_bo_mmap(struct file *filp, struct vm_area_struct *vma,
		struct ttm_bo_device *bdev)
{
	struct ttm_bo_driver *driver;
	struct ttm_buffer_object *bo;
	int ret;

	if (unlikely(vma->vm_pgoff < DRM_FILE_PAGE_OFFSET_START))
		return -EINVAL;

	bo = ttm_bo_vm_lookup(bdev, vma->vm_pgoff, vma_pages(vma));
	if (unlikely(!bo))
		return -EINVAL;

	driver = bo->bdev->driver;
	if (unlikely(!driver->verify_access)) {
		ret = -EPERM;
		goto out_unref;
	}
	ret = driver->verify_access(bo, filp);
	if (unlikely(ret != 0))
		goto out_unref;

	ttm_bo_mmap_vma_setup(bo, vma);
	return 0;
out_unref:
	ttm_bo_put(bo);
	return ret;
}
EXPORT_SYMBOL(ttm_bo_mmap);

int ttm_bo_mmap_obj(struct vm_area_struct *vma, struct ttm_buffer_object *bo)
{
	ttm_bo_get(bo);
	ttm_bo_mmap_vma_setup(bo, vma);
	return 0;
}
EXPORT_SYMBOL(ttm_bo_mmap_obj);
