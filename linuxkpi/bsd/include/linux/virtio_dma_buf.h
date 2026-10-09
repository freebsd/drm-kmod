/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Linux's virtio dma-buf helpers (drivers/virtio/virtio_dma_buf.c).
 */

#ifndef _BSD_LKPI_LINUX_VIRTIO_DMA_BUF_H_
#define	_BSD_LKPI_LINUX_VIRTIO_DMA_BUF_H_

#include <linux/dma-buf.h>
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
	return (dma_buf_export(exp_info));
}

static inline int
virtio_dma_buf_attach(struct dma_buf *dma_buf,
    struct dma_buf_attachment *attach)
{
	const struct virtio_dma_buf_ops *ops =
	    container_of(dma_buf->ops, const struct virtio_dma_buf_ops, ops);

	if (ops->device_attach != NULL)
		return (ops->device_attach(dma_buf, attach));
	return (0);
}

#endif /* _BSD_LKPI_LINUX_VIRTIO_DMA_BUF_H_ */
