/*-
 * Copyright (c) 2010 Isilon Systems, Inc.
 * Copyright (c) 2016 Matt Macy (mmacy@nextbsd.org)
 * Copyright (c) 2017 Mellanox Technologies, Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>

#include <sys/lock.h>
#include <sys/rwlock.h>

#include <machine/atomic.h>

#include <vm/vm.h>
#include <vm/vm_param.h>
#include <vm/vm_page.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vm_pager.h>

#include <linux/io.h>
#include <linux/mm.h>
#include <linux/page.h>
#include <linux/pfn_t.h>
#include <linux/vmalloc.h>

#if defined(__amd64__) || defined(__aarch64__) || defined(__riscv__)
#define	LINUXKPI_HAVE_DMAP
#else
#undef	LINUXKPI_HAVE_DMAP
#endif

extern u_int	cpu_feature;
extern u_int	cpu_stdext_feature;

void
__linux_clflushopt(u_long addr)
{
	if (cpu_stdext_feature & CPUID_STDEXT_CLFLUSHOPT)
		clflushopt(addr);
	else if (cpu_feature & CPUID_CLFSH)
		clflush(addr);
	else
		pmap_invalidate_cache();
}

void *
kmap(vm_page_t page)
{
#ifdef LINUXKPI_HAVE_DMAP
	vm_offset_t daddr;

	daddr = PHYS_TO_DMAP(VM_PAGE_TO_PHYS(page));

	return ((void *)daddr);
#else
	struct sf_buf *sf;

	sched_pin();
	sf = sf_buf_alloc(page, SFB_NOWAIT | SFB_CPUPRIVATE);
	if (sf == NULL) {
		sched_unpin();
		return (NULL);
	}
	return ((void *)sf_buf_kva(sf));
#endif
}

void *
kmap_atomic_prot(vm_page_t page, pgprot_t prot)
{
	vm_memattr_t attr = pgprot2cachemode(prot);

	if (attr != VM_MEMATTR_DEFAULT) {
		vm_page_lock(page);
		page->flags |= PG_FICTITIOUS;
		vm_page_unlock(page);
		pmap_page_set_memattr(page, attr);
	}
	return (kmap(page));
}

void *
kmap_atomic(vm_page_t page)
{
	return (kmap_atomic_prot(page, VM_PROT_ALL));
}

void
kunmap(vm_page_t page)
{
#ifdef LINUXKPI_HAVE_DMAP
	/* NOP */
#else
	struct sf_buf *sf;

	/* lookup SF buffer in list */
	sf = sf_buf_alloc(page, SFB_NOWAIT | SFB_CPUPRIVATE);

	/* double-free */
	sf_buf_free(sf);
	sf_buf_free(sf);

	sched_unpin();
#endif
}

void
kunmap_atomic(void *vaddr)
{
#ifdef LINUXKPI_HAVE_DMAP
	/* NOP */
#else
	struct sf_buf *sf;
	vm_page_t page;

	page = virt_to_page(vaddr);

	/* lookup SF buffer in list */
	sf = sf_buf_alloc(page, SFB_NOWAIT | SFB_CPUPRIVATE);

	/* double-free */
	sf_buf_free(sf);
	sf_buf_free(sf);

	sched_unpin();
#endif
}

int
set_memory_uc(unsigned long addr, int numpages)
{
	return (pmap_change_attr(addr, numpages, VM_MEMATTR_UNCACHEABLE));
}

int
set_pages_uc(vm_page_t page, int numpages)
{
	KASSERT(numpages == 1, ("%s: numpages %d", __func__, numpages));

	pmap_page_set_memattr(page, VM_MEMATTR_UNCACHEABLE);
	return (0);
}

int
set_memory_wc(unsigned long addr, int numpages)
{
	return (pmap_change_attr(addr, numpages, PAT_WRITE_COMBINING));
}

int
set_pages_wc(vm_page_t page, int numpages)
{
	KASSERT(numpages == 1, ("%s: numpages %d", __func__, numpages));

	pmap_page_set_memattr(page, VM_MEMATTR_WRITE_COMBINING);
	return (0);
}

int
set_memory_wb(unsigned long addr, int numpages)
{
	return (pmap_change_attr(addr, numpages, PAT_WRITE_BACK));
}

int
set_pages_wb(vm_page_t page, int numpages)
{
	KASSERT(numpages == 1, ("%s: numpages %d", __func__, numpages));

	pmap_page_set_memattr(page, VM_MEMATTR_WRITE_BACK);
	return (0);
}

int
arch_io_reserve_memtype_wc(resource_size_t start, resource_size_t size)
{
	return (set_memory_wc(start, size >> PAGE_SHIFT));
}

void
arch_io_free_memtype_wc(resource_size_t start, resource_size_t size)
{
	set_memory_wb(start, size >> PAGE_SHIFT);
}

void *
linux_page_address(struct page *page)
{

	if (page->object != kmem_object && page->object != kernel_object) {
#ifdef LINUXKPI_HAVE_DMAP
		return ((void *)PHYS_TO_DMAP(VM_PAGE_TO_PHYS(page)));
#else
		return (NULL);
#endif
	}
	return ((void *)(uintptr_t)(VM_MIN_KERNEL_ADDRESS +
	    IDX_TO_OFF(page->pindex)));
}

vm_paddr_t
page_to_phys(vm_page_t page)
{
	return (VM_PAGE_TO_PHYS(page));
}

void
unmap_mapping_range(void *obj, loff_t const holebegin, loff_t const holelen, int even_cows)
{
	vm_object_t devobj;
	vm_page_t page;
	int i, page_count;

#ifdef LINUX_VERBOSE_DEBUG
	BACKTRACE();
	printf("unmap_mapping_range: obj: %p holebegin %zu, holelen: %zu, even_cows: %d\n",
	       obj, holebegin, holelen, even_cows);
#endif
	devobj = cdev_pager_lookup(obj);
	if (devobj != NULL) {
		page_count = OFF_TO_IDX(holelen);

		VM_OBJECT_WLOCK(devobj);
retry:
		for (i = 0; i < page_count; i++) {
			page = vm_page_lookup(devobj, i);
			if (page == NULL)
				continue;
			if (vm_page_sleep_if_busy(page, "linuxkpi"))
				goto retry;
			cdev_pager_free_page(devobj, page);
		}
		VM_OBJECT_WUNLOCK(devobj);
		vm_object_deallocate(devobj);
	}
}

#if defined(__i386__) || defined(__amd64__)
int
set_pages_array_wb(struct page **pages, int addrinarray)
{
	int i;

	for (i = 0; i < addrinarray; i++)
		set_pages_wb(pages[i], 1);
	return (0);
}

int
set_pages_array_wc(struct page **pages, int addrinarray)
{
	int i;

	for (i = 0; i < addrinarray; i++)
		set_pages_wc(pages[i], 1);
	return (0);
}

int
set_pages_array_uc(struct page **pages, int addrinarray)
{
	int i;

	for (i = 0; i < addrinarray; i++)
		set_pages_uc(pages[i], 1);
	return (0);
}
#endif
