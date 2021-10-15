#ifndef _BSD_LKPI_LINUX_PAGEMAP_H_
#define	_BSD_LKPI_LINUX_PAGEMAP_H_

#include <linux/highmem.h>

#define	mapping_gfp_mask(x) 0

#include_next <linux/pagemap.h>

#endif	/* _BSD_LKPI_LINUX_PAGEMAP_H_ */
