/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Interface between the native TU (virtgpu_freebsd.c) and the Linux
 * shim TU (linux_virtio.c).  Both kernels define struct virtqueue, so
 * no TU may see both: FreeBSD virtqueues cross as the opaque
 * struct fbsd_vq, Linux shim virtqueues as void *.
 */

#ifndef VIRTGPU_FREEBSD_H
#define VIRTGPU_FREEBSD_H

struct sglist;
struct fbsd_vq;		/* really: FreeBSD struct virtqueue */

#define	VIRTGPU_NVQS		2	/* controlq + cursorq, as on Linux */
/*
 * ATTACH_BACKING submits one segment per page of its ents buffer;
 * without indirect descriptors a large framebuffer demands more ring
 * slots than exist and the upstream ring-space precheck hangs.  256
 * is virtqueue(9)'s VIRTIO_MAX_INDIRECT.
 */
#define	VIRTGPU_MAX_INDIRECT	256

/*
 * Implemented by virtgpu_freebsd.c (native TU).
 */
int	 fbsd_gpu_alloc_vqs(device_t dev, int nvqs, void *lvqs[],
	    const char *names[], struct fbsd_vq *vqs[]);
void	 fbsd_gpu_device_ready(device_t dev);
void	 fbsd_gpu_read_config(device_t dev, unsigned int offset, void *dst,
	    int len);
void	 fbsd_gpu_write_config(device_t dev, unsigned int offset, void *src,
	    int len);
int	 fbsd_vq_enqueue(struct fbsd_vq *vq, void *cookie, struct sglist *sg,
	    int readable, int writable);
void	*fbsd_vq_dequeue(struct fbsd_vq *vq, unsigned int *len);
void	 fbsd_gpu_notify(device_t dev, unsigned int index);
int	 fbsd_vq_nfree(struct fbsd_vq *vq);
int	 fbsd_vq_enable_intr(struct fbsd_vq *vq);	/* !=0: work pending */
void	 fbsd_vq_disable_intr(struct fbsd_vq *vq);

/*
 * Implemented by linux_virtio.c (Linux-API TU).
 */
void	*lkpi_virtio_vdev_create(device_t dev, device_t pcidev,
	    uint64_t features);
void	 lkpi_virtio_vdev_destroy(void *vdev);
void	 lkpi_virtio_vq_intr(void *lvq);	/* vq interrupt trampoline */
uint64_t lkpi_virtio_reap_count(void *vdev);	/* diagnostic counter */
int	 lkpi_virtio_gpu_attach(void *vdev);	/* upstream .probe */
void	 lkpi_virtio_gpu_config_changed(void *vdev);

#endif /* VIRTGPU_FREEBSD_H */
