# $FreeBSD$

.PATH: ${.CURDIR}

KMOD=	debugfs
SRCS=	vnode_if.h \
	device_if.h bus_if.h  pci_if.h \
	lindebugfs.c

.if ${MACHINE_CPUARCH} == "powerpc"
CFLAGS+= -DSMP
CFLAGS+= -DAIM
.endif

CFLAGS+= -I${.CURDIR}/../linuxkpi/dummy/include
CFLAGS+= -I${.CURDIR}/../linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS.gcc+= -Wno-cast-qual

.include <bsd.kmod.mk>
