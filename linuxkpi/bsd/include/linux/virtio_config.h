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

#ifndef _BSD_LKPI_LINUX_VIRTIO_CONFIG_H_
#define	_BSD_LKPI_LINUX_VIRTIO_CONFIG_H_

#include <linux/virtio.h>

/* Feature bit numbers, Linux-style (dev/virtio uses masks). */
#define	VIRTIO_F_VERSION_1		32
#define	VIRTIO_F_ACCESS_PLATFORM	33

struct irq_affinity;

struct virtio_shm_region {
	uint64_t addr;
	uint64_t len;
};

typedef void vq_callback_t(struct linux_virtqueue *);

struct virtqueue_info {
	const char *name;
	vq_callback_t *callback;
};

struct virtio_config_ops {
	void	(*del_vqs)(struct virtio_device *vdev);
};

static inline bool
virtio_has_feature(const struct virtio_device *vdev, unsigned int fbit)
{
	return ((vdev->features & (1ULL << fbit)) != 0);
}

static inline bool
virtio_has_dma_quirk(const struct virtio_device *vdev)
{
	return (!virtio_has_feature(vdev, VIRTIO_F_ACCESS_PLATFORM));
}

int	lkpi_virtio_find_vqs(struct virtio_device *vdev, unsigned int nvqs,
	    struct linux_virtqueue *vqs[], struct virtqueue_info vqs_info[],
	    struct irq_affinity *desc);
void	lkpi_virtio_device_ready(struct virtio_device *vdev);
#define	virtio_find_vqs(...)		lkpi_virtio_find_vqs(__VA_ARGS__)
#define	virtio_device_ready(vdev)	lkpi_virtio_device_ready(vdev)

/* Shared memory regions (VIRTIO_PCI_CAP_SHARED_MEMORY_CFG) unsupported. */
static inline bool
virtio_get_shm_region(struct virtio_device *vdev,
    struct virtio_shm_region *region, uint8_t id)
{
	return (false);
}

void	lkpi_virtio_cread_bytes(struct virtio_device *vdev,
	    unsigned int offset, void *buf, int len);
void	lkpi_virtio_cwrite_bytes(struct virtio_device *vdev,
	    unsigned int offset, const void *buf, int len);

/* virtio_read_device_config() already returns host byte order. */
#define	virtio_cread_le(vdev, structname, member, ptr)			\
	lkpi_virtio_cread_bytes((vdev), offsetof(structname, member),	\
	    (ptr), sizeof(*(ptr)))
#define	virtio_cwrite_le(vdev, structname, member, ptr)			\
	lkpi_virtio_cwrite_bytes((vdev), offsetof(structname, member),	\
	    (ptr), sizeof(*(ptr)))

#endif /* _BSD_LKPI_LINUX_VIRTIO_CONFIG_H_ */
