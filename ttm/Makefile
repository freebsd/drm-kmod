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
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include
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

# drivers/gpu/drm/drm_gem_ttm_helper.c
EXPORT_SYMS+=	drm_gem_ttm_dumb_map_offset
EXPORT_SYMS+=	drm_gem_ttm_mmap
EXPORT_SYMS+=	drm_gem_ttm_print_info
EXPORT_SYMS+=	drm_gem_ttm_vmap
EXPORT_SYMS+=	drm_gem_ttm_vunmap

# drivers/gpu/drm/ttm/ttm_agp_backend.c
EXPORT_SYMS+=	ttm_agp_bind
EXPORT_SYMS+=	ttm_agp_destroy
EXPORT_SYMS+=	ttm_agp_is_bound
EXPORT_SYMS+=	ttm_agp_tt_create
EXPORT_SYMS+=	ttm_agp_unbind

# drivers/gpu/drm/ttm/ttm_bo.c
EXPORT_SYMS+=	ttm_bo_eviction_valuable
EXPORT_SYMS+=	ttm_bo_init_reserved
EXPORT_SYMS+=	ttm_bo_init_validate
EXPORT_SYMS+=	ttm_bo_mem_space
EXPORT_SYMS+=	ttm_bo_move_to_lru_tail
EXPORT_SYMS+=	ttm_bo_pin
EXPORT_SYMS+=	ttm_bo_put
EXPORT_SYMS+=	ttm_bo_set_bulk_move
EXPORT_SYMS+=	ttm_bo_unmap_virtual
EXPORT_SYMS+=	ttm_bo_unpin
EXPORT_SYMS+=	ttm_bo_validate
EXPORT_SYMS+=	ttm_bo_wait_ctx

# drivers/gpu/drm/ttm/ttm_bo_util.c
EXPORT_SYMS+=	ttm_bo_kmap
EXPORT_SYMS+=	ttm_bo_kunmap
EXPORT_SYMS+=	ttm_bo_move_accel_cleanup
EXPORT_SYMS+=	ttm_bo_move_memcpy
EXPORT_SYMS+=	ttm_bo_move_sync_cleanup
EXPORT_SYMS+=	ttm_bo_vmap
EXPORT_SYMS+=	ttm_bo_vunmap
EXPORT_SYMS+=	ttm_io_prot
EXPORT_SYMS+=	ttm_move_memcpy

# drivers/gpu/drm/ttm/ttm_bo_vm.c
EXPORT_SYMS+=	ttm_bo_mmap_obj
EXPORT_SYMS+=	ttm_bo_vm_access
EXPORT_SYMS+=	ttm_bo_vm_close
EXPORT_SYMS+=	ttm_bo_vm_dummy_page
EXPORT_SYMS+=	ttm_bo_vm_fault
EXPORT_SYMS+=	ttm_bo_vm_fault_reserved
EXPORT_SYMS+=	ttm_bo_vm_open
EXPORT_SYMS+=	ttm_bo_vm_reserve

# drivers/gpu/drm/ttm/ttm_device.c
EXPORT_SYMS+=	ttm_device_clear_dma_mappings
EXPORT_SYMS+=	ttm_device_fini
EXPORT_SYMS+=	ttm_device_init
EXPORT_SYMS+=	ttm_device_swapout
EXPORT_SYMS+=	ttm_glob

# drivers/gpu/drm/ttm/ttm_execbuf_util.c
EXPORT_SYMS+=	ttm_eu_backoff_reservation
EXPORT_SYMS+=	ttm_eu_fence_buffer_objects
EXPORT_SYMS+=	ttm_eu_reserve_buffers

# drivers/gpu/drm/ttm/ttm_pool.c
EXPORT_SYMS+=	ttm_pool_alloc
EXPORT_SYMS+=	ttm_pool_debugfs
EXPORT_SYMS+=	ttm_pool_fini
EXPORT_SYMS+=	ttm_pool_free
EXPORT_SYMS+=	ttm_pool_init

# drivers/gpu/drm/ttm/ttm_range_manager.c
EXPORT_SYMS+=	ttm_range_man_fini_nocheck
EXPORT_SYMS+=	ttm_range_man_init_nocheck

# drivers/gpu/drm/ttm/ttm_resource.c
EXPORT_SYMS+=	ttm_kmap_iter_iomap_init
EXPORT_SYMS+=	ttm_lru_bulk_move_init
EXPORT_SYMS+=	ttm_lru_bulk_move_tail
EXPORT_SYMS+=	ttm_resource_fini
EXPORT_SYMS+=	ttm_resource_free
EXPORT_SYMS+=	ttm_resource_init
EXPORT_SYMS+=	ttm_resource_manager_create_debugfs
EXPORT_SYMS+=	ttm_resource_manager_debug
EXPORT_SYMS+=	ttm_resource_manager_evict_all
EXPORT_SYMS+=	ttm_resource_manager_init
EXPORT_SYMS+=	ttm_resource_manager_usage

# drivers/gpu/drm/ttm/ttm_tt.c
EXPORT_SYMS+=	ttm_kmap_iter_tt_init
EXPORT_SYMS+=	ttm_sg_tt_init
EXPORT_SYMS+=	ttm_tt_fini
EXPORT_SYMS+=	ttm_tt_init
EXPORT_SYMS+=	ttm_tt_pages_limit
EXPORT_SYMS+=	ttm_tt_populate

.include <bsd.kmod.mk>
