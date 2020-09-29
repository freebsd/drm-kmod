# $FreeBSD$

SRCDIR=	${.CURDIR:H:H}/drivers/gpu/drm/amd

.PATH:	${SRCDIR}/amdkfd

KMOD	= amdkfd

# XXX NOT PORTED YET:
#	kfd_device.c : amd_iommu 
#	kfd_chardev.c : lots of missing stuff
#	kfd_device_queue_manager_cik.c : memset declaration missing in rwsem
#	kfd_device_queue_manager_vi.c : memset declaration missing in rwsem
#	kfd_events.c : incomplete mm struct
#	kfd_module.c : module version macros
#	kfd_mqd_manager_cik.c : we seem to be missing an include w/ basic types
#	kfd_mqd_manager_vi.c : we seem to be missing an include w/ basic types
#	kfd_process.c : srcu
#	kfd_topology.c : rwsems

SRCS	= \
	cik_event_interrupt.c \
	kfd_dbgdev.c \
	kfd_dbgmgr.c \
	kfd_device_queue_manager.c \
	kfd_doorbell.c \
	kfd_flat_memory.c \
	kfd_interrupt.c \
	kfd_kernel_queue.c \
	kfd_kernel_queue_cik.c \
	kfd_kernel_queue_vi.c \
	kfd_mqd_manager.c \
	kfd_packet_manager.c \
	kfd_pasid.c \
	kfd_process_queue_manager.c \
	kfd_queue.c #\
#	kfd_topology.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
.if exists(${SYSDIR}/compat/linuxkpi/common/linuxkpi.h)
CFLAGS+= -include ${SYSDIR}/compat/linuxkpi/common/linuxkpi.h
.endif
CFLAGS+= -I${.CURDIR:H:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H:H}/include
CFLAGS+= -I${.CURDIR:H:H}/include/drm
CFLAGS+= -I${.CURDIR:H:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= -I${SRCDIR}/amdgpu
CFLAGS+= -I${SRCDIR}/include
CFLAGS+= -I${SRCDIR}/acp/include
CFLAGS+= -I${SRCDIR}/powerplay/inc
CFLAGS+= -I${SRCDIR}/scheduler
CFLAGS+= -I${SRCDIR}/include/asic_reg

CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=amdkfd_'
CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'

SRCS+=	device_if.h vnode_if.h bus_if.h pci_if.h pci_iov_if.h device_if.h iicbus_if.h opt_drm.h \
	opt_vm.h opt_compat.h opt_syscons.h

.include <bsd.kmod.mk>

CWARNFLAGS+=	-Wno-pointer-arith
CWARNFLAGS+=	-Wno-pointer-sign ${CWARNFLAGS.${.IMPSRC:T}}

CWARNFLAGS.kfd_chardev.c=	-Wno-format
CWARNFLAGS.kfd_dbgdev.c=	-Wno-format
CWARNFLAGS.kfd_device_queue_manager.c=	-Wno-format -Wno-strict-prototypes -Wno-implicit-function-declaration
CWARNFLAGS.kfd_doorbell.c=	-Wno-format
CWARNFLAGS.kfd_packet_manager.c=	-Wno-format
CWARNFLAGS.kfd_queue.c= -Wno-format
