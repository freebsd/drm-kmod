# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/vmwgfx

.PATH:	${SRCDIR}

.include "../kconfig.mk"

KMOD=	vmwgfx
SRCS=	\
	ttm_lock.c \
	ttm_object.c \
	vmwgfx_binding.c \
	vmwgfx_blit.c \
	vmwgfx_bo.c \
	vmwgfx_cmdbuf_res.c \
	vmwgfx_cmdbuf.c \
	vmwgfx_context.c \
	vmwgfx_cotable.c \
	vmwgfx_drv.c \
	vmwgfx_execbuf.c \
	vmwgfx_fb.c \
	vmwgfx_fb_freebsd.c \
	vmwgfx_fence.c \
	vmwgfx_fifo.c \
	vmwgfx_gmr.c \
	vmwgfx_gmrid_manager.c \
	vmwgfx_ioctl.c \
	vmwgfx_irq.c \
	vmwgfx_kms.c \
	vmwgfx_ldu.c \
	vmwgfx_marker.c \
	vmwgfx_mob.c \
	vmwgfx_msg.c \
	vmwgfx_overlay.c \
	vmwgfx_prime.c \
	vmwgfx_resource.c \
	vmwgfx_scrn.c \
	vmwgfx_shader.c \
	vmwgfx_simple_resource.c \
	vmwgfx_so.c \
	vmwgfx_stdu.c \
	vmwgfx_surface.c \
	vmwgfx_ttm_buffer.c \
	vmwgfx_ttm_glue.c \
	vmwgfx_va.c \
	vmwgfx_validation.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
.if exists(${SYSDIR}/compat/linuxkpi/common/linuxkpi.h)
CFLAGS+= -include ${SYSDIR}/compat/linuxkpi/common/linuxkpi.h
.endif
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=${KMOD}_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD} -DLINUXKPI_VERSION=50000
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

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

.include <bsd.kmod.mk>

CWARNFLAGS+= -Wno-expansion-to-defined -Wno-pointer-arith

CWARNFLAGS.ttm_object.c= -Wno-incompatible-pointer-types

CWARNFLAGS.vmwgfx_blit.c= -Wno-cast-qual
CWARNFLAGS.vmwgfx_execbuf.c= -Wno-cast-qual -Wno-sometimes-uninitialized
CWARNFLAGS.vmwgfx_irq.c= -Wno-uninitialized
CWARNFLAGS.vmwgfx_kms.c= -Wno-missing-prototypes
CWARNFLAGS.vmwgfx_msg.c= -Wno-missing-prototypes
CWARNFLAGS.vmwgfx_overlay.c= -Wno-unused-function
CWARNFLAGS.vmwgfx_stdu.c= -Wno-cast-qual
