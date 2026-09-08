/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * Linux virtio API shim on FreeBSD virtqueue(9).  This TU must never
 * include <dev/virtio/virtqueue.h> (struct virtqueue tag clash);
 * native operations go through the fbsd_* wrappers in
 * freebsd/virtgpu_freebsd.h.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/sglist.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/scatterlist.h>
#include <linux/pci.h>
#include <linux/virtio.h>
#include <linux/virtio_config.h>

#include "freebsd/virtgpu_freebsd.h"

/* from virtgpu_drv.c's FreeBSD tail */
const struct virtio_driver *virtio_gpu_freebsd_driver(void);

_Static_assert(nitems(((struct virtio_device *)0)->vqs) == VIRTGPU_NVQS,
    "virtio_device.vqs[] size != VIRTGPU_NVQS");

/*
 * Coalescing must not cross the readable/writable boundary: upstream
 * places small responses inline right after the command, and a merged
 * segment would be classified readable-only, leaving the device
 * nowhere to write the response.
 */
static int
shim_append_sg(struct sglist *fsg, struct scatterlist *sg, bool merge)
{
	struct sglist_seg *ss;
	vm_paddr_t paddr = sg_phys(sg);

	if (paddr == 0 || sg->length == 0) {
		printf("virtio_gpu shim: bad segment phys %#jx len %u\n",
		    (uintmax_t)paddr, sg->length);
		return (EINVAL);
	}
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

/*
 * Flatten the Linux sg chains into the queue's sglist.
 * virtqueue_enqueue() requires readable + writable == sg_nseg, so the
 * readable count is read off the sglist at the direction boundary.
 */
int
virtqueue_add_sgs(struct virtqueue *vq, struct scatterlist *sgs[],
    unsigned int out_sgs, unsigned int in_sgs, void *data, gfp_t gfp)
{
	struct sglist *fsg = vq->fsg;	/* qlock-serialized, see virtio.h */
	struct scatterlist *sg;
	unsigned int i;
	int readable, error;
	bool merge;

	sglist_reset(fsg);
	error = 0;
	for (i = 0; i < out_sgs && error == 0; i++)
		for (sg = sgs[i]; sg != NULL && error == 0; sg = sg_next(sg))
			error = shim_append_sg(fsg, sg, true);
	readable = fsg->sg_nseg;
	merge = false;			/* no merge across the boundary */
	for (; i < out_sgs + in_sgs && error == 0; i++)
		for (sg = sgs[i]; sg != NULL && error == 0; sg = sg_next(sg)) {
			error = shim_append_sg(fsg, sg, merge);
			merge = true;
		}
	if (error == 0)
		error = fbsd_vq_enqueue(vq->fbsd_vq, data, fsg, readable,
		    fsg->sg_nseg - readable);
	if (error == EMSGSIZE)		/* Linux has only ENOSPC for "full" */
		error = ENOSPC;
	vq->num_free = fbsd_vq_nfree(vq->fbsd_vq);
	return (error != 0 ? -error : 0);	/* -ENOSPC etc., as Linux */
}

uint64_t
lkpi_virtio_reap_count(void *vdevp)
{
	struct virtio_device *vdev = vdevp;
	uint64_t n = 0;
	int i;

	for (i = 0; i < VIRTGPU_NVQS; i++)
		if (vdev->vqs[i] != NULL)
			n += vdev->vqs[i]->reaps;
	return (n);
}

void *
virtqueue_get_buf(struct virtqueue *vq, unsigned int *len)
{
	void *cookie;

	cookie = fbsd_vq_dequeue(vq->fbsd_vq, len);
	if (cookie != NULL)
		vq->reaps++;
	vq->num_free = fbsd_vq_nfree(vq->fbsd_vq);
	return (cookie);
}

bool
virtqueue_kick_prepare(struct virtqueue *vq)
{
	/* Always notify; suppression is only a performance hint. */
	return (true);
}

bool
virtqueue_notify(struct virtqueue *vq)
{
	fbsd_gpu_notify(vq->vdev->bsddev, vq->index);
	return (true);
}

void
virtqueue_disable_cb(struct virtqueue *vq)
{
	fbsd_vq_disable_intr(vq->fbsd_vq);
}

bool
virtqueue_enable_cb(struct virtqueue *vq)
{
	/* Linux: false = more work pending; FreeBSD: non-zero. */
	return (fbsd_vq_enable_intr(vq->fbsd_vq) == 0);
}

void
virtio_device_ready(struct virtio_device *vdev)
{
	fbsd_gpu_device_ready(vdev->bsddev);
}

void
virtio_reset_device(struct virtio_device *vdev)
{
	/* Teardown is owned by the newbus detach path (virtio_stop). */
}

void
lkpi_virtio_cread_bytes(struct virtio_device *vdev, unsigned int offset,
    void *buf, int len)
{
	fbsd_gpu_read_config(vdev->bsddev, offset, buf, len);
}

void
lkpi_virtio_cwrite_bytes(struct virtio_device *vdev, unsigned int offset,
    const void *buf, int len)
{
	fbsd_gpu_write_config(vdev->bsddev, offset,
	    __DECONST(void *, buf), len);
}

/*
 * Interrupt trampoline.  No num_free refresh here: all writers hold
 * the queue lock, and the unlocked wait_event() re-checks only run
 * after a wakeup that follows a locked refresh — an unlocked write
 * could only publish a stale value.
 */
void
lkpi_virtio_vq_intr(void *arg)
{
	struct virtqueue *vq = arg;

	if (vq->callback != NULL)
		vq->callback(vq);
}

static void
lkpi_virtio_del_vqs(struct virtio_device *vdev __unused)
{
	/* Freed by the virtio bus when the newbus child detaches. */
}

static const struct virtio_config_ops lkpi_virtio_config_ops = {
	.del_vqs = lkpi_virtio_del_vqs,
};

void *
lkpi_virtio_vdev_create(device_t dev, device_t pcidev, uint64_t features)
{
	struct virtio_device *vdev;
	struct pci_dev *pdev;

	vdev = kzalloc(sizeof(*vdev), GFP_KERNEL);
	if (vdev == NULL)
		return (NULL);
	vdev->config = &lkpi_virtio_config_ops;
	/* drm_dev_alloc() needs a real parent struct device. */
	pdev = lkpinew_pci_dev(pcidev);
	if (pdev == NULL) {
		kfree(vdev);
		return (NULL);
	}
	vdev->dev.parent = &pdev->dev;
	vdev->bsddev = dev;
	vdev->features = features;
	return (vdev);
}

void
lkpi_virtio_vdev_destroy(void *vdevp)
{
	struct virtio_device *vdev = vdevp;
	int i;

	if (vdev == NULL)
		return;
	for (i = 0; i < VIRTGPU_NVQS; i++) {
		if (vdev->vqs[i] != NULL && vdev->vqs[i]->fsg != NULL)
			sglist_free(vdev->vqs[i]->fsg);
		kfree(vdev->vqs[i]);
	}
	/* balance lkpinew_pci_dev()'s creation reference */
	pci_dev_put(to_pci_dev(vdev->dev.parent));
	kfree(vdev);
}

/* Linux 6.11+ virtio_find_vqs() signature. */
int
virtio_find_vqs(struct virtio_device *vdev, unsigned int nvqs,
    struct virtqueue *vqs[], struct virtqueue_info vqs_info[],
    struct irq_affinity *desc __unused)
{
	const char *names[VIRTGPU_NVQS];
	struct fbsd_vq *fvqs[VIRTGPU_NVQS];
	void *args[VIRTGPU_NVQS];
	struct virtqueue *vq;
	unsigned int i;
	int error;

	if (nvqs > VIRTGPU_NVQS)
		return (-EINVAL);

	for (i = 0; i < nvqs; i++) {
		vq = kzalloc(sizeof(*vq), GFP_KERNEL);
		if (vq == NULL)
			return (-ENOMEM);
		vq->vdev = vdev;
		vq->index = i;
		vq->callback = vqs_info[i].callback;
		vdev->vqs[i] = vq;
		args[i] = vq;
		names[i] = vqs_info[i].name;
	}

	error = fbsd_gpu_alloc_vqs(vdev->bsddev, nvqs, args, names, fvqs);
	if (error != 0)
		return (-error);

	for (i = 0; i < nvqs; i++) {
		vdev->vqs[i]->fbsd_vq = fvqs[i];
		vdev->vqs[i]->num_free = fbsd_vq_nfree(fvqs[i]);
		/* An indirect chain may exceed the ring size. */
		vdev->vqs[i]->fsg = sglist_alloc(MAX(fbsd_vq_nfree(fvqs[i]),
		    VIRTGPU_MAX_INDIRECT), M_WAITOK);
		vqs[i] = vdev->vqs[i];
	}
	return (0);
}

int
lkpi_virtio_gpu_attach(void *vdevp)
{
	return (virtio_gpu_freebsd_driver()->probe(vdevp));
}

void
lkpi_virtio_gpu_config_changed(void *vdevp)
{
	virtio_gpu_freebsd_driver()->config_changed(vdevp);
}
