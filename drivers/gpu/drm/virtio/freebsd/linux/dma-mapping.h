/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Sgtable-sync wrappers, which base LinuxKPI lacks.  Only reachable
 * when VIRTIO_F_ACCESS_PLATFORM is negotiated.
 */

#ifndef _VIRTGPU_SHIM_LINUX_DMA_MAPPING_H
#define	_VIRTGPU_SHIM_LINUX_DMA_MAPPING_H

#include_next <linux/dma-mapping.h>

static inline void
dma_sync_sgtable_for_device(struct device *dev, struct sg_table *sgt,
    enum dma_data_direction dir)
{
	dma_sync_sg_for_device(dev, sgt->sgl, sgt->orig_nents, dir);
}

#endif /* _VIRTGPU_SHIM_LINUX_DMA_MAPPING_H */
