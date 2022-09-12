#ifndef _BSD_LKPI_LINUX_MM_H_
#define	_BSD_LKPI_LINUX_MM_H_

#include_next <linux/mm.h>

#if __FreeBSD_version < 1301507
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

#endif	/* _BSD_LKPI_LINUX_MM_H_ */
