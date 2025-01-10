SRCDIR=	${.CURDIR:H}/drivers/dma-buf

.PATH:	${SRCDIR}

.include "../kconfig.mk"
.include "../linuxkpi_version.mk"

KMOD=	dmabuf
SRCS=	dma-buf-kmod.c \
	dma-buf.c \
	dma-fence-array.c \
	dma-fence-chain.c \
	dma-fence-unwrap.c \
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
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

CWARNFLAGS.dma-buf.c+=	-Wno-pointer-arith

EXPORT_SYMS=	YES

.include <bsd.kmod.mk>
