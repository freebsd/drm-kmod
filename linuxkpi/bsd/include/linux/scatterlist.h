#ifndef _BSD_LKPI_LINUX_SCATTERLIST_H_
#define _BSD_LKPI_LINUX_SCATTERLIST_H_

#include_next <linux/scatterlist.h>

#define	for_each_sgtable_sg(sgt, sg, i) \
	for_each_sg((sgt)->sgl, sg, (sgt)->orig_nents, i)

#define	for_each_sgtable_page(sgt, iter, pgoffset) \
	for_each_sg_page((sgt)->sgl, iter, (sgt)->orig_nents, pgoffset)

#ifndef for_each_sgtable_dma_sg
#define	for_each_sgtable_dma_sg(sgt, sg, iter)		\
	for_each_sg((sgt)->sgl, sg, (sgt)->nents, iter)
#endif

#ifndef for_each_sgtable_dma_page
#define	for_each_sgtable_dma_page(sgt, iter, pgoffset)			\
	for_each_sg_dma_page((sgt)->sgl, iter, (sgt)->nents, pgoffset)
#endif

#endif
