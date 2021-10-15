#ifndef _BSD_LKPI_LINUX_SWAP_H_
#define	_BSD_LKPI_LINUX_SWAP_H_

#include <linux/types.h>

static inline long
get_nr_swap_pages(void)
{
	UNIMPLEMENTED();
	return 0;
}

static inline int
current_is_kswapd(void)
{
	UNIMPLEMENTED();
	return 0;
}

#endif	/* _BSD_LKPI_LINUX_SWAP_H_ */
