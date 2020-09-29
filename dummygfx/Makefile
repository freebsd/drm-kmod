# $FreeBSD$

SRCDIR=	${.CURDIR}

.PATH:	${SRCDIR}

KMOD=	dummygfx
SRCS=	\
	dummygfx_drv.c \
	dummygfx_debugfs.c

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
CFLAGS+= -I${SRCDIR:H}/drivers/gpu

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=dummygfx_'
CFLAGS+= -include ${SRCDIR:H}/drivers/gpu/drm/drm_os_config.h

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
	vnode_if.h

.include <bsd.kmod.mk>

CWARNFLAGS += -Wno-cast-qual
