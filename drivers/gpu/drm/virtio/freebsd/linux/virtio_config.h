/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Linux virtio config-space accessors on FreeBSD virtio(4).
 */

#ifndef _LINUX_VIRTIO_CONFIG_H
#define _LINUX_VIRTIO_CONFIG_H

#include <linux/virtio.h>

void	lkpi_virtio_cread_bytes(struct virtio_device *vdev,
	    unsigned int offset, void *buf, int len);
void	lkpi_virtio_cwrite_bytes(struct virtio_device *vdev,
	    unsigned int offset, const void *buf, int len);

/*
 * Virtio-modern config space is little-endian; FreeBSD aarch64/amd64 run
 * little-endian, so byte copies suffice.  Big-endian arches would need
 * swaps here — out of scope (aarch64 first, amd64 later).
 */
#define	virtio_cread_le(vdev, structname, member, ptr)			\
	lkpi_virtio_cread_bytes((vdev), offsetof(structname, member),	\
	    (ptr), sizeof(*(ptr)))
#define	virtio_cwrite_le(vdev, structname, member, ptr)			\
	lkpi_virtio_cwrite_bytes((vdev), offsetof(structname, member),	\
	    (ptr), sizeof(*(ptr)))

/*
 * Shared-memory regions (VIRTIO_GPU_F_RESOURCE_BLOB / host-visible):
 * rejected by this 2D-only port, so the probe just reports absence.
 */
struct virtio_shm_region {
	u64 addr;
	u64 len;
};

static inline bool
virtio_get_shm_region(struct virtio_device *vdev,
    struct virtio_shm_region *region, u8 id)
{
	return (false);
}

#endif /* _LINUX_VIRTIO_CONFIG_H */
