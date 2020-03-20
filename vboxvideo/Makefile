# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/staging/vboxvideo

.PATH:	${SRCDIR}

KMOD= 	vboxvideo
SRCS= 	\
	hgsmi_base.c \
	modesetting.c \
	vbox_drv.c \
	vbox_fb.c \
	vbox_hgsmi.c \
	vbox_irq.c \
	vbox_main.c \
	vbox_mode.c \
	vbox_prime.c \
	vbox_ttm.c \
	vbva_base.c


CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}/gpu

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=${KMOD}_'
CFLAGS+= -include ${SRCDIR:H:H}/gpu/drm/drm_os_config.h

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

CWARNFLAGS.vbva_base.c= -Wno-cast-qual -Wno-pointer-arith
CWARNFLAGS.vbox_hgsmi.c= -Wno-cast-qual
CWARNFLAGS.vbox_main.c= -Wno-pointer-arith
CWARNFLAGS.vbox_mode.c= -Wno-cast-qual

.include <bsd.kmod.mk>
