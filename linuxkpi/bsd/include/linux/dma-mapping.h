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

static inline int
dma_map_sgtable(struct device *dev, struct sg_table *sgt,
    enum dma_data_direction dir,
    unsigned long attrs)
{

	return (dma_map_sg_attrs(dev, sgt->sgl, sgt->nents, dir, attrs));
}

static inline void
dma_unmap_sgtable(struct device *dev, struct sg_table *sgt,
    enum dma_data_direction dir,
    unsigned long attrs __unused)
{

	dma_unmap_sg(dev, sgt->sgl, sgt->nents, dir);
}

#if __FreeBSD_version < 1301502
static inline size_t
dma_max_mapping_size(struct device *dev)
{

	return (SCATTERLIST_MAX_SEGMENT);
}
#endif

#endif	/* _BSD_LKPI_LINUX_DMA_MAPPING_H_ */
