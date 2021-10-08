/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright 2021 Alex Richardson <arichardson@FreeBSD.org>
 *
 * This work was supported by Innovate UK project 105694, "Digital Security by
 * Design (DSbD) Technology Platform Prototype".
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
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/bus.h>
#include <sys/module.h>

#include <dev/virtio/virtio.h>

#define __DO_NOT_DEFINE_LINUX_VIRTIO_NAMES
#include <linux/virtio.h>
#include <linux/virtio_gpu.h>

#include "virtio_if.h"

SYSCTL_NODE(_hw, OID_AUTO, virtio_gpu, CTLFLAG_RW | CTLFLAG_MPSAFE, 0,
    "VirtIO GPU parameters");

struct vtgpu_softc {
	struct linux_virtio_device linux_dev;
};

static struct virtio_feature_desc vtgpu_feature_desc[] = {
#if BYTE_ORDER == LITTLE_ENDIAN
	/*
	 * Gallium command stream send by virgl is native endian.
	 * Because of that we only support little endian guests on
	 * little endian hosts.
	 */
	{ BIT_ULL(VIRTIO_GPU_F_VIRGL), "Virgl3D" },
#else
#error "This driver is only useful on little-endian systems"
#endif
	{ BIT_ULL(VIRTIO_GPU_F_EDID), "EDID" },
	{ 0, NULL },
};

static int vtgpu_probe(device_t dev);
static int vtgpu_attach(device_t dev);
static int vtgpu_detach(device_t dev);
static int vtgpu_config_change(device_t dev);

static device_method_t vtgpu_methods[] = {
	/* Device methods. */
	DEVMETHOD(device_probe, vtgpu_probe),
	DEVMETHOD(device_attach, vtgpu_attach),
	DEVMETHOD(device_detach, vtgpu_detach),

	/* VirtIO methods. */
	DEVMETHOD(virtio_config_change, vtgpu_config_change),

	DEVMETHOD_END
};

static int
vtgpu_modevent(module_t mod, int type, void *unused)
{
	int error;

	error = 0;

	switch (type) {
	case MOD_LOAD:
	case MOD_QUIESCE:
	case MOD_UNLOAD:
	case MOD_SHUTDOWN:
		break;
	default:
		error = EOPNOTSUPP;
		break;
	}

	return (error);
}

static driver_t vtgpu_driver = { "vtgpu", vtgpu_methods,
	sizeof(struct vtgpu_softc) };
static devclass_t vtgpu_devclass;

VIRTIO_DRIVER_MODULE(virtio_gpu, vtgpu_driver, vtgpu_devclass, vtgpu_modevent,
    0);
MODULE_VERSION(virtio_gpu, 1);
VIRTIO_SIMPLE_PNPINFO(virtio_gpu, VIRTIO_ID_GPU, "Virtio GPU driver");

/* TODO: are all those modules really needed? */
MODULE_DEPEND(virtio_gpu, drmn, 2, 2, 2);
MODULE_DEPEND(virtio_gpu, ttm, 1, 1, 1);
MODULE_DEPEND(virtio_gpu, agp, 1, 1, 1);
MODULE_DEPEND(virtio_gpu, virtio, 1, 1, 1);
MODULE_DEPEND(virtio_gpu, linuxkpi, 1, 1, 1);
/* TODO: linuxkpi_gplv2 should not be needed. */
MODULE_DEPEND(virtio_gpu, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(virtio_gpu, debugfs, 1, 1, 1);

extern int virtio_gpu_probe(struct linux_virtio_device *vdev);
extern void virtio_gpu_remove(struct linux_virtio_device *vdev);
extern void virtio_gpu_config_changed(struct linux_virtio_device *vdev);

static int
vtgpu_probe(device_t dev)
{
	printf("vtgpu_probe\n");
	return (VIRTIO_SIMPLE_PROBE(dev, virtio_gpu));
}

static int
vtgpu_attach(device_t dev)
{
	struct vtgpu_softc *sc = device_get_softc(dev);
	printf("vtgpu_attach\n");
	return (virtio_gpu_probe(&sc->linux_dev));
}

static int
vtgpu_detach(device_t dev)
{
	struct vtgpu_softc *sc = device_get_softc(dev);
	printf("vtgpu_detach\n");
	virtio_gpu_remove(&sc->linux_dev);
	return (0);
}

static int
vtgpu_config_change(device_t dev)
{
	struct vtgpu_softc *sc = device_get_softc(dev);

	printf("vtgpu_config_change\n");
	virtio_gpu_config_changed(&sc->linux_dev);
	return (0);
}
