# `kconfig.mk` defines variables used to conditionally add files to $(SRCS).
.include "../kconfig.mk"

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/virtio

KMOD=	virtio_gpu_drm

EXPORT_SYMS=	YES

.PATH:	${SRCDIR} ${SRCDIR}/freebsd ${SRCDIR:H}

# drm_simple_kms_helper.c is not part of drm.ko; build it here for
# drm_simple_encoder_init().
SRCS=	virtgpu_freebsd.c \
	linux_virtio.c \
	drm_simple_kms_helper.c \
	virtgpu_vram.c \
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
	virtgpu_submit.c \
	virtgpu_trace_points.c \
	virtgpu_vq.c

SRCS+=	\
	opt_drm.h		\
	opt_platform.h		\
	vnode_if.h		\
	bus_if.h		\
	device_if.h		\
	pci_if.h		\
	virtio_bus_if.h		\
	virtio_if.h		\
	virtio_pci_if.h

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

.include <bsd.kmod.mk>

# These Makefiles are included after <bsd.kmod.mk> because they expand
# variables defined by it.
.include "../linuxkpi_version.mk"
.include "../compiler_flags.mk"

# The shim headers under freebsd/ must resolve before any other
# include root.
CFLAGS+= -I${SRCDIR}/freebsd

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}		# <drm/drm_os_freebsd.h>
CFLAGS+= -I${SRCDIR}

CFLAGS+= -DCONFIG_DRM_VIRTIO_GPU_KMS=1
CFLAGS+= '-DKBUILD_MODNAME="virtio_gpu"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=virtio_gpu_' -DDRM_SYSCTL_PARAM_PREFIX=_virtio_gpu

# The full shared KCONFIG set, as every kmod in this tree uses: DRM
# header struct layouts and inline functions depend on CONFIG_* values,
# so all modules must see the same set as drm.ko or their ABIs diverge.
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

# Base LinuxKPI's iosys-map.h does void-pointer arithmetic; forced on
# every TU that includes DRM headers, so it must stay global.
CWARNFLAGS+= -Wno-gnu-pointer-arith
# Linux u64 printf formats; upstream files only — the FreeBSD TUs
# compile with the full kernel warning set.
CWARNFLAGS.virtgpu_fence.c+= -Wno-format
CWARNFLAGS.virtgpu_vq.c+= -Wno-format
