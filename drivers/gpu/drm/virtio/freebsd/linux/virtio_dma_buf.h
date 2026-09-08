/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Linux's virtio dma-buf helpers, mirroring drivers/virtio/
 * virtio_dma_buf.c.  All PRIME exports route through these; the UUID
 * paths stay dormant (RESOURCE_UUID is rejected by this port).
 */

#ifndef _LINUX_VIRTIO_DMA_BUF_H
#define	_LINUX_VIRTIO_DMA_BUF_H

#include <linux/dma-buf.h>
#include <linux/err.h>
#include <linux/uuid.h>

struct virtio_dma_buf_ops {
	struct dma_buf_ops ops;
	int (*device_attach)(struct dma_buf *dma_buf,
	    struct dma_buf_attachment *attach);
	int (*get_uuid)(struct dma_buf *dma_buf, uuid_t *uuid);
};

static inline struct dma_buf *
virtio_dma_buf_export(const struct dma_buf_export_info *exp_info)
{
	const struct virtio_dma_buf_ops *virtio_ops =
	    container_of(exp_info->ops, const struct virtio_dma_buf_ops, ops);

	if (exp_info->ops == NULL || exp_info->ops->attach == NULL ||
	    virtio_ops->get_uuid == NULL)
		return (ERR_PTR(-EINVAL));

	return (dma_buf_export(exp_info));
}

static inline int
virtio_dma_buf_attach(struct dma_buf *dma_buf,
    struct dma_buf_attachment *attach)
{
	const struct virtio_dma_buf_ops *ops =
	    container_of(dma_buf->ops, const struct virtio_dma_buf_ops, ops);
	int ret;

	if (ops->device_attach != NULL) {
		ret = ops->device_attach(dma_buf, attach);
		if (ret != 0)
			return (ret);
	}
	return (0);
}

#endif /* _LINUX_VIRTIO_DMA_BUF_H */
