SRCDIR=	${.CURDIR:H}/drivers/dma-buf

.PATH:	${SRCDIR}

.include "../kconfig.mk"

KMOD=	dmabuf
SRCS=	dma-buf-kmod.c \
	dma-buf.c \
	dma-fence-array.c \
	dma-fence-chain.c \
	dma-fence.c \
	dma-resv.c \
	sync_file.c

SRCS+=	device_if.h \
	bus_if.h \
	vnode_if.h

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include # fallback to dummy

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -DLINUXKPI_VERSION=50000
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

.include <bsd.kmod.mk>
