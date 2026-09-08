/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Minimal Linux virtio API on native virtio(4)/virtqueue(9), covering
 * exactly the surface drivers/gpu/drm/virtio/ uses.  Module-local
 * because base LinuxKPI has no virtio support.
 */

#ifndef _LINUX_VIRTIO_H
#define _LINUX_VIRTIO_H

#include <sys/types.h>
#include <sys/bus.h>

#include <linux/types.h>
#include <linux/device.h>
#include <linux/gfp.h>
#include <linux/scatterlist.h>
#include <linux/uuid.h>		/* uuid_t for virtgpu_drv.h (shim shadow) */

/* hw.virtio_gpu sysctl node (defined in virtgpu_drv.c's FreeBSD tail) */
SYSCTL_DECL(_hw_virtio_gpu);

struct fbsd_vq;
struct sglist;
struct virtio_device;

/* Feature bit numbers, Linux-style (FreeBSD headers use masks). */
#define	VIRTIO_RING_F_INDIRECT_DESC	28
#define	VIRTIO_F_VERSION_1		32
#define	VIRTIO_F_ACCESS_PLATFORM	33

/* Teardown is owned by newbus; del_vqs is a no-op. */
struct virtio_config_ops {
	void	(*del_vqs)(struct virtio_device *vdev);
};

struct virtio_device {
	struct device dev;		/* generic device; &vdev->dev users */
	device_t bsddev;		/* FreeBSD virtio-bus child */
	const struct virtio_config_ops *config;
	u64 features;			/* negotiated feature bits */
	void *priv;
	int index;
	/* shim bookkeeping: the GPU's queues, filled by find_vqs */
	struct virtqueue *vqs[2];
};

/* Dead path: virtio_get_shm_region() always reports absence. */
#define	devm_request_mem_region(dev, start, n, name)	((void *)0)

struct virtqueue {
	struct virtio_device *vdev;
	void (*callback)(struct virtqueue *vq);
	unsigned int index;
	unsigned int num_free;
	struct fbsd_vq *fbsd_vq;	/* backing FreeBSD virtqueue */
	uint64_t reaps;			/* diagnostic; qlock-serialized */
	/*
	 * Preallocated add_sgs flatten buffer, serialized by the queue's
	 * qlock; a per-call allocation could fail, which the Linux API
	 * cannot and upstream does not handle.
	 */
	struct sglist *fsg;
};

typedef void vq_callback_t(struct virtqueue *);

/* Linux 6.11+ virtio_find_vqs() takes per-queue info structs. */
struct virtqueue_info {
	const char *name;
	vq_callback_t *callback;
	bool ctx;
};

struct irq_affinity;	/* opaque; always NULL in this driver */

int	virtio_find_vqs(struct virtio_device *vdev, unsigned int nvqs,
	    struct virtqueue *vqs[], struct virtqueue_info vqs_info[],
	    struct irq_affinity *desc);

/*
 * Registration surface so virtgpu_drv.c compiles unmodified;
 * module_virtio_driver() expands to nothing and the newbus glue
 * reaches the callbacks through the struct.
 */
#define	VIRTIO_DEV_ANY_ID	0xffffffff

struct virtio_device_id {
	uint32_t device;
	uint32_t vendor;
};

struct virtio_driver {
	const unsigned int *feature_table;
	unsigned int feature_table_size;
	struct {
		const char *name;
	} driver;
	const struct virtio_device_id *id_table;
	int	(*probe)(struct virtio_device *vdev);
	void	(*remove)(struct virtio_device *vdev);
	void	(*config_changed)(struct virtio_device *vdev);
};

#define	module_virtio_driver(drv)

/* LinuxKPI's MODULE_DEVICE_TABLE() needs a per-bus suffix macro. */
#define	MODULE_DEVICE_TABLE_BUS_virtio(_bus, _table)

#ifndef	pci_is_vga
/* virtio-gpu-pci is class DISPLAY_OTHER, never VGA. */
#define	pci_is_vga(pdev)	false
#endif

int	virtqueue_add_sgs(struct virtqueue *vq, struct scatterlist *sgs[],
	    unsigned int out_sgs, unsigned int in_sgs, void *data, gfp_t gfp);
void	*virtqueue_get_buf(struct virtqueue *vq, unsigned int *len);
bool	virtqueue_kick_prepare(struct virtqueue *vq);
bool	virtqueue_notify(struct virtqueue *vq);
void	virtqueue_disable_cb(struct virtqueue *vq);
bool	virtqueue_enable_cb(struct virtqueue *vq);

void	virtio_device_ready(struct virtio_device *vdev);
void	virtio_reset_device(struct virtio_device *vdev);

/* No cross-device virtio dma-buf sharing on this shim. */
static inline bool
is_virtio_device(struct device *dev)
{
	return (false);
}

static inline bool
virtio_has_feature(const struct virtio_device *vdev, unsigned int fbit)
{
	return ((vdev->features & (1ULL << fbit)) != 0);
}

/*
 * The "DMA quirk" is present when ACCESS_PLATFORM was not negotiated:
 * the device addresses guest memory physically, bypassing the DMA API.
 */
static inline bool
virtio_has_dma_quirk(const struct virtio_device *vdev)
{
	return (!(vdev->features & (1ULL << VIRTIO_F_ACCESS_PLATFORM)));
}

#endif /* _LINUX_VIRTIO_H */
