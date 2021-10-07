# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/virtio

.PATH:	${SRCDIR} ${SRCDIR}/../

.include "../kconfig.mk"

KMOD= 	virtio_gpu
SRCS= 	\
	virtgpu_bsdmodule.c \
	virtgpu_debugfs.c \
	virtgpu_display.c \
	virtgpu_drv.c \
	virtgpu_fence.c \
	virtgpu_gem.c \
	virtgpu_ioctl.c \
	virtgpu_kms.c \
	virtgpu_object.c \
	virtgpu_plane.c \
	virtgpu_prime.c \
	virtgpu_vq.c

# TODO: virtgpu_trace_points.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include/uapi
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
kmod_prefix=${KMOD:S/-/_/g}
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=${KMOD}_' '-DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}' -DLINUXKPI_VERSION=50000
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

# TODO: are any of these needed?
SRCS	+=			\
	opt_acpi.h		\
	opt_compat.h		\
	opt_drm.h		\
	opt_platform.h		\
	opt_splash.h		\
	opt_syscons.h		\
	opt_teken.h		\
	acpi_if.h		\
	bus_if.h		\
	fb_if.h			\
	device_if.h		\
	pci_if.h		\
	pci_iov_if.h		\
	vnode_if.h

# Lots of these, probably not worth fixing
CWARNFLAGS+=	-Wno-pointer-arith -Wno-pointer-sign -Wno-error=format

.include <bsd.kmod.mk>
