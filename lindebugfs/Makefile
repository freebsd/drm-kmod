# $FreeBSD$

.PATH: ${.CURDIR}

KMOD=	debugfs
SRCS=	vnode_if.h \
	device_if.h bus_if.h  pci_if.h \
	lindebugfs.c

CFLAGS+= -I${.CURDIR}/../linuxkpi/dummy/include
CFLAGS+= -I${.CURDIR}/../linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include

.include <bsd.kmod.mk>
