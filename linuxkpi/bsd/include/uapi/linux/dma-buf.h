/*-
 * Copyright (c) 2023 Beckhoff Automation GmbH & Co. KG
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
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef _BSD_LKPI_UAPI_LINUX_DMA_BUF_H_
#define	_BSD_LKPI_UAPI_LINUX_DMA_BUF_H_

#include <linux/types.h>

struct dma_buf_sync {
	__u64 flags;
};

#define	DMA_BUF_SYNC_READ		(1 << 0)
#define	DMA_BUF_SYNC_WRITE		(2 << 0)
#define	DMA_BUF_SYNC_RW			(DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define	DMA_BUF_SYNC_START		(0 << 2)
#define	DMA_BUF_SYNC_END		(1 << 2)
#define	DMA_BUF_SYNC_VALID_FLAGS_MASK	(DMA_BUF_SYNC_RW | DMA_BUF_SYNC_END)

#define	DMA_BUF_IOCTL_SYNC		_IOW('b', 0, struct dma_buf_sync)

#endif	/* _BSD_LKPI_UAPI_LINUX_DMA_BUF_H_ */
