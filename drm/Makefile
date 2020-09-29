# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm

.PATH:	${SRCDIR} ${SRCDIR}/scheduler

.include "../kconfig.mk"

KMOD=	drm
SRCS=	drm_atomic.c \
	drm_atomic_helper.c \
	drm_atomic_state_helper.c \
	drm_atomic_uapi.c \
	drm_auth.c \
	drm_blend.c \
	drm_bridge.c \
	drm_cache.c \
	drm_client.c \
	drm_client_modeset.c \
	drm_color_mgmt.c \
	drm_connector.c \
	drm_crtc.c \
	drm_crtc_helper.c \
	drm_damage_helper.c \
	drm_dp_dual_mode_helper.c \
	drm_dp_helper.c \
	drm_dp_mst_topology.c \
	drm_drv.c \
	drm_dsc.c \
	drm_dumb_buffers.c \
	drm_edid.c \
	drm_encoder.c \
	drm_encoder_slave.c \
	drm_fb_helper_freebsd.c \
	drm_file.c \
	drm_flip_work.c \
	drm_fourcc.c \
	drm_framebuffer.c \
	drm_gem.c \
	drm_gem_framebuffer_helper.c \
	drm_hashtab.c \
	drm_ioctl.c \
	drm_info.c \
	drm_irq.c \
	drm_lease.c \
	drm_memory.c \
	drm_mm.c \
	drm_modes.c \
	drm_mode_config.c \
	drm_mode_object.c \
	drm_modeset_lock.c \
	drm_modeset_helper.c \
	drm_os_freebsd.c \
	drm_panel.c \
	drm_pci.c \
	drm_plane.c \
	drm_plane_helper.c \
	drm_print.c \
	drm_prime.c \
	drm_probe_helper.c \
	drm_property.c \
	drm_rect.c \
	drm_self_refresh_helper.c \
	drm_scdc_helper.c \
	drm_syncobj.c \
	drm_sysctl_freebsd.c \
	drm_sysfs.c \
	drm_vblank.c \
	drm_vma_manager.c \
	drm_writeback.c \
	linux_fb.c \
	tainted_linux_fb.c \
	linux_hdmi.c

.if !empty(KCONFIG:MDRM_FBDEV_EMULATION)
SRCS+=	drm_fb_helper.c
.endif

.if !empty(KCONFIG:MAGP)
SRCS+=	drm_agpsupport.c
.endif

.if !empty(KCONFIG:MDRM_LEGACY)
SRCS+=	ati_pcigart.c \
	drm_legacy_misc.c \
	drm_bufs.c \
	drm_context.c \
	drm_dma.c \
	drm_scatter.c \
	drm_lock.c
.endif

.if !empty(KCONFIG:MCOMPAT)
SRCS+=	drm_ioc32.c
.endif

.if !empty(KCONFIG:MDEBUGFS)
SRCS_DEBUG=	\
		drm_debugfs.c \
		drm_debugfs_crc.c
.endif

.if !empty(KCONFIG:MDRM_LOAD_EDID_FIRMWARE)
SRCS+=	drm_edid_load.c
.endif

.if !empty(KCONFIG:MDRM_VM)
SRCS+=	drm_vm.c
.endif

.if !empty(KCONFIG:MDRM_DP_AUX_CHARDEV)
SRCS+=	drm_dp_aux_dev.c
.endif

.if !empty(KCONFIG:MDRM_MIPI_DSI)
SRCS+=	drm_mipi_dsi.c
.endif

.if !empty(KCONFIG:MDRM_PANEL_ORIENTATION_QUIRKS)
SRCS+=	drm_panel_orientation_quirks.c
.endif

# drm scheduler (moved from amdgpu)
SRCS+=	sched_main.c \
	sched_fence.c \
	sched_entity.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
.if exists(${SYSDIR}/compat/linuxkpi/common/linuxkpi.h)
CFLAGS+= -include ${SYSDIR}/compat/linuxkpi/common/linuxkpi.h
.endif
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include # fallback to dummy
CFLAGS+= -I${SRCDIR}
CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=drm_' -DDRM_SYSCTL_PARAM_PREFIX=_dri -DLINUXKPI_VERSION=50000
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
	opt_compat.h \
	opt_syscons.h

.if ${MACHINE_CPUARCH} == "powerpc"
CWARNFLAGS+=-Wno-cast-qual
.endif
CWARNFLAGS+= -Wno-pointer-sign -Wno-format

CWARNFLAGS.drm_ioc32.c+=	-Wno-address-of-packed-member

.include <bsd.kmod.mk>
