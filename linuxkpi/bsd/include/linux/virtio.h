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
 * Linux virtio driver API over FreeBSD virtio(4)/virtqueue(9).
 * Implemented in drivers/virtio/linuxkpi_virtio.c, which #undef's the
 * macros here that collide with dev/virtio.
 */

#ifndef _BSD_LKPI_LINUX_VIRTIO_H_
#define	_BSD_LKPI_LINUX_VIRTIO_H_

#include <sys/param.h>
#include <sys/bus.h>
#include <sys/lock.h>
#include <sys/module.h>
#include <sys/mutex.h>

#include <linux/types.h>
#include <linux/device.h>
#include <linux/device/driver.h>
#include <linux/err.h>
#include <linux/gfp.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

struct virtio_config_ops;

struct linux_virtqueue {
	void (*callback)(struct linux_virtqueue *vq);
	struct virtio_device *vdev;
	unsigned int num_free;
	/* FreeBSD */
	struct virtqueue *bsd_vq;	/* virtqueue(9): before the #define */
	struct sglist *bsd_sg;		/* segments for virtqueue_enqueue() */
};
#define	virtqueue	linux_virtqueue

int	lkpi_virtqueue_add_sgs(struct linux_virtqueue *vq,
	    struct scatterlist *sgs[], unsigned int out_sgs,
	    unsigned int in_sgs, void *data, gfp_t gfp);
bool	lkpi_virtqueue_kick_prepare(struct linux_virtqueue *vq);
void	*lkpi_virtqueue_get_buf(struct linux_virtqueue *vq, unsigned int *len);
void	lkpi_virtqueue_disable_cb(struct linux_virtqueue *vq);
bool	lkpi_virtqueue_enable_cb(struct linux_virtqueue *vq);
#define	virtqueue_add_sgs(...)		lkpi_virtqueue_add_sgs(__VA_ARGS__)
#define	virtqueue_kick_prepare(vq)	lkpi_virtqueue_kick_prepare(vq)
#define	virtqueue_notify(vq)		lkpi_virtqueue_notify(vq)
#define	virtqueue_get_buf(vq, len)	lkpi_virtqueue_get_buf(vq, len)
#define	virtqueue_disable_cb(vq)	lkpi_virtqueue_disable_cb(vq)
#define	virtqueue_enable_cb(vq)		lkpi_virtqueue_enable_cb(vq)

/* virtqueue_kick_prepare() already notified the device. */
static inline bool
lkpi_virtqueue_notify(struct linux_virtqueue *vq)
{
	return (true);
}

/*
 * virtqueue(9) indirect tables hold VIRTIO_MAX_INDIRECT descriptors,
 * where Linux allocates any size, so that is as many segments as a
 * request can count on.  Data a driver hands the device that could span
 * more pages goes in physically contiguous memory, which add_sgs
 * coalesces into one segment (LinuxKPI's kmalloc() uses contigmalloc(9)
 * above PAGE_SIZE).  Three segments stay for the command, the response,
 * and one more page when the data is not page-aligned.
 */
#define	LKPI_VIRTQUEUE_MAX_SEGS	(PAGE_SIZE / 16)

static inline void *
lkpi_virtqueue_kvmalloc(size_t size, gfp_t gfp)
{
	if (size > INT_MAX)		/* as Linux's kvmalloc() */
		return (NULL);
	/* M_WAITOK contigmalloc(9) can retry forever on NUMA: fail instead. */
	if (size > (LKPI_VIRTQUEUE_MAX_SEGS - 3) * PAGE_SIZE)
		return (kmalloc(size, (gfp & ~M_WAITOK) | M_NOWAIT));
	return (kvmalloc(size, gfp));
}

static inline void *
lkpi_virtqueue_memdup_user(const void __user *src, size_t len)
{
	void *p;

	p = lkpi_virtqueue_kvmalloc(len, GFP_KERNEL);
	if (p == NULL)
		return (ERR_PTR(-ENOMEM));
	if (copy_from_user(p, src, len) != 0) {
		kvfree(p);
		return (ERR_PTR(-EFAULT));
	}
	return (p);
}

struct virtio_device {
	struct device dev;
	const struct virtio_config_ops *config;
	uint64_t features;
	void *priv;
	/* FreeBSD */
	struct linux_virtqueue *bsd_vqs;
	unsigned int bsd_nvqs;
	struct mtx bsd_lock;		/* callbacks vs. reset and detach */
	bool bsd_vqs_ready;		/* queue callbacks allowed */
	bool bsd_detaching;		/* removed: no config callbacks */
};

void	lkpi_virtio_reset_device(struct virtio_device *vdev);
#define	virtio_reset_device(vdev)	lkpi_virtio_reset_device(vdev)

#define	VIRTIO_DEV_ANY_ID	0xffffffff
struct virtio_device_id {
	uint32_t device;
	uint32_t vendor;
};

struct virtio_driver {
	struct device_driver driver;
	const struct virtio_device_id *id_table;
	const unsigned int *feature_table;
	unsigned int feature_table_size;
	int	(*probe)(struct virtio_device *vdev);
	void	(*remove)(struct virtio_device *vdev);
	void	(*config_changed)(struct virtio_device *vdev);
	/* FreeBSD */
	driver_t bsddriver;
};

int	lkpi_register_virtio_driver(struct virtio_driver *drv);
void	lkpi_unregister_virtio_driver(struct virtio_driver *drv);
#define	register_virtio_driver(drv)	lkpi_register_virtio_driver(drv)
#define	unregister_virtio_driver(drv)	lkpi_unregister_virtio_driver(drv)
#define	module_virtio_driver(_drv)					\
    module_driver(_drv, register_virtio_driver, unregister_virtio_driver)

/* The subvendor is Linux's virtio vendor ID. */
#define	MODULE_DEVICE_TABLE_BUS_virtio(_bus, _table)			\
    MODULE_PNP_INFO("U32:device_type;V32:subvendor", virtio_pci,	\
    lkpi_ ## _table, _table, nitems(_table) - 1)

/* No cross-device virtio dma-buf sharing. */
static inline bool
is_virtio_device(struct device *dev)
{
	return (false);
}

#endif /* _BSD_LKPI_LINUX_VIRTIO_H_ */
