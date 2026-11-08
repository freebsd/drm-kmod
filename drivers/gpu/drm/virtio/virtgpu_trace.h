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

#ifndef _VIRTGPU_TRACE_FREEBSD_H_
#define	_VIRTGPU_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>

#include "virtgpu_drv.h"

/* DEFINE_EVENT(virtio_gpu_cmd, virtio_gpu_cmd_queue) */
static inline void
trace_virtio_gpu_cmd_queue(struct linux_virtqueue *vq,
    struct virtio_gpu_ctrl_hdr *hdr, uint32_t seqno)
{
	CTR6(KTR_DRM, "virtio_gpu_cmd_queue type %#x flags %#x fence_id %ju "
	    "ctx_id %u num_free %u seqno %u", le32_to_cpu(hdr->type),
	    le32_to_cpu(hdr->flags), (uintmax_t)le64_to_cpu(hdr->fence_id),
	    le32_to_cpu(hdr->ctx_id), vq->num_free, seqno);
}

/* DEFINE_EVENT(virtio_gpu_cmd, virtio_gpu_cmd_response) */
static inline void
trace_virtio_gpu_cmd_response(struct linux_virtqueue *vq,
    struct virtio_gpu_ctrl_hdr *hdr, uint32_t seqno)
{
	CTR6(KTR_DRM, "virtio_gpu_cmd_response type %#x flags %#x fence_id %ju "
	    "ctx_id %u num_free %u seqno %u", le32_to_cpu(hdr->type),
	    le32_to_cpu(hdr->flags), (uintmax_t)le64_to_cpu(hdr->fence_id),
	    le32_to_cpu(hdr->ctx_id), vq->num_free, seqno);
}

#endif /* _VIRTGPU_TRACE_FREEBSD_H_ */
