/*
 * Copyright © 2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <linux/mm.h>
#include <linux/io-mapping.h>


#include "i915_drv.h"
#include "i915_mm.h"

#ifdef __FreeBSD__
#include <vm/vm_pageout.h>
#define	pte_t	linux_pte_t
#define	apply_to_page_range(dummy, ...)	remap_page_range(__VA_ARGS__)
#define	flush_cache_range(...)
#endif

struct remap_pfn {
	struct mm_struct *mm;
#ifdef __FreeBSD__
	struct vm_area_struct *vma;
#endif
	unsigned long pfn;
	pgprot_t prot;

	struct sgt_iter sgt;
	resource_size_t iobase;
};

#ifdef __FreeBSD__
static int
remap_page_range(unsigned long start_addr, unsigned long size,
    pte_fn_t fn,  struct remap_pfn *r)
{
	vm_object_t vm_obj = r->vma->vm_obj;
	unsigned long addr;
	int err = 0;

	VM_OBJECT_WLOCK(vm_obj);
	for (addr = start_addr; addr < start_addr + size; addr += PAGE_SIZE) {
retry:
		err = fn(0, addr, r);
		if (err == -ENOMEM) {
			VM_OBJECT_WUNLOCK(vm_obj);
			vm_wait(NULL);
			VM_OBJECT_WLOCK(vm_obj);
			goto retry;
		}
		if (err != 0)
			break;
	}
	VM_OBJECT_WUNLOCK(vm_obj);

	return (err);
}
#endif

#define use_dma(io) ((io) != -1)

static inline unsigned long sgt_pfn(const struct remap_pfn *r)
{
	if (use_dma(r->iobase))
		return (r->sgt.dma + r->sgt.curr + r->iobase) >> PAGE_SHIFT;
	else
		return r->sgt.pfn + (r->sgt.curr >> PAGE_SHIFT);
}

static int remap_sg(pte_t *pte, unsigned long addr, void *data)
{
	struct remap_pfn *r = data;

	if (GEM_WARN_ON(!r->sgt.sgp))
		return -EINVAL;

#ifdef __linux__
	/* Special PTE are not associated with any struct page */
	set_pte_at(r->mm, addr, pte,
		   pte_mkspecial(pfn_pte(sgt_pfn(r), r->prot)));
#elif defined(__FreeBSD__)
	vm_fault_t ret =
	    lkpi_vmf_insert_pfn_prot_locked(r->vma, addr, sgt_pfn(r), r->prot);
	if ((ret & VM_FAULT_OOM) != 0)
		return -ENOMEM;
	if ((ret & VM_FAULT_ERROR) != 0)
		return -EFAULT;
#endif
	r->pfn++; /* track insertions in case we need to unwind later */

	r->sgt.curr += PAGE_SIZE;
	if (r->sgt.curr >= r->sgt.max)
		r->sgt = __sgt_iter(__sg_next(r->sgt.sgp), use_dma(r->iobase));

	return 0;
}

#define EXPECTED_FLAGS (VM_PFNMAP | VM_DONTEXPAND | VM_DONTDUMP)

#if IS_ENABLED(CONFIG_X86)
static int remap_pfn(pte_t *pte, unsigned long addr, void *data)
{
	struct remap_pfn *r = data;

#ifdef __linux__
	/* Special PTE are not associated with any struct page */
	set_pte_at(r->mm, addr, pte, pte_mkspecial(pfn_pte(r->pfn, r->prot)));
#elif defined(__FreeBSD__)
	vm_fault_t ret;
	ret = lkpi_vmf_insert_pfn_prot_locked(r->vma, addr, r->pfn, r->prot);
	if ((ret & VM_FAULT_OOM) != 0)
		return -ENOMEM;
	if ((ret & VM_FAULT_ERROR) != 0)
		return -EFAULT;
#endif
	r->pfn++;

	return 0;
}

/**
 * remap_io_mapping - remap an IO mapping to userspace
 * @vma: user vma to map to
 * @addr: target user address to start at
 * @pfn: physical address of kernel memory
 * @size: size of map area
 * @iomap: the source io_mapping
 *
 *  Note: this is only safe if the mm semaphore is held when called.
 */
int remap_io_mapping(struct vm_area_struct *vma,
		     unsigned long addr, unsigned long pfn, unsigned long size,
		     struct io_mapping *iomap)
{
	struct remap_pfn r;
	int err;

	GEM_BUG_ON((vma->vm_flags & EXPECTED_FLAGS) != EXPECTED_FLAGS);

	/* We rely on prevalidation of the io-mapping to skip track_pfn(). */
	r.mm = vma->vm_mm;
	r.pfn = pfn;
#ifdef __linux__
	r.prot = __pgprot((pgprot_val(iomap->prot) & _PAGE_CACHE_MASK) |
			  (pgprot_val(vma->vm_page_prot) & ~_PAGE_CACHE_MASK));
#elif defined(__FreeBSD__)
	r.vma = vma;
	r.prot = cachemode2protval(iomap->attr);
#endif

	err = apply_to_page_range(r.mm, addr, size, remap_pfn, &r);
	if (unlikely(err)) {
		zap_vma_ptes(vma, addr, (r.pfn - pfn) << PAGE_SHIFT);
		return err;
	}

	return 0;
}
#endif

/**
 * remap_io_sg - remap an IO mapping to userspace
 * @vma: user vma to map to
 * @addr: target user address to start at
 * @size: size of map area
 * @sgl: Start sg entry
 * @offset: offset from the start of the page
 * @iobase: Use stored dma address offset by this address or pfn if -1
 *
 *  Note: this is only safe if the mm semaphore is held when called.
 */
int remap_io_sg(struct vm_area_struct *vma,
		unsigned long addr, unsigned long size,
		struct scatterlist *sgl, unsigned long offset,
		resource_size_t iobase)
{
	struct remap_pfn r = {
		.mm = vma->vm_mm,
#ifdef __FreeBSD__
		.vma = vma,
#endif
		.prot = vma->vm_page_prot,
		.sgt = __sgt_iter(sgl, use_dma(iobase)),
		.iobase = iobase,
	};
	int err;

	/* We rely on prevalidation of the io-mapping to skip track_pfn(). */
	GEM_BUG_ON((vma->vm_flags & EXPECTED_FLAGS) != EXPECTED_FLAGS);

	while (offset >= sg_dma_len(r.sgt.sgp) >> PAGE_SHIFT) {
		offset -= sg_dma_len(r.sgt.sgp) >> PAGE_SHIFT;
		r.sgt = __sgt_iter(__sg_next(r.sgt.sgp), use_dma(iobase));
		if (!r.sgt.sgp)
			return -EINVAL;
	}
	r.sgt.curr = offset << PAGE_SHIFT;

	if (!use_dma(iobase))
		flush_cache_range(vma, addr, size);

	err = apply_to_page_range(r.mm, addr, size, remap_sg, &r);
	if (unlikely(err)) {
		zap_vma_ptes(vma, addr, r.pfn << PAGE_SHIFT);
		return err;
	}

	return 0;
}
