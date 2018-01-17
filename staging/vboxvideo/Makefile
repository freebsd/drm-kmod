# $FreeBSD$

KMOD= vboxvideo

SRCS= vboxvideo_freebsd.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${.CURDIR:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${.CURDIR:H}/drm/drm_os_config.h


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
