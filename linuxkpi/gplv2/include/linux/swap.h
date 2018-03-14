#ifndef _LINUX_SWAP_H
#define _LINUX_SWAP_H

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

#endif
