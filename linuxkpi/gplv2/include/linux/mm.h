#ifndef __GPLV2_MM_H__
#define	__GPLV2_MM_H__

#include_next <linux/mm.h>

static inline int
trylock_page(struct page *page)
{
	return (vm_page_trylock(page));
}

static inline void
unlock_page(struct page *page)
{

	vm_page_unlock(page);
}

#endif
