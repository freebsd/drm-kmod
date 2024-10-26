SRCDIR=	${.CURDIR:H}/drivers/video

.PATH:	${SRCDIR}

.include "../kconfig.mk"
.include "../linuxkpi_version.mk"

KMOD=	linuxkpi_video
SRCS=	hdmi.c \
	video_kmod.c

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

CFLAGS+=	-I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+=	-I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/dummy/include
CFLAGS+=	-I${.CURDIR:H}/include

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=ttm_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

CWARNFLAGS.hdmi.c=	-Wno-cast-qual

EXPORT_SYMS=    YES

.include <bsd.kmod.mk>
