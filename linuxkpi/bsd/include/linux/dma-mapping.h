#ifndef _BSD_LKPI_LINUX_DMA_MAPPING_H_
#define _BSD_LKPI_LINUX_DMA_MAPPING_H_

#include_next <linux/dma-mapping.h>

/*
 * dma_(un)map_resource implementation should belong to LKPI from base system
 * and should stay in sys/compat/linuxkpi/common/include/linux/dma-mapping.h.
 * See https://reviews.freebsd.org/D30933
 * It temporarily left here as untested and should be replaced with patch from
 * aforementioned review after it gets proven to work.
 */
#define dma_map_resource(dev, phys_addr, size, dir, attrs)      \
	linux_dma_map_phys(dev, phys_addr, size)
#define dma_unmap_resource(dev, dma_addr, size, dir, attrs)     \
	linux_dma_unmap(dev, dma_addr, size)

#endif	/* _BSD_LKPI_LINUX_DMA_MAPPING_H_ */
