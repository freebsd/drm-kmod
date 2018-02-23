# $FreeBSD$

SRCDIR=	${.CURDIR:H:H}/drivers/gpu/drm/vc4

.PATH:	${SRCDIR}

KMOD= vc4
SRCS= \
	vc4_freebsd.c 

# Cause build error because of missing lkpi functionality
# 	vc4_crtc.c \
# 	vc4_plane.c \
# 	vc4_render_cl.c \
# 	vc4_debugfs.c \
# 	vc4_trace_points.c \
# 	vc4_validate_shaders.c \
# 	vc4_bo.c \
# 	vc4_vec.c \
# 	vc4_dsi.c \
# 	vc4_gem.c \
# 	vc4_hvs.c \
# 	vc4_v3d.c \
# 	vc4_drv.c \
# 	vc4_validate.c \
# 	vc4_dpi.c \
# 	vc4_hdmi.c \
# 	vc4_irq.c \
# 	vc4_kms.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${SRCDIR:H}/drm_os_config.h


# Copied from i915kms, can probably clean up a bit
SRCS	+=								\
	opt_acpi.h							\
	opt_compat.h						\
	opt_drm.h							\
	opt_syscons.h						\
	acpi_if.h							\
	bus_if.h							\
	fb_if.h								\
	device_if.h							\
	pci_if.h							\
	vnode_if.h

.include <bsd.kmod.mk>
