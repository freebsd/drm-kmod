SRCDIR=	${.CURDIR:H}/drivers/video

.PATH:	${SRCDIR}

.include "../kconfig.mk"
.include "../linuxkpi_version.mk"

KMOD=	linuxkpi_video
SRCS=	hdmi.c \
	linux_fb.c \
	video_kmod.c \
	vt_drmfb.c

.if !empty(KCONFIG:MAPERTURE_HELPERS)
SRCS+= aperture.c
.endif

.if !empty(KCONFIG:MVIDEO_CMDLINE)
SRCS+=	cmdline.c
.endif

SRCS+=	bus_if.h \
	device_if.h \
	pci_if.h \
	vnode_if.h \
	opt_splash.h \
	opt_syscons.h \
	opt_teken.h

CFLAGS+=	-I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+=	-I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/dummy/include
CFLAGS+=	-I${.CURDIR:H}/include
CFLAGS+=	-I${SRCDIR}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=ttm_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

CWARNFLAGS.hdmi.c=	-Wno-cast-qual
CWARNFLAGS.linux_fb.c=	-Wno-unused-variable -Wno-format

EXPORT_SYMS=    YES

.include <bsd.kmod.mk>
