# `kconfig.mk` defines variables used to conditionally add files to $(SRCS).
.include "../kconfig.mk"

SRCDIR=	${.CURDIR:H}/drivers/video

.PATH:	${SRCDIR}

KMOD=	linuxkpi_video

EXPORT_SYMS=    YES

SRCS=	linux_hdmi.c \
	linux_fb.c \
	video_kmod.c \
	vt_drmfb.c

.if !empty(KCONFIG:MAPERTURE_HELPERS)
SRCS+=	linux_aperture.c
.endif

.if !empty(KCONFIG:MVIDEO_CMDLINE)
SRCS+=	linux_cmdline.c
.endif

SRCS+=	bus_if.h \
	device_if.h \
	fb_if.h \
	pci_if.h \
	vnode_if.h \
	opt_splash.h \
	opt_syscons.h \
	opt_teken.h

.include <bsd.kmod.mk>

# These Makefiles are included after <bsd.kmod.mk> because they expand
# variables defined by it.
.include "../linuxkpi_version.mk"
.include "../compiler_flags.mk"

CFLAGS+=	-I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+=	-I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/dummy/include
CFLAGS+=	-I${.CURDIR:H}/include
CFLAGS+=	-I${SRCDIR}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=ttm_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

CWARNFLAGS.linux_hdmi.c=	-Wno-cast-qual
