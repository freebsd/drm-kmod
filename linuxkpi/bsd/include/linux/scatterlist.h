#ifndef _BSD_LKPI_LINUX_SCATTERLIST_H_
#define _BSD_LKPI_LINUX_SCATTERLIST_H_

#include_next <linux/scatterlist.h>

#define	for_each_sgtable_sg(sgt, sg, i) \
	for_each_sg((sgt)->sgl, sg, (sgt)->orig_nents, i)

#define	for_each_sgtable_page(sgt, iter, pgoffset) \
	for_each_sg_page((sgt)->sgl, iter, (sgt)->orig_nents, pgoffset)

#endif
