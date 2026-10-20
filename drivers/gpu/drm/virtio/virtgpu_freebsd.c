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

#include <sys/param.h>
#include <sys/module.h>
#include <sys/sysctl.h>

SYSCTL_DECL(_hw_virtio);

/* Node for the driver's LinuxKPI module parameters. */
SYSCTL_NODE(_hw_virtio, OID_AUTO, gpu, CTLFLAG_RW | CTLFLAG_MPSAFE, 0,
    "VirtIO GPU DRM parameters");

MODULE_VERSION(virtio_gpu_drm, 1);
MODULE_DEPEND(virtio_gpu_drm, linuxkpi, 1, 1, 1);
MODULE_DEPEND(virtio_gpu_drm, linuxkpi_virtio, 1, 1, 1);
MODULE_DEPEND(virtio_gpu_drm, virtio, 1, 1, 1);
MODULE_DEPEND(virtio_gpu_drm, linuxkpi_video, 1, 1, 1);
MODULE_DEPEND(virtio_gpu_drm, drmn, 2, 2, 2);
MODULE_DEPEND(virtio_gpu_drm, dmabuf, 1, 1, 1);
