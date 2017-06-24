#ifndef _LINUX_GPLV2_DMA_MAPPING_H_
#define	_LINUX_GPLV2_DMA_MAPPING_H_

#include_next <linux/dma-mapping.h>

#include <linux/compiler.h>

static inline int
dma_get_sgtable_attrs(struct device *dev, struct sg_table *sgt, void *cpu_addr,
		      dma_addr_t dma_addr, size_t size, struct dma_attrs *attrs)
{

	UNIMPLEMENTED();
	return (-ENOMEM);
}

#define dma_get_sgtable(d, t, v, h, s) dma_get_sgtable_attrs(d, t, v, h, s, NULL)

static inline void *dma_alloc_wc(struct device *dev, size_t size,
				 dma_addr_t *dma_addr, gfp_t gfp)
{

	panic("implement me");
}
#ifndef dma_alloc_writecombine
#define dma_alloc_writecombine dma_alloc_wc
#endif

static inline void dma_free_wc(struct device *dev, size_t size,
			       void *cpu_addr, dma_addr_t dma_addr)
{
	panic("implement me");
}
#ifndef dma_free_writecombine
#define dma_free_writecombine dma_free_wc
#endif

static inline int dma_mmap_wc(struct device *dev,
			      struct vm_area_struct *vma,
			      void *cpu_addr, dma_addr_t dma_addr,
			      size_t size)
{
	panic("implement me!!!");
}

#endif /* _LINUX_GPLV2_DMA_MAPPING_H_ */
