# $FreeBSD$

#.PATH:	${SRCTOP}/sys/dev/drm/amd/amdkfd

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

CFLAGS+=	-I${SRC_BASE}/sys/compat/linuxkpi/common/include
CFLAGS+=	-I${SRC_BASE}/sys/compat/linuxkpi/common/include/drm
CFLAGS+=	-I${SRC_BASE}/sys/compat/linuxkpi/common/include/uapi
CFLAGS+=	-I${SRC_BASE}/sys/compat/linuxkpi/gplv2/include
CFLAGS+=	-I.
CFLAGS+=	-I../amdgpu
CFLAGS+=	-I../include
CFLAGS+=	-I../acp/include
CFLAGS+=	-I../powerplay/inc
CFLAGS+=	-I../scheduler
CFLAGS+=	-I../include/asic_reg
CFLAGS+=	'-DKBUILD_MODNAME="${KMOD}"'

SRCS	+=device_if.h vnode_if.h bus_if.h pci_if.h device_if.h iicbus_if.h opt_drm.h \
        opt_vm.h opt_compat.h opt_syscons.h

.include <bsd.kmod.mk>

CWARNFLAGS+=	-Wno-pointer-arith
CWARNFLAGS+=	-Wno-pointer-sign ${CWARNFLAGS.${.IMPSRC:T}}

CWARNFLAGS.kfd_chardev.c=	-Wno-format
CWARNFLAGS.kfd_dbgdev.c=	-Wno-format
CWARNFLAGS.kfd_device_queue_manager.c=	-Wno-format
CWARNFLAGS.kfd_doorbell.c=	-Wno-format
CWARNFLAGS.kfd_packet_manager.c=	-Wno-format
CWARNFLAGS.kfd_queue.c=	-Wno-format
