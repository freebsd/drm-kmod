/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Denis Borovikov <denis.borovikov@gmail.com>
 *
 * FreeBSD attach glue for the virtio-gpu DRM driver: a native newbus
 * virtio child driver replacing the Linux virtio-bus registration.
 * This TU must not include the shim linux/virtio.h (struct virtqueue
 * tag clash); see freebsd/virtgpu_freebsd.h for the boundary contract.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sglist.h>
#include <sys/sysctl.h>

#include <vm/vm.h>
#include <vm/pmap.h>

#include <machine/bus.h>

#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>

#include <dev/virtio/virtio.h>
#include <dev/virtio/virtio_config.h>
#include <dev/virtio/virtio_ids.h>
#include <dev/virtio/virtqueue.h>
#include <dev/virtio/gpu/virtio_gpu.h>

#include "virtio_if.h"
#include "virtio_pci_if.h"

#include "freebsd/virtgpu_freebsd.h"

/* EDID, classic virgl 3D, and indirect descriptors (VIRTGPU_MAX_INDIRECT). */
#define	VGD_FEATURES	((1ULL << VIRTIO_GPU_F_EDID) | \
			    (1ULL << VIRTIO_GPU_F_VIRGL) | \
			    VIRTIO_RING_F_INDIRECT_DESC)

/* gates MOD_QUIESCE */
static u_int vgd_ndevs;

struct virtio_gpu_drm_softc {
	device_t	 vgd_dev;
	uint64_t	 vgd_features;
	void		*vgd_vdev;		/* Linux shim virtio_device */
	struct fbsd_vq	*vgd_vqs[VIRTGPU_NVQS];	/* [0]=control [1]=cursor */
	void		*vgd_notify_va;		/* own notify-window mapping */
	bus_size_t	 vgd_notify_off[VIRTGPU_NVQS];
	int		 vgd_notify_ok;
	int		 vgd_attached;		/* upstream probe succeeded */
};

int
fbsd_vq_enqueue(struct fbsd_vq *vq, void *cookie, struct sglist *sg,
    int readable, int writable)
{
	return (virtqueue_enqueue((struct virtqueue *)vq, cookie, sg,
	    readable, writable));
}

void *
fbsd_vq_dequeue(struct fbsd_vq *vq, unsigned int *len)
{
	uint32_t len32 = 0;	/* virtqueue_dequeue leaves it untouched on NULL */
	void *cookie;

	cookie = virtqueue_dequeue((struct virtqueue *)vq, &len32);
	if (len != NULL)
		*len = len32;
	return (cookie);
}

/*
 * Map the notify window ourselves and never use virtqueue_notify(9):
 * stores through vtpci's notify sub-map fault on QEMU/hvf aarch64,
 * while a fresh pmap_mapdev() mapping of the same GPA works.
 */
static int
vgd_map_notify(struct virtio_gpu_drm_softc *sc)
{
	device_t parent = device_get_parent(sc->vgd_dev);
	uint64_t bar, gpa;
	uint32_t capoff;
	int capreg, barid, error, i;

	for (error = pci_find_cap(parent, PCIY_VENDOR, &capreg); error == 0;
	    error = pci_find_next_cap(parent, PCIY_VENDOR, capreg, &capreg)) {
		if (pci_read_config(parent, capreg + 3, 1) ==
		    2 /* VIRTIO_PCI_CAP_NOTIFY_CFG */)
			break;
	}
	if (error != 0)
		return (ENXIO);
	barid = pci_read_config(parent, capreg + 4, 1);
	capoff = pci_read_config(parent, capreg + 8, 4);
	bar = pci_read_config(parent, PCIR_BAR(barid), 4);
	if ((bar & PCIM_BAR_MEM_TYPE) == PCIM_BAR_MEM_64)
		bar |= (uint64_t)pci_read_config(parent,
		    PCIR_BAR(barid + 1), 4) << 32;
	bar &= ~(uint64_t)0xf;
	gpa = bar + capoff;

	sc->vgd_notify_va = pmap_mapdev(gpa, PAGE_SIZE);
	if (sc->vgd_notify_va == NULL)
		return (ENXIO);
	for (i = 0; i < VIRTGPU_NVQS; i++)
		sc->vgd_notify_off[i] =
		    VIRTIO_PCI_GET_VQ_NOTIFY_OFF(parent, i);
	sc->vgd_notify_ok = 1;
	device_printf(sc->vgd_dev,
	    "notify window mapped at %#jx (queue offsets %#jx, %#jx)\n",
	    (uintmax_t)gpa, (uintmax_t)sc->vgd_notify_off[0],
	    (uintmax_t)sc->vgd_notify_off[1]);
	return (0);
}

void
fbsd_gpu_notify(device_t dev, unsigned int index)
{
	struct virtio_gpu_drm_softc *sc = device_get_softc(dev);

	KASSERT(sc->vgd_notify_ok && index < VIRTGPU_NVQS,
	    ("notify before map, or bad index %u", index));
	/* Ring updates must be visible to the host before the kick. */
	mb();
	*(volatile uint16_t *)((uint8_t *)sc->vgd_notify_va +
	    sc->vgd_notify_off[index]) = index;
}

int
fbsd_vq_nfree(struct fbsd_vq *vq)
{
	return (virtqueue_nfree((struct virtqueue *)vq));
}

int
fbsd_vq_enable_intr(struct fbsd_vq *vq)
{
	return (virtqueue_enable_intr((struct virtqueue *)vq));
}

void
fbsd_vq_disable_intr(struct fbsd_vq *vq)
{
	virtqueue_disable_intr((struct virtqueue *)vq);
}

void
fbsd_gpu_device_ready(device_t dev)
{
	/* DRIVER_OK; the Linux driver submits commands during probe. */
	virtio_reinit_complete(dev);
}

void
fbsd_gpu_read_config(device_t dev, unsigned int offset, void *dst, int len)
{
	virtio_read_device_config(dev, offset, dst, len);
}

void
fbsd_gpu_write_config(device_t dev, unsigned int offset, void *src, int len)
{
	virtio_write_device_config(dev, offset, src, len);
}

int
fbsd_gpu_alloc_vqs(device_t dev, int nvqs, void *lvqs[],
    const char *names[], struct fbsd_vq *vqs[])
{
	struct virtio_gpu_drm_softc *sc = device_get_softc(dev);
	struct vq_alloc_info info[VIRTGPU_NVQS];
	struct virtqueue *fvq[VIRTGPU_NVQS] = { NULL };
	int i, error;

	KASSERT(nvqs <= VIRTGPU_NVQS, ("too many vqs"));

	for (i = 0; i < nvqs; i++)
		VQ_ALLOC_INFO_INIT(&info[i], VIRTGPU_MAX_INDIRECT,
		    lkpi_virtio_vq_intr, lvqs[i], &fvq[i],
		    "%s %s", device_get_nameunit(dev), names[i]);

	error = virtio_alloc_virtqueues(dev, nvqs, info);
	if (error != 0)
		return (error);
	error = virtio_setup_intr(dev, INTR_TYPE_TTY);
	if (error != 0)
		return (error);

	for (i = 0; i < nvqs; i++) {
		vqs[i] = (struct fbsd_vq *)fvq[i];
		sc->vgd_vqs[i] = vqs[i];
		/*
		 * FreeBSD virtqueues start with interrupts disabled and
		 * drivers arm them explicitly; Linux vrings start enabled
		 * and the upstream driver relies on that.
		 */
		virtqueue_enable_intr(fvq[i]);
	}
	return (0);
}

static int
vgd_reaps_sysctl(struct sysctl_oid *oidp, void *arg1, intmax_t arg2 __unused,
    struct sysctl_req *req)
{
	struct virtio_gpu_drm_softc *sc = arg1;
	uint64_t v = lkpi_virtio_reap_count(sc->vgd_vdev);

	return (sysctl_handle_64(oidp, &v, 0, req));
}

static int
vgd_qstate_sysctl(struct sysctl_oid *oidp, void *arg1, intmax_t arg2 __unused,
    struct sysctl_req *req)
{
	struct virtio_gpu_drm_softc *sc = arg1;
	struct virtqueue *ctrl = (struct virtqueue *)sc->vgd_vqs[0];
	struct virtqueue *cursor = (struct virtqueue *)sc->vgd_vqs[1];
	char buf[128];

	snprintf(buf, sizeof(buf),
	    "ctrl nfree=%d nused=%d | cursor nfree=%d nused=%d",
	    ctrl != NULL ? virtqueue_nfree(ctrl) : -1,
	    ctrl != NULL ? virtqueue_nused(ctrl) : -1,
	    cursor != NULL ? virtqueue_nfree(cursor) : -1,
	    cursor != NULL ? virtqueue_nused(cursor) : -1);
	return (sysctl_handle_string(oidp, buf, sizeof(buf), req));
}

static int
virtio_gpu_drm_probe(device_t dev)
{
	if (virtio_get_device_type(dev) != VIRTIO_ID_GPU)
		return (ENXIO);
	device_set_desc(dev, "VirtIO GPU (DRM)");
	/* Outrank the base vtgpu(4) driver (BUS_PROBE_DEFAULT). */
	return (BUS_PROBE_VENDOR);
}

static int
virtio_gpu_drm_attach(device_t dev)
{
	struct virtio_gpu_drm_softc *sc;
	int error;

	sc = device_get_softc(dev);
	sc->vgd_dev = dev;

	/*
	 * A post-boot attach runs outside vtpci's boot-time wrapper;
	 * drive the virtio 1.x status sequence ourselves.
	 */
	virtio_stop(dev);
	error = virtio_reinit(dev, VGD_FEATURES);
	if (error != 0) {
		device_printf(dev, "virtio_reinit failed: %d\n", error);
		return (error);
	}
	sc->vgd_features = 0;
	if (virtio_with_feature(dev, VIRTIO_F_VERSION_1))
		sc->vgd_features |= VIRTIO_F_VERSION_1;
	if (virtio_with_feature(dev, 1ULL << VIRTIO_GPU_F_EDID))
		sc->vgd_features |= 1ULL << VIRTIO_GPU_F_EDID;
	if (virtio_with_feature(dev, 1ULL << VIRTIO_GPU_F_VIRGL))
		sc->vgd_features |= 1ULL << VIRTIO_GPU_F_VIRGL;
	if (virtio_with_feature(dev, VIRTIO_F_IOMMU_PLATFORM))
		sc->vgd_features |= VIRTIO_F_IOMMU_PLATFORM;
	if (virtio_with_feature(dev, VIRTIO_RING_F_INDIRECT_DESC))
		sc->vgd_features |= VIRTIO_RING_F_INDIRECT_DESC;
	device_printf(dev, "negotiated features 0x%016jx%s%s%s%s\n",
	    (uintmax_t)sc->vgd_features,
	    (sc->vgd_features & (1ULL << VIRTIO_GPU_F_EDID)) ? " EDID" : "",
	    (sc->vgd_features & (1ULL << VIRTIO_GPU_F_VIRGL)) ? " VIRGL" : "",
	    (sc->vgd_features & VIRTIO_F_IOMMU_PLATFORM) ?
	    " ACCESS_PLATFORM" : "",
	    (sc->vgd_features & VIRTIO_RING_F_INDIRECT_DESC) ?
	    " INDIRECT" : "");

	error = vgd_map_notify(sc);
	if (error != 0) {
		device_printf(dev, "notify mapping failed: %d\n", error);
		return (error);
	}

	sc->vgd_vdev = lkpi_virtio_vdev_create(dev, device_get_parent(dev),
	    sc->vgd_features);
	if (sc->vgd_vdev == NULL) {
		error = ENOMEM;
		goto fail;
	}

	error = lkpi_virtio_gpu_attach(sc->vgd_vdev);
	if (error != 0) {
		device_printf(dev, "virtio_gpu probe failed: %d\n", error);
		error = (error < 0) ? -error : error;
		goto fail;
	}
	sc->vgd_attached = 1;
	vgd_ndevs++;
	SYSCTL_ADD_PROC(device_get_sysctl_ctx(dev),
	    SYSCTL_CHILDREN(device_get_sysctl_tree(dev)), OID_AUTO,
	    "reaps", CTLTYPE_U64 | CTLFLAG_RD | CTLFLAG_MPSAFE, sc, 0,
	    vgd_reaps_sysctl, "QU", "completions reaped via the shim");
	SYSCTL_ADD_PROC(device_get_sysctl_ctx(dev),
	    SYSCTL_CHILDREN(device_get_sysctl_tree(dev)), OID_AUTO,
	    "queue_state", CTLTYPE_STRING | CTLFLAG_RD | CTLFLAG_MPSAFE, sc, 0,
	    vgd_qstate_sysctl, "A", "guest-side vq counters");
	device_printf(dev, "DRM device registered\n");

	return (0);
fail:
	virtio_stop(dev);	/* do not leave DRIVER set without DRIVER_OK */
	if (sc->vgd_notify_ok) {
		pmap_unmapdev(sc->vgd_notify_va, PAGE_SIZE);
		sc->vgd_notify_ok = 0;
	}
	lkpi_virtio_vdev_destroy(sc->vgd_vdev);
	sc->vgd_vdev = NULL;
	return (error);
}

static int
virtio_gpu_drm_detach(device_t dev)
{
	struct virtio_gpu_drm_softc *sc = device_get_softc(dev);

	/*
	 * DRM devfs teardown panics in drm-kmod ("can't remove
	 * non-dynamic nodes"); refuse detach once registered.
	 */
	if (sc->vgd_attached)
		return (EBUSY);
	virtio_stop(dev);
	if (sc->vgd_notify_ok) {
		pmap_unmapdev(sc->vgd_notify_va, PAGE_SIZE);
		sc->vgd_notify_ok = 0;
	}
	lkpi_virtio_vdev_destroy(sc->vgd_vdev);
	sc->vgd_vdev = NULL;
	return (0);
}

static int
virtio_gpu_drm_config_change(device_t dev)
{
	struct virtio_gpu_drm_softc *sc = device_get_softc(dev);

	if (sc->vgd_attached)
		lkpi_virtio_gpu_config_changed(sc->vgd_vdev);
	return (0);
}

static device_method_t virtio_gpu_drm_methods[] = {
	DEVMETHOD(device_probe,		virtio_gpu_drm_probe),
	DEVMETHOD(device_attach,	virtio_gpu_drm_attach),
	DEVMETHOD(device_detach,	virtio_gpu_drm_detach),

	DEVMETHOD(virtio_config_change,	virtio_gpu_drm_config_change),

	DEVMETHOD_END
};

static driver_t virtio_gpu_drm_driver = {
	.name = "virtio_gpu_drm",
	.methods = virtio_gpu_drm_methods,
	.size = sizeof(struct virtio_gpu_drm_softc),
};

static int
virtio_gpu_drm_modevent(module_t mod __unused, int type, void *arg __unused)
{
	switch (type) {
	case MOD_QUIESCE:
		/* kldunload would panic in detach; see above. */
		return (vgd_ndevs != 0 ? EBUSY : 0);
	case MOD_LOAD:
	case MOD_UNLOAD:
	case MOD_SHUTDOWN:
		return (0);
	default:
		return (EOPNOTSUPP);
	}
}

VIRTIO_DRIVER_MODULE(virtio_gpu_drm, virtio_gpu_drm_driver,
    virtio_gpu_drm_modevent, NULL);
MODULE_VERSION(virtio_gpu_drm, 1);
MODULE_DEPEND(virtio_gpu_drm, virtio, 1, 1, 1);
MODULE_DEPEND(virtio_gpu_drm, linuxkpi, 1, 1, 1);
MODULE_DEPEND(virtio_gpu_drm, drmn, 2, 2, 2);
MODULE_DEPEND(virtio_gpu_drm, dmabuf, 1, 1, 1);
VIRTIO_SIMPLE_PNPINFO(virtio_gpu_drm, VIRTIO_ID_GPU, "VirtIO GPU (DRM)");
