# `kconfig.mk` defines variables used to conditionally add files to $(SRCS).
.include "../kconfig.mk"

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/virtio

.PATH:	${SRCDIR}

KMOD=	virtio_gpu_drm

EXPORT_SYMS=	YES

SRCS=	virtgpu_debugfs.c \
	virtgpu_display.c \
	virtgpu_drv.c \
	virtgpu_fence.c \
	virtgpu_freebsd.c \
	virtgpu_gem.c \
	virtgpu_ioctl.c \
	virtgpu_kms.c \
	virtgpu_object.c \
	virtgpu_plane.c \
	virtgpu_prime.c \
	virtgpu_submit.c \
	virtgpu_vq.c

# GPL-2.0.
SRCS+=	virtgpu_vram.c

SRCS+=	bus_if.h \
	device_if.h \
	opt_drm.h \
	pci_if.h \
	vnode_if.h

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

.include <bsd.kmod.mk>

# These Makefiles are included after <bsd.kmod.mk> because they expand
# variables defined by it.
.include "../linuxkpi_version.mk"
.include "../compiler_flags.mk"

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}		# <drm/drm_os_freebsd.h>
CFLAGS+= -I${SRCDIR}

# Linux's module name, also the newbus driver name.
CFLAGS+= '-DKBUILD_MODNAME="virtio_gpu"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=virtio_gpu_' \
	-DDRM_SYSCTL_PARAM_PREFIX=_virtio_gpu
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

CWARNFLAGS+= -Wno-pointer-arith -Wno-format
