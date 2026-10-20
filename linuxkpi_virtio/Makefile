# `kconfig.mk` defines variables used to conditionally add files to $(SRCS).
.include "../kconfig.mk"

SRCDIR=	${.CURDIR:H}/drivers/virtio

.PATH:	${SRCDIR}

KMOD=	linuxkpi_virtio

EXPORT_SYMS=	YES

SRCS=	linuxkpi_virtio.c

SRCS+=	bus_if.h \
	device_if.h \
	pci_if.h \
	vnode_if.h \
	virtio_if.h

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

.include <bsd.kmod.mk>

# These Makefiles are included after <bsd.kmod.mk> because they expand
# variables defined by it.
.include "../linuxkpi_version.mk"
.include "../compiler_flags.mk"

CFLAGS+=	-I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+=	-I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+=	-I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}
