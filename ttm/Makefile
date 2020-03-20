# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/ttm

.PATH:	${SRCDIR}

KMOD=	ttm
SRCS=	ttm_bo.c \
	ttm_bo_manager.c \
	ttm_bo_util.c \
	ttm_bo_vm.c \
	ttm_execbuf_util.c \
	ttm_memory.c \
	ttm_module.c \
	ttm_page_alloc.c \
	ttm_page_alloc_dma.c \
	ttm_tt.c

.if ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "amd64"
SRCS+=	ttm_agp_backend.c
.endif

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=ttm_'
CFLAGS+= -include ${SRCDIR:H}/drm_os_config.h

SRCS+=	device_if.h \
	vnode_if.h \
	bus_if.h \
	pci_if.h \
	pci_iov_if.h \
	device_if.h \
	iicbus_if.h \
	opt_drm.h \
	opt_vm.h \
	opt_compat.h \
	opt_syscons.h

.if ${MACHINE_CPUARCH} == "powerpc"
CWARNFLAGS+=-Wno-cast-qual
.endif
CWARNFLAGS+= -Wno-pointer-arith -Wno-pointer-sign -Wno-format
CWARNFLAGS+= -Wno-expansion-to-defined

.include <bsd.kmod.mk>
