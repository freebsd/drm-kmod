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
 * LinuxKPI virtio: binds Linux virtio_driver's to FreeBSD virtio_pci(4)
 * children (as linux_pci.c does for pci_driver's) and implements the
 * Linux virtqueue API over virtqueue(9).
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/sglist.h>

#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/pci.h>
#include <linux/virtio.h>
#include <linux/virtio_config.h>

/* Linux names that collide with dev/virtio. */
#undef virtqueue
#undef virtqueue_notify
#undef VIRTIO_F_VERSION_1

#include <dev/virtio/virtio.h>
#include <dev/virtio/virtqueue.h>

#include "virtio_if.h"

static MALLOC_DEFINE(M_LKPI_VIRTIO, "lkpivirtio", "LinuxKPI virtio");

CTASSERT(LKPI_VIRTQUEUE_MAX_SEGS == VIRTIO_MAX_INDIRECT);

/*
 * As sglist_append_phys(), but merging only when asked: coalescing must
 * not cross the readable/writable boundary.  Drivers place small
 * responses right after the command, and a merged segment would be
 * readable-only, leaving the device nowhere to write the response.
 */
static int
lkpi_append_sg(struct sglist *fsg, struct scatterlist *sg, bool merge)
{
	struct sglist_seg *ss;
	vm_paddr_t paddr = sg_phys(sg);

	if (merge && fsg->sg_nseg > 0) {
		ss = &fsg->sg_segs[fsg->sg_nseg - 1];
		if (paddr == ss->ss_paddr + ss->ss_len) {
			ss->ss_len += sg->length;
			return (0);
		}
	}
	if (fsg->sg_nseg == fsg->sg_maxseg)
		return (EFBIG);
	ss = &fsg->sg_segs[fsg->sg_nseg++];
	ss->ss_paddr = paddr;
	ss->ss_len = sg->length;
	return (0);
}

int
lkpi_virtqueue_add_sgs(struct linux_virtqueue *vq,
    struct scatterlist *sgs[], unsigned int out_sgs, unsigned int in_sgs,
    void *data, gfp_t gfp)
{
	struct sglist *fsg = vq->bsd_sg;	/* caller's queue lock */
	struct scatterlist *sg;
	unsigned int i;
	int readable, error;
	bool merge;

	sglist_reset(fsg);
	error = 0;
	for (i = 0; i < out_sgs && error == 0; i++)
		for (sg = sgs[i]; sg != NULL && error == 0; sg = sg_next(sg))
			error = lkpi_append_sg(fsg, sg, true);
	readable = fsg->sg_nseg;
	merge = false;
	for (; i < out_sgs + in_sgs && error == 0; i++)
		for (sg = sgs[i]; sg != NULL && error == 0; sg = sg_next(sg)) {
			error = lkpi_append_sg(fsg, sg, merge);
			merge = true;
		}
	if (error == 0)
		error = virtqueue_enqueue(vq->bsd_vq, data, fsg, readable,
		    fsg->sg_nseg - readable);
	if (error == EMSGSIZE)		/* Linux has only ENOSPC for "full" */
		error = ENOSPC;
	vq->num_free = virtqueue_nfree(vq->bsd_vq);
	return (-error);
}

void *
lkpi_virtqueue_get_buf(struct linux_virtqueue *vq, unsigned int *len)
{
	void *cookie;

	cookie = virtqueue_dequeue(vq->bsd_vq, len);
	vq->num_free = virtqueue_nfree(vq->bsd_vq);
	return (cookie);
}

/*
 * virtqueue(9) has no locking of its own, so notify here, under the
 * caller's queue lock, rather than in virtqueue_notify(), which Linux
 * drivers call after dropping it.
 */
bool
lkpi_virtqueue_kick_prepare(struct linux_virtqueue *vq)
{
	virtqueue_notify(vq->bsd_vq);
	return (false);
}

void
lkpi_virtqueue_disable_cb(struct linux_virtqueue *vq)
{
	virtqueue_disable_intr(vq->bsd_vq);
}

bool
lkpi_virtqueue_enable_cb(struct linux_virtqueue *vq)
{
	/* Linux: false = more work pending; FreeBSD: non-zero. */
	return (virtqueue_enable_intr(vq->bsd_vq) == 0);
}

/*
 * One handler for all queues.  It takes the softc, so it can check
 * bsd_vqs_ready under the lock before touching queues that del_vqs() may
 * have freed, and calls every callback: queue state may only be read
 * under the driver's lock, and Linux callbacks cope with finding no work.
 */
static void
lkpi_virtio_intr(void *arg)
{
	struct virtio_device *vdev;
	struct linux_virtqueue *vq;
	unsigned int i;

	vdev = arg;
	if (linux_set_current_flags(curthread, M_NOWAIT) != 0)
		return;
	mtx_lock(&vdev->bsd_lock);
	for (i = 0; vdev->bsd_vqs_ready && i < vdev->bsd_nvqs; i++) {
		vq = &vdev->bsd_vqs[i];
		if (vq->callback != NULL)
			vq->callback(vq);
	}
	mtx_unlock(&vdev->bsd_lock);
}

void
lkpi_virtio_cread_bytes(struct virtio_device *vdev, unsigned int offset,
    void *buf, int len)
{
	virtio_read_device_config(vdev->dev.bsddev, offset, buf, len);
}

void
lkpi_virtio_cwrite_bytes(struct virtio_device *vdev, unsigned int offset,
    const void *buf, int len)
{
	virtio_write_device_config(vdev->dev.bsddev, offset, buf, len);
}

void
lkpi_virtio_device_ready(struct virtio_device *vdev)
{
	virtio_reinit_complete(vdev->dev.bsddev);
}

/*
 * No queue callbacks after a reset or del_vqs(), and one already
 * running completes first.
 */
static void
lkpi_virtio_stop_callbacks(struct virtio_device *vdev)
{
	mtx_lock(&vdev->bsd_lock);
	vdev->bsd_vqs_ready = false;
	mtx_unlock(&vdev->bsd_lock);
}

void
lkpi_virtio_reset_device(struct virtio_device *vdev)
{
	lkpi_virtio_stop_callbacks(vdev);
	virtio_stop(vdev->dev.bsddev);
}

static void
lkpi_virtio_del_vqs(struct virtio_device *vdev)
{
	unsigned int i;

	lkpi_virtio_stop_callbacks(vdev);
	/* The virtqueue(9) side is freed when the newbus child detaches. */
	for (i = 0; i < vdev->bsd_nvqs; i++)
		sglist_free(vdev->bsd_vqs[i].bsd_sg);
	free(vdev->bsd_vqs, M_LKPI_VIRTIO);
	vdev->bsd_vqs = NULL;
	vdev->bsd_nvqs = 0;
}

static const struct virtio_config_ops lkpi_virtio_config_ops = {
	.del_vqs = lkpi_virtio_del_vqs,
};

int
lkpi_virtio_find_vqs(struct virtio_device *vdev, unsigned int nvqs,
    struct linux_virtqueue *vqs[], struct virtqueue_info vqs_info[],
    struct irq_affinity *desc)
{
	device_t dev = vdev->dev.bsddev;
	struct vq_alloc_info *info;
	struct linux_virtqueue *vq;
	unsigned int i;
	int error;

	vdev->bsd_vqs = malloc(nvqs * sizeof(*vdev->bsd_vqs), M_LKPI_VIRTIO,
	    M_WAITOK | M_ZERO);
	info = malloc(nvqs * sizeof(*info), M_LKPI_VIRTIO, M_WAITOK);
	vdev->bsd_nvqs = nvqs;

	for (i = 0; i < nvqs; i++) {
		vq = &vdev->bsd_vqs[i];
		vq->vdev = vdev;
		vq->callback = vqs_info[i].callback;
		VQ_ALLOC_INFO_INIT(&info[i], VIRTIO_MAX_INDIRECT,
		    lkpi_virtio_intr, vdev, &vq->bsd_vq, "%s %s",
		    device_get_nameunit(dev), vqs_info[i].name);
	}

	error = virtio_alloc_virtqueues(dev, nvqs, info);
	free(info, M_LKPI_VIRTIO);
	if (error != 0)
		return (-error);
	error = virtio_setup_intr(dev, INTR_TYPE_TTY);
	if (error != 0)
		return (-error);

	/* Before any queue interrupt is enabled. */
	mtx_lock(&vdev->bsd_lock);
	vdev->bsd_vqs_ready = true;
	mtx_unlock(&vdev->bsd_lock);
	for (i = 0; i < nvqs; i++) {
		vq = &vdev->bsd_vqs[i];
		vq->num_free = virtqueue_nfree(vq->bsd_vq);
		/* An indirect chain may exceed the ring size. */
		vq->bsd_sg = sglist_alloc(MAX(vq->num_free,
		    VIRTIO_MAX_INDIRECT), M_WAITOK);
		/*
		 * FreeBSD virtqueues start with interrupts disabled and
		 * drivers arm them explicitly; Linux vrings start enabled
		 * and Linux drivers rely on that.
		 */
		virtqueue_enable_intr(vq->bsd_vq);
		vqs[i] = vq;
	}
	return (0);
}

static struct virtio_driver *
lkpi_virtio_driver(device_t dev)
{
	return (container_of(device_get_driver(dev), struct virtio_driver,
	    bsddriver));
}

static int
lkpi_virtio_probe(device_t dev)
{
	struct virtio_driver *drv;
	const struct virtio_device_id *id;
	uint32_t type, vendor;

	drv = lkpi_virtio_driver(dev);
	type = virtio_get_device_type(dev);
	vendor = virtio_get_subvendor(dev);
	/* As Linux's virtio_id_match(). */
	for (id = drv->id_table; id->device != 0; id++) {
		if ((id->device == type || id->device == VIRTIO_DEV_ANY_ID) &&
		    (id->vendor == vendor || id->vendor == VIRTIO_DEV_ANY_ID)) {
			device_set_desc(dev, drv->driver.name);
			/* Outrank base drivers for the same device type. */
			return (BUS_PROBE_VENDOR);
		}
	}
	return (ENXIO);
}

/* Undoes attach; on detach it runs after the driver's remove(). */
static void
lkpi_virtio_release(struct virtio_device *vdev)
{
	/* Stops the device's interrupts and clears its status. */
	lkpi_virtio_reset_device(vdev);
	lkpi_virtio_del_vqs(vdev);
	if (vdev->dev.parent != NULL)
		pci_dev_put(to_pci_dev(vdev->dev.parent));
	mtx_destroy(&vdev->bsd_lock);
}

static int
lkpi_virtio_attach(device_t dev)
{
	struct virtio_device *vdev = device_get_softc(dev);
	struct virtio_driver *drv = lkpi_virtio_driver(dev);
	struct pci_dev *pdev;
	uint64_t mask;
	unsigned int i;
	int error;

	linux_set_current(curthread);

	vdev->dev.bsddev = dev;
	mtx_init(&vdev->bsd_lock, device_get_nameunit(dev), "lkpi virtio",
	    MTX_DEF);
	/*
	 * virtio_reinit() runs the whole status sequence, reset included:
	 * devctl(8) attach and enable bypass vtpci's probe-and-attach
	 * wrapper, which otherwise sets the initial status.
	 *
	 * Linux always negotiates indirect descriptors and drivers size
	 * their ring-space waits on that: without them a request with more
	 * segments than ring slots can never be queued.
	 */
	mask = VIRTIO_RING_F_INDIRECT_DESC;
	for (i = 0; i < drv->feature_table_size; i++)
		mask |= 1ULL << drv->feature_table[i];
	error = virtio_reinit(dev, mask);
	if (error != 0) {
		device_printf(dev, "virtio_reinit failed: %d\n", error);
		goto fail;
	}

	/*
	 * drm_dev_alloc() and friends need a real parent struct device.  It
	 * has no DMA tag: VIRTIO_F_ACCESS_PLATFORM is never requested, so
	 * drivers hand the device physical addresses.
	 */
	pdev = lkpinew_pci_dev(device_get_parent(dev));
	if (pdev == NULL) {
		error = ENOMEM;
		goto fail;
	}
	vdev->dev.parent = &pdev->dev;
	vdev->config = &lkpi_virtio_config_ops;
	for (i = 0; i < 64; i++)
		if (virtio_with_feature(dev, 1ULL << i))
			vdev->features |= 1ULL << i;

	error = drv->probe(vdev);
	if (error != 0) {
		error = -error;
		goto fail;
	}
	return (0);

fail:
	lkpi_virtio_release(vdev);
	return (error);
}

static int
lkpi_virtio_detach(device_t dev)
{
	struct virtio_device *vdev = device_get_softc(dev);

	linux_set_current(curthread);

	/* Already removed if an earlier detach returned EBUSY below. */
	if (!vdev->bsd_detaching) {
		/*
		 * As Linux, no config callbacks once remove() starts: the
		 * device stays attached until we return.
		 */
		mtx_lock(&vdev->bsd_lock);
		vdev->bsd_detaching = true;
		mtx_unlock(&vdev->bsd_lock);
		lkpi_virtio_driver(dev)->remove(vdev);
	}
	/*
	 * As linux_pci: refuse while others, such as a DRM device with open
	 * files, still hold the parent, whose bsddev is the transport.
	 */
	if (kref_read(&vdev->dev.parent->kobj.kref) > 1) {
		device_printf(dev, "%s failed due to %u other pending "
		    "references on the parent device.\n", __func__,
		    kref_read(&vdev->dev.parent->kobj.kref) - 1);
		return (EBUSY);
	}
	lkpi_virtio_release(vdev);
	return (0);
}

static int
lkpi_virtio_config_change(device_t dev)
{
	struct virtio_device *vdev = device_get_softc(dev);
	struct virtio_driver *drv = lkpi_virtio_driver(dev);

	if (linux_set_current_flags(curthread, M_NOWAIT) != 0)
		return (0);
	/*
	 * Delivered between the end of probe and the start of detach.
	 * Unlike Linux, a change that arrives during probe is not replayed
	 * afterwards.
	 */
	mtx_lock(&vdev->bsd_lock);
	if (device_is_attached(dev) && !vdev->bsd_detaching &&
	    drv->config_changed != NULL)
		drv->config_changed(vdev);
	mtx_unlock(&vdev->bsd_lock);
	return (0);
}

static device_method_t lkpi_virtio_methods[] = {
	DEVMETHOD(device_probe,		lkpi_virtio_probe),
	DEVMETHOD(device_attach,	lkpi_virtio_attach),
	DEVMETHOD(device_detach,	lkpi_virtio_detach),

	DEVMETHOD(virtio_config_change,	lkpi_virtio_config_change),

	DEVMETHOD_END
};

int
lkpi_register_virtio_driver(struct virtio_driver *drv)
{
	int error;

	drv->bsddriver.name = drv->driver.name;
	drv->bsddriver.methods = lkpi_virtio_methods;
	drv->bsddriver.size = sizeof(struct virtio_device);
	bus_topo_lock();
	error = devclass_add_driver(devclass_create("virtio_pci"),
	    &drv->bsddriver, BUS_PASS_DEFAULT, NULL);
	bus_topo_unlock();
	return (-error);
}

void
lkpi_unregister_virtio_driver(struct virtio_driver *drv)
{
	bus_topo_lock();
	(void)devclass_delete_driver(devclass_find("virtio_pci"),
	    &drv->bsddriver);
	bus_topo_unlock();
}

MODULE_VERSION(linuxkpi_virtio, 1);
MODULE_DEPEND(linuxkpi_virtio, linuxkpi, 1, 1, 1);
MODULE_DEPEND(linuxkpi_virtio, virtio, 1, 1, 1);
