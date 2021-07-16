/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2014-2016 Intel Corporation
 */

#include <linux/mman.h>
#include <linux/pfn_t.h>
#include <linux/sizes.h>

#include "gt/intel_gt.h"
#include "gt/intel_gt_requests.h"

#include "i915_drv.h"
#include "i915_gem_gtt.h"
#include "i915_gem_ioctls.h"
#include "i915_gem_object.h"
#include "i915_gem_mman.h"
#include "i915_trace.h"
#include "i915_vma.h"

#ifdef __FreeBSD__
#include <sys/resourcevar.h>	/* for lim_cur_proc */
#define	resource linux_resource
#endif

#ifdef __linux__ /* Mute unused function warning. */
static inline bool
__vma_matches(struct vm_area_struct *vma, struct file *filp,
	      unsigned long addr, unsigned long size)
{
	if (vma->vm_file != filp)
		return false;

	return vma->vm_start == addr &&
	       (vma->vm_end - vma->vm_start) == PAGE_ALIGN(size);
}
#endif

/**
 * i915_gem_mmap_ioctl - Maps the contents of an object, returning the address
 *			 it is mapped to.
 * @dev: drm device
 * @data: ioctl data blob
 * @file: drm file
 *
 * While the mapping holds a reference on the contents of the object, it doesn't
 * imply a ref on the object itself.
 *
 * IMPORTANT:
 *
 * DRM driver writers who look a this function as an example for how to do GEM
 * mmap support, please don't implement mmap support like here. The modern way
 * to implement DRM mmap support is with an mmap offset ioctl (like
 * i915_gem_mmap_gtt) and then using the mmap syscall on the DRM fd directly.
 * That way debug tooling like valgrind will understand what's going on, hiding
 * the mmap call in a driver private ioctl will break that. The i915 driver only
 * does cpu mmaps this way because we didn't know better.
 */
int
i915_gem_mmap_ioctl(struct drm_device *dev, void *data,
		    struct drm_file *file)
{
	struct drm_i915_gem_mmap *args = data;
	struct drm_i915_gem_object *obj;
#ifdef __FreeBSD__
	vm_offset_t addr;
	vm_object_t vmobj;
	struct proc *p;
	vm_map_t map;
	vm_size_t size;
	int error, rv;
#else
	unsigned long addr;
#endif

	if (args->flags & ~(I915_MMAP_WC))
		return -EINVAL;

	if (args->flags & I915_MMAP_WC && !boot_cpu_has(X86_FEATURE_PAT))
		return -ENODEV;

	obj = i915_gem_object_lookup(file, args->handle);
	if (!obj)
		return -ENOENT;

	/* prime objects have no backing filp to GEM mmap
	 * pages from.
	 */
	if (!obj->base.filp) {
		addr = -ENXIO;
		goto err;
	}

	if (range_overflows(args->offset, args->size, (u64)obj->base.size)) {
		addr = -EINVAL;
		goto err;
	}

#ifdef __linux__
	addr = vm_mmap(obj->base.filp, 0, args->size,
		       PROT_READ | PROT_WRITE, MAP_SHARED,
		       args->offset);
	if (IS_ERR_VALUE(addr))
		goto err;

	if (args->flags & I915_MMAP_WC) {
		struct mm_struct *mm = current->mm;
		struct vm_area_struct *vma;

		if (down_write_killable(&mm->mmap_sem)) {
			addr = -EINTR;
			goto err;
		}
		vma = find_vma(mm, addr);
		if (vma && __vma_matches(vma, obj->base.filp, addr, args->size))
			vma->vm_page_prot =
				pgprot_writecombine(vm_get_page_prot(vma->vm_flags));
		else
			addr = -ENOMEM;
		up_write(&mm->mmap_sem);
		if (IS_ERR_VALUE(addr))
			goto err;
	}
#else
	error = 0;
	addr = 0;
	if (args->size == 0)
		goto out;
	p = curproc;
	map = &p->p_vmspace->vm_map;
	size = round_page(args->size);
	PROC_LOCK(p);
	if (map->size + size > lim_cur_proc(p, RLIMIT_VMEM)) {
		PROC_UNLOCK(p);
		error = -ENOMEM;
		goto out;
	}
	PROC_UNLOCK(p);

	vmobj = obj->base.filp->f_shmem;
	vm_object_reference(vmobj);
	rv = vm_map_find(map, vmobj, args->offset, &addr, args->size, 0,
	    VMFS_OPTIMAL_SPACE, VM_PROT_READ | VM_PROT_WRITE,
	    VM_PROT_READ | VM_PROT_WRITE, MAP_INHERIT_SHARE);

	/* currently disabled as it causes artifacts on FreeBSD */
	if ((rv == KERN_SUCCESS) && (args->flags & I915_MMAP_WC)) {
		VM_OBJECT_WLOCK(vmobj);
		if (vm_object_set_memattr(vmobj, VM_MEMATTR_WRITE_COMBINING) != KERN_SUCCESS) {
			for (vm_page_t page = vm_page_find_least(vmobj, 0); page != NULL; page = vm_page_next(page)) {
				pmap_page_set_memattr(page, VM_MEMATTR_WRITE_COMBINING);
			}
		}
		VM_OBJECT_WUNLOCK(vmobj);
	}

	if (rv != KERN_SUCCESS) {
		vm_object_deallocate(vmobj);
		error = -vm_mmap_to_errno(rv);
	} else {
		args->addr_ptr = (uint64_t)addr;
	}

out:
#endif
	i915_gem_object_put(obj);

	args->addr_ptr = (u64)addr;
	return 0;

err:
	i915_gem_object_put(obj);
	return addr;
}

static unsigned int tile_row_pages(const struct drm_i915_gem_object *obj)
{
	return i915_gem_object_get_tile_row_size(obj) >> PAGE_SHIFT;
}

/**
 * i915_gem_mmap_gtt_version - report the current feature set for GTT mmaps
 *
 * A history of the GTT mmap interface:
 *
 * 0 - Everything had to fit into the GTT. Both parties of a memcpy had to
 *     aligned and suitable for fencing, and still fit into the available
 *     mappable space left by the pinned display objects. A classic problem
 *     we called the page-fault-of-doom where we would ping-pong between
 *     two objects that could not fit inside the GTT and so the memcpy
 *     would page one object in at the expense of the other between every
 *     single byte.
 *
 * 1 - Objects can be any size, and have any compatible fencing (X Y, or none
 *     as set via i915_gem_set_tiling() [DRM_I915_GEM_SET_TILING]). If the
 *     object is too large for the available space (or simply too large
 *     for the mappable aperture!), a view is created instead and faulted
 *     into userspace. (This view is aligned and sized appropriately for
 *     fenced access.)
 *
 * 2 - Recognise WC as a separate cache domain so that we can flush the
 *     delayed writes via GTT before performing direct access via WC.
 *
 * 3 - Remove implicit set-domain(GTT) and synchronisation on initial
 *     pagefault; swapin remains transparent.
 *
 * 4 - Support multiple fault handlers per object depending on object's
 *     backing storage (a.k.a. MMAP_OFFSET).
 *
 * Restrictions:
 *
 *  * snoopable objects cannot be accessed via the GTT. It can cause machine
 *    hangs on some architectures, corruption on others. An attempt to service
 *    a GTT page fault from a snoopable object will generate a SIGBUS.
 *
 *  * the object must be able to fit into RAM (physical memory, though no
 *    limited to the mappable aperture).
 *
 *
 * Caveats:
 *
 *  * a new GTT page fault will synchronize rendering from the GPU and flush
 *    all data to system memory. Subsequent access will not be synchronized.
 *
 *  * all mappings are revoked on runtime device suspend.
 *
 *  * there are only 8, 16 or 32 fence registers to share between all users
 *    (older machines require fence register for display and blitter access
 *    as well). Contention of the fence registers will cause the previous users
 *    to be unmapped and any new access will generate new page faults.
 *
 *  * running out of memory while servicing a fault may generate a SIGBUS,
 *    rather than the expected SIGSEGV.
 */
int i915_gem_mmap_gtt_version(void)
{
	return 4;
}

static inline struct i915_ggtt_view
compute_partial_view(const struct drm_i915_gem_object *obj,
		     pgoff_t page_offset,
		     unsigned int chunk)
{
	struct i915_ggtt_view view;

	if (i915_gem_object_is_tiled(obj))
		chunk = roundup(chunk, tile_row_pages(obj));

	view.type = I915_GGTT_VIEW_PARTIAL;
	view.partial.offset = rounddown(page_offset, chunk);
	view.partial.size =
		min_t(unsigned int, chunk,
		      (obj->base.size >> PAGE_SHIFT) - view.partial.offset);

	/* If the partial covers the entire object, just create a normal VMA. */
	if (chunk >= obj->base.size >> PAGE_SHIFT)
		view.type = I915_GGTT_VIEW_NORMAL;

	return view;
}

static vm_fault_t i915_error_to_vmf_fault(int err)
{
	switch (err) {
	default:
		WARN_ONCE(err, "unhandled error in %s: %i\n", __func__, err);
		/* fallthrough */
	case -EIO: /* shmemfs failure from swap device */
	case -EFAULT: /* purged object */
	case -ENODEV: /* bad object, how did you get here! */
		return VM_FAULT_SIGBUS;

	case -ENOSPC: /* shmemfs allocation failure */
	case -ENOMEM: /* our allocation failure */
		return VM_FAULT_OOM;

	case 0:
	case -EAGAIN:
	case -ERESTARTSYS:
	case -EINTR:
	case -EBUSY:
		/*
		 * EBUSY is ok: this just means that another thread
		 * already did the job.
		 */
		return VM_FAULT_NOPAGE;
	}
}

#ifdef __linux__
vm_fault_t vm_fault_cpu(struct vm_fault *vmf)
#elif defined(__FreeBSD__)
vm_fault_t vm_fault_cpu(struct vm_area_struct *dummy, struct vm_fault *vmf)
#endif
{
	struct vm_area_struct *area = vmf->vma;
	struct i915_mmap_offset *mmo = area->vm_private_data;
	struct drm_i915_gem_object *obj = mmo->obj;
	unsigned long i, size = area->vm_end - area->vm_start;
	bool write = area->vm_flags & VM_WRITE;
	vm_fault_t ret = VM_FAULT_SIGBUS;
	int err;

	if (!i915_gem_object_has_struct_page(obj))
		return ret;

	/* Sanity check that we allow writing into this object */
	if (i915_gem_object_is_readonly(obj) && write)
		return ret;

	err = i915_gem_object_pin_pages(obj);
	if (err)
		return i915_error_to_vmf_fault(err);

	/* PTEs are revoked in obj->ops->put_pages() */
	for (i = 0; i < size >> PAGE_SHIFT; i++) {
		struct page *page = i915_gem_object_get_page(obj, i);

		ret = vmf_insert_pfn(area,
				     (unsigned long)area->vm_start + i * PAGE_SIZE,
				     page_to_pfn(page));
		if (ret != VM_FAULT_NOPAGE)
			break;
	}

	if (write) {
		GEM_BUG_ON(!i915_gem_object_has_pinned_pages(obj));
		obj->cache_dirty = true; /* XXX flush after PAT update? */
		obj->mm.dirty = true;
	}

	i915_gem_object_unpin_pages(obj);

	return ret;
}

#ifdef __linux__
vm_fault_t vm_fault_gtt(struct vm_fault *vmf)
#elif defined(__FreeBSD__)
vm_fault_t vm_fault_gtt(struct vm_area_struct *dummy, struct vm_fault *vmf)
#endif
{
#define MIN_CHUNK_PAGES (SZ_1M >> PAGE_SHIFT)
	struct vm_area_struct *area = vmf->vma;
	struct i915_mmap_offset *mmo = area->vm_private_data;
	struct drm_i915_gem_object *obj = mmo->obj;
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *i915 = to_i915(dev);
	struct intel_runtime_pm *rpm = &i915->runtime_pm;
	struct i915_ggtt *ggtt = &i915->ggtt;
	bool write = area->vm_flags & VM_WRITE;
	intel_wakeref_t wakeref;
	struct i915_vma *vma;
	pgoff_t page_offset;
#ifdef __freebsd_notyet__
	int srcu;
#endif
	int ret;

	/* Sanity check that we allow writing into this object */
	if (i915_gem_object_is_readonly(obj) && write)
		return VM_FAULT_SIGBUS;

	/* We don't use vmf->pgoff since that has the fake offset */
	page_offset = (vmf->address - area->vm_start) >> PAGE_SHIFT;

	trace_i915_gem_object_fault(obj, page_offset, true, write);

	ret = i915_gem_object_pin_pages(obj);
	if (ret)
		goto err;

	wakeref = intel_runtime_pm_get(rpm);

#ifdef __freebsd_notyet__
	ret = intel_gt_reset_trylock(ggtt->vm.gt, &srcu);
	if (ret)
		goto err_rpm;
#endif

	/* Now pin it into the GTT as needed */
	vma = i915_gem_object_ggtt_pin(obj, NULL, 0, 0,
				       PIN_MAPPABLE |
				       PIN_NONBLOCK /* NOWARN */ |
				       PIN_NOEVICT);
	if (IS_ERR(vma)) {
		/* Use a partial view if it is bigger than available space */
		struct i915_ggtt_view view =
			compute_partial_view(obj, page_offset, MIN_CHUNK_PAGES);
		unsigned int flags;

		flags = PIN_MAPPABLE | PIN_NOSEARCH;
		if (view.type == I915_GGTT_VIEW_NORMAL)
			flags |= PIN_NONBLOCK; /* avoid warnings for pinned */

		/*
		 * Userspace is now writing through an untracked VMA, abandon
		 * all hope that the hardware is able to track future writes.
		 */

		vma = i915_gem_object_ggtt_pin(obj, &view, 0, 0, flags);
		if (IS_ERR(vma)) {
			flags = PIN_MAPPABLE;
			view.type = I915_GGTT_VIEW_PARTIAL;
			vma = i915_gem_object_ggtt_pin(obj, &view, 0, 0, flags);
		}

		/* The entire mappable GGTT is pinned? Unexpected! */
		GEM_BUG_ON(vma == ERR_PTR(-ENOSPC));
	}
	if (IS_ERR(vma)) {
		ret = PTR_ERR(vma);
		goto err_reset;
	}

	/* Access to snoopable pages through the GTT is incoherent. */
	if (obj->cache_level != I915_CACHE_NONE && !HAS_LLC(i915)) {
		ret = -EFAULT;
		goto err_unpin;
	}

	ret = i915_vma_pin_fence(vma);
	if (ret)
		goto err_unpin;

	/* Finally, remap it using the new GTT offset */
	ret = remap_io_mapping(area,
			       area->vm_start + (vma->ggtt_view.partial.offset << PAGE_SHIFT),
			       (ggtt->gmadr.start + vma->node.start) >> PAGE_SHIFT,
			       min_t(u64, vma->size, area->vm_end - area->vm_start),
			       &ggtt->iomap);
	if (ret)
		goto err_fence;

	assert_rpm_wakelock_held(rpm);

	/* Mark as being mmapped into userspace for later revocation */
	mutex_lock(&i915->ggtt.vm.mutex);
	if (!i915_vma_set_userfault(vma) && !obj->userfault_count++)
		list_add(&obj->userfault_link, &i915->ggtt.userfault_list);
	mutex_unlock(&i915->ggtt.vm.mutex);

	/* Track the mmo associated with the fenced vma */
	vma->mmo = mmo;

	if (IS_ACTIVE(CONFIG_DRM_I915_USERFAULT_AUTOSUSPEND))
		intel_wakeref_auto(&i915->ggtt.userfault_wakeref,
				   msecs_to_jiffies_timeout(CONFIG_DRM_I915_USERFAULT_AUTOSUSPEND));

	if (write) {
		GEM_BUG_ON(!i915_gem_object_has_pinned_pages(obj));
		i915_vma_set_ggtt_write(vma);
		obj->mm.dirty = true;
	}

err_fence:
	i915_vma_unpin_fence(vma);
err_unpin:
	__i915_vma_unpin(vma);
err_reset:
#ifdef __freebsd_notyet__
	intel_gt_reset_unlock(ggtt->vm.gt, srcu);
err_rpm:
#endif
	intel_runtime_pm_put(rpm, wakeref);
	i915_gem_object_unpin_pages(obj);
err:
	return i915_error_to_vmf_fault(ret);
}

void __i915_gem_object_release_mmap_gtt(struct drm_i915_gem_object *obj)
{
	struct i915_vma *vma;

	GEM_BUG_ON(!obj->userfault_count);

	for_each_ggtt_vma(vma, obj)
		i915_vma_revoke_mmap(vma);

	GEM_BUG_ON(obj->userfault_count);
}

/*
 * It is vital that we remove the page mapping if we have mapped a tiled
 * object through the GTT and then lose the fence register due to
 * resource pressure. Similarly if the object has been moved out of the
 * aperture, than pages mapped into userspace must be revoked. Removing the
 * mapping will then trigger a page fault on the next user access, allowing
 * fixup by vm_fault_gtt().
 */
static void i915_gem_object_release_mmap_gtt(struct drm_i915_gem_object *obj)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	intel_wakeref_t wakeref;

	/*
	 * Serialisation between user GTT access and our code depends upon
	 * revoking the CPU's PTE whilst the mutex is held. The next user
	 * pagefault then has to wait until we release the mutex.
	 *
	 * Note that RPM complicates somewhat by adding an additional
	 * requirement that operations to the GGTT be made holding the RPM
	 * wakeref.
	 */
	wakeref = intel_runtime_pm_get(&i915->runtime_pm);
	mutex_lock(&i915->ggtt.vm.mutex);

	if (!obj->userfault_count)
		goto out;

	__i915_gem_object_release_mmap_gtt(obj);

	/*
	 * Ensure that the CPU's PTE are revoked and there are not outstanding
	 * memory transactions from userspace before we return. The TLB
	 * flushing implied above by changing the PTE above *should* be
	 * sufficient, an extra barrier here just provides us with a bit
	 * of paranoid documentation about our requirement to serialise
	 * memory writes before touching registers / GSM.
	 */
	wmb();

out:
	mutex_unlock(&i915->ggtt.vm.mutex);
	intel_runtime_pm_put(&i915->runtime_pm, wakeref);
}

void i915_gem_object_release_mmap_offset(struct drm_i915_gem_object *obj)
{
	struct i915_mmap_offset *mmo;

	spin_lock(&obj->mmo.lock);
	list_for_each_entry(mmo, &obj->mmo.offsets, offset) {
		/*
		 * vma_node_unmap for GTT mmaps handled already in
		 * __i915_gem_object_release_mmap_gtt
		 */
		if (mmo->mmap_type == I915_MMAP_TYPE_GTT)
			continue;

		spin_unlock(&obj->mmo.lock);
		drm_vma_node_unmap(&mmo->vma_node,
				   obj->base.dev->anon_inode->i_mapping);
		spin_lock(&obj->mmo.lock);
	}
	spin_unlock(&obj->mmo.lock);
}

/**
 * i915_gem_object_release_mmap - remove physical page mappings
 * @obj: obj in question
 *
 * Preserve the reservation of the mmapping with the DRM core code, but
 * relinquish ownership of the pages back to the system.
 */
void i915_gem_object_release_mmap(struct drm_i915_gem_object *obj)
{
	i915_gem_object_release_mmap_gtt(obj);
	i915_gem_object_release_mmap_offset(obj);
}

static struct i915_mmap_offset *
mmap_offset_attach(struct drm_i915_gem_object *obj,
		   enum i915_mmap_type mmap_type,
		   struct drm_file *file)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_mmap_offset *mmo;
	int err;

	mmo = kmalloc(sizeof(*mmo), GFP_KERNEL);
	if (!mmo)
		return ERR_PTR(-ENOMEM);

	mmo->obj = obj;
	mmo->dev = obj->base.dev;
	mmo->file = file;
	mmo->mmap_type = mmap_type;
	drm_vma_node_reset(&mmo->vma_node);

	err = drm_vma_offset_add(mmo->dev->vma_offset_manager, &mmo->vma_node,
				 obj->base.size / PAGE_SIZE);
	if (likely(!err))
		goto out;

	/* Attempt to reap some mmap space from dead objects */
	err = intel_gt_retire_requests_timeout(&i915->gt, MAX_SCHEDULE_TIMEOUT);
	if (err)
		goto err;

	i915_gem_drain_freed_objects(i915);
	err = drm_vma_offset_add(mmo->dev->vma_offset_manager, &mmo->vma_node,
				 obj->base.size / PAGE_SIZE);
	if (err)
		goto err;

out:
	if (file)
		drm_vma_node_allow(&mmo->vma_node, file);

	spin_lock(&obj->mmo.lock);
	list_add(&mmo->offset, &obj->mmo.offsets);
	spin_unlock(&obj->mmo.lock);

	return mmo;

err:
	kfree(mmo);
	return ERR_PTR(err);
}

static int
__assign_mmap_offset(struct drm_file *file,
		     u32 handle,
		     enum i915_mmap_type mmap_type,
		     u64 *offset)
{
	struct drm_i915_gem_object *obj;
	struct i915_mmap_offset *mmo;
	int err;

	obj = i915_gem_object_lookup(file, handle);
	if (!obj)
		return -ENOENT;

	if (mmap_type == I915_MMAP_TYPE_GTT &&
	    i915_gem_object_never_bind_ggtt(obj)) {
		err = -ENODEV;
		goto out;
	}

	if (mmap_type != I915_MMAP_TYPE_GTT &&
	    !i915_gem_object_has_struct_page(obj)) {
		err = -ENODEV;
		goto out;
	}

	mmo = mmap_offset_attach(obj, mmap_type, file);
	if (IS_ERR(mmo)) {
		err = PTR_ERR(mmo);
		goto out;
	}

	*offset = drm_vma_node_offset_addr(&mmo->vma_node);
	err = 0;
out:
	i915_gem_object_put(obj);
	return err;
}

int
i915_gem_dumb_mmap_offset(struct drm_file *file,
			  struct drm_device *dev,
			  u32 handle,
			  u64 *offset)
{
	enum i915_mmap_type mmap_type;

	if (boot_cpu_has(X86_FEATURE_PAT))
		mmap_type = I915_MMAP_TYPE_WC;
	else if (!i915_ggtt_has_aperture(&to_i915(dev)->ggtt))
		return -ENODEV;
	else
		mmap_type = I915_MMAP_TYPE_GTT;

	return __assign_mmap_offset(file, handle, mmap_type, offset);
}

/**
 * i915_gem_mmap_offset_ioctl - prepare an object for GTT mmap'ing
 * @dev: DRM device
 * @data: GTT mapping ioctl data
 * @file: GEM object info
 *
 * Simply returns the fake offset to userspace so it can mmap it.
 * The mmap call will end up in drm_gem_mmap(), which will set things
 * up so we can get faults in the handler above.
 *
 * The fault handler will take care of binding the object into the GTT
 * (since it may have been evicted to make room for something), allocating
 * a fence register, and mapping the appropriate aperture address into
 * userspace.
 */
int
i915_gem_mmap_offset_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file)
{
	struct drm_i915_private *i915 = to_i915(dev);
	struct drm_i915_gem_mmap_offset *args = data;
	enum i915_mmap_type type;

	if (args->extensions)
		return -EINVAL;

	switch (args->flags) {
	case I915_MMAP_OFFSET_GTT:
		if (!i915_ggtt_has_aperture(&i915->ggtt))
			return -ENODEV;
		type = I915_MMAP_TYPE_GTT;
		break;

	case I915_MMAP_OFFSET_WC:
		if (!boot_cpu_has(X86_FEATURE_PAT))
			return -ENODEV;
		type = I915_MMAP_TYPE_WC;
		break;

	case I915_MMAP_OFFSET_WB:
		type = I915_MMAP_TYPE_WB;
		break;

	case I915_MMAP_OFFSET_UC:
		if (!boot_cpu_has(X86_FEATURE_PAT))
			return -ENODEV;
		type = I915_MMAP_TYPE_UC;
		break;

	default:
		return -EINVAL;
	}

	return __assign_mmap_offset(file, args->handle, type, &args->offset);
}

static void vm_open(struct vm_area_struct *vma)
{
	struct i915_mmap_offset *mmo = vma->vm_private_data;
	struct drm_i915_gem_object *obj = mmo->obj;

	GEM_BUG_ON(!obj);
	i915_gem_object_get(obj);
}

static void vm_close(struct vm_area_struct *vma)
{
	struct i915_mmap_offset *mmo = vma->vm_private_data;
	struct drm_i915_gem_object *obj = mmo->obj;

	GEM_BUG_ON(!obj);
	i915_gem_object_put(obj);
}

static const struct vm_operations_struct vm_ops_gtt = {
	.fault = vm_fault_gtt,
	.open = vm_open,
	.close = vm_close,
};

static const struct vm_operations_struct vm_ops_cpu = {
	.fault = vm_fault_cpu,
	.open = vm_open,
	.close = vm_close,
};

/*
 * This overcomes the limitation in drm_gem_mmap's assignment of a
 * drm_gem_object as the vma->vm_private_data. Since we need to
 * be able to resolve multiple mmap offsets which could be tied
 * to a single gem object.
 */
int i915_gem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct drm_vma_offset_node *node;
	struct drm_file *priv = filp->private_data;
	struct drm_device *dev = priv->minor->dev;
	struct i915_mmap_offset *mmo = NULL;
	struct drm_gem_object *obj = NULL;

	if (drm_dev_is_unplugged(dev))
		return -ENODEV;

	drm_vma_offset_lock_lookup(dev->vma_offset_manager);
	node = drm_vma_offset_exact_lookup_locked(dev->vma_offset_manager,
						  vma->vm_pgoff,
						  vma_pages(vma));
	if (likely(node)) {
		mmo = container_of(node, struct i915_mmap_offset,
				   vma_node);
		/*
		 * In our dependency chain, the drm_vma_offset_node
		 * depends on the validity of the mmo, which depends on
		 * the gem object. However the only reference we have
		 * at this point is the mmo (as the parent of the node).
		 * Try to check if the gem object was at least cleared.
		 */
		if (!mmo || !mmo->obj) {
			drm_vma_offset_unlock_lookup(dev->vma_offset_manager);
			return -EINVAL;
		}
		/*
		 * Skip 0-refcnted objects as it is in the process of being
		 * destroyed and will be invalid when the vma manager lock
		 * is released.
		 */
		obj = &mmo->obj->base;
		if (!kref_get_unless_zero(&obj->refcount))
			obj = NULL;
	}
	drm_vma_offset_unlock_lookup(dev->vma_offset_manager);
	if (!obj)
		return -EINVAL;

	if (!drm_vma_node_is_allowed(node, priv)) {
		drm_gem_object_put_unlocked(obj);
		return -EACCES;
	}

	if (i915_gem_object_is_readonly(to_intel_bo(obj))) {
		if (vma->vm_flags & VM_WRITE) {
			drm_gem_object_put_unlocked(obj);
			return -EINVAL;
		}
		vma->vm_flags &= ~VM_MAYWRITE;
	}

	vma->vm_flags |= VM_PFNMAP | VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_private_data = mmo;

	switch (mmo->mmap_type) {
	case I915_MMAP_TYPE_WC:
		vma->vm_page_prot =
			pgprot_writecombine(vm_get_page_prot(vma->vm_flags));
		vma->vm_ops = &vm_ops_cpu;
		break;

	case I915_MMAP_TYPE_WB:
		vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
		vma->vm_ops = &vm_ops_cpu;
		break;

	case I915_MMAP_TYPE_UC:
		vma->vm_page_prot =
			pgprot_noncached(vm_get_page_prot(vma->vm_flags));
		vma->vm_ops = &vm_ops_cpu;
		break;

	case I915_MMAP_TYPE_GTT:
		vma->vm_page_prot =
			pgprot_writecombine(vm_get_page_prot(vma->vm_flags));
		vma->vm_ops = &vm_ops_gtt;
		break;
	}
	vma->vm_page_prot = pgprot_decrypted(vma->vm_page_prot);

	return 0;
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/i915_gem_mman.c"
#endif
