#ifndef _LINUX_PAGEMAP_H
#define _LINUX_PAGEMAP_H

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/highmem.h>
#include <linux/compiler.h>
#include <linux/hardirq.h>
#include <linux/mm.h>


static inline int
fault_in_multipages_readable(const char __user *uaddr, int size)
{
	char c;
	int ret = 0;
	const char __user *end = uaddr + size - 1;

	if (unlikely(size == 0))
		return ret;

	while (uaddr <= end) {
		ret = -copyin(uaddr, &c, 1);
		if (ret != 0)
			return -EFAULT;
		uaddr += PAGE_SIZE;
	}

	/* Check whether the range spilled into the next page. */
	if (((unsigned long)uaddr & ~(PAGE_SIZE - 1)) ==
			((unsigned long)end & ~(PAGE_SIZE - 1))) {
		ret = -copyin(end, &c, 1);
	}

	return ret;
}

static inline int
fault_in_multipages_writeable(char __user *uaddr, int size)
{
	int ret = 0;
	char __user *end = uaddr + size - 1;

	if (unlikely(size == 0))
		return ret;

	/*
	 * Writing zeroes into userspace here is OK, because we know that if
	 * the zero gets there, we'll be overwriting it.
	 */
	while (uaddr <= end) {
		ret = subyte(uaddr, 0);
		if (ret != 0)
			return -EFAULT;
		uaddr += PAGE_SIZE;
	}

	/* Check whether the range spilled into the next page. */
	if (((unsigned long)uaddr & ~(PAGE_SIZE - 1)) ==
			((unsigned long)end & ~(PAGE_SIZE - 1)))
		ret = subyte(end, 0);

	return ret;
}

static inline int
fault_in_pages_writeable(char __user *uaddr, int size)
{
	return (fault_in_multipages_writeable(uaddr, size));
}

static inline int
fault_in_pages_readable(char __user *uaddr, int size)
{
	return (fault_in_multipages_readable(uaddr, size));
}
	
static inline void
release_pages(struct page **pages, int nr)
{
	int i;

	for (i = 0; i < nr; i++)
		put_page(pages[i]);
}

#endif
