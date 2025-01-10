# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/ttm

.PATH:	${SRCDIR} ${SRCDIR}/..

.include "../kconfig.mk"
.include "../linuxkpi_version.mk"

KMOD=	ttm
SRCS=	ttm_bo.c \
	ttm_bo_util.c \
	ttm_bo_vm.c \
	ttm_execbuf_util.c \
	ttm_module.c \
	ttm_pool.c \
	ttm_device.c \
	ttm_sys_manager.c \
	ttm_range_manager.c \
	ttm_resource.c \
	ttm_tt.c \
	drm_gem_ttm_helper.c

.if !empty(KCONFIG:MAGP)
SRCS+=	ttm_agp_backend.c
.endif

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=ttm_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS+=	device_if.h \
	vnode_if.h \
	bus_if.h \
	pci_if.h \
	pci_iov_if.h \
	device_if.h \
	iicbus_if.h \
	opt_drm.h \
	opt_vm.h \
	opt_syscons.h

.if ${MACHINE_CPUARCH} == "powerpc"
CWARNFLAGS+=-Wno-cast-qual
.endif
CWARNFLAGS+= -Wno-pointer-arith -Wno-pointer-sign -Wno-format
CWARNFLAGS+= -Wno-expansion-to-defined

EXPORT_SYMS=	YES

.include <bsd.kmod.mk>
