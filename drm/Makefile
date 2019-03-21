# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm

.PATH:	${SRCDIR} ${SRCDIR}/scheduler

KMOD=	drm
SRCS=	ati_pcigart.c \
	drm_agpsupport.c \
	drm_atomic.c \
	drm_atomic_helper.c \
	drm_auth.c \
	drm_blend.c \
	drm_bufs.c \
	drm_bridge.c \
	drm_cache.c \
	drm_color_mgmt.c \
	drm_connector.c \
	drm_context.c \
	drm_crtc.c \
	drm_crtc_helper.c \
	drm_debugfs.c \
	drm_debugfs_crc.c \
	drm_dma.c \
	drm_dp_aux_dev.c \
	drm_dp_dual_mode_helper.c \
	drm_dp_helper.c \
	drm_dp_mst_topology.c \
	drm_drv.c \
	drm_dumb_buffers.c \
	drm_edid.c \
	drm_edid_load.c \
	drm_encoder.c \
	drm_encoder_slave.c \
	drm_fb_helper.c \
	drm_fb_helper_freebsd.c \
	drm_file.c \
	drm_flip_work.c \
	drm_fourcc.c \
	drm_framebuffer.c \
	drm_gem.c \
	drm_gem_cma_helper.c \
	drm_global.c \
	drm_hashtab.c \
	drm_ioc32.c \
	drm_ioctl.c \
	drm_info.c \
	drm_irq.c \
	drm_kms_helper_common.c \
	drm_lease.c \
	drm_lock.c \
	drm_memory.c \
	drm_mipi_dsi.c \
	drm_mm.c \
	drm_modes.c \
	drm_mode_config.c \
	drm_mode_object.c \
	drm_modeset_lock.c \
	drm_modeset_helper.c \
	drm_os_freebsd.c \
	drm_panel.c \
	drm_panel_orientation_quirks.c \
	drm_pci.c \
	drm_plane.c \
	drm_plane_helper.c \
	drm_print.c \
	drm_prime.c \
	drm_probe_helper.c \
	drm_property.c \
	drm_rect.c \
	drm_scatter.c \
	drm_scdc_helper.c \
	drm_syncobj.c \
	drm_sysctl_freebsd.c \
	drm_sysfs.c \
	drm_vblank.c \
	drm_vm.c \
	drm_vma_manager.c \
	linux_fb.c \
	tainted_linux_fb.c \
	linux_hdmi.c

# drm scheduler (moved from amdgpu)
SRCS+=	gpu_scheduler.c \
	sched_fence.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include # fallback to dummy

CFLAGS+= -I${SRCDIR}
CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${SRCDIR}/drm_os_config.h

SRCS+=	device_if.h \
	vnode_if.h \
	bus_if.h \
	pci_if.h \
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

CWARNFLAGS.drm_mipi_dsi.c+=	-Wno-incompatible-pointer-types-discards-qualifiers -Wno-int-conversion
CWARNFLAGS.drm_cache.c+=	-Wno-int-conversion
CWARNFLAGS.drm_info.c+=		-Wno-tautological-compare
CWARNFLAGS.drm_ioc32.c+=	-Wno-address-of-packed-member
CWARNFLAGS.drm_debugfs.c+=	-Wno-format-security
CWARNFLAGS.drm_scatter.c+=	-Wno-int-conversion
.include <bsd.kmod.mk>
