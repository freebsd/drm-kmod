# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/i915

.PATH:	${SRCDIR} ${SRCDIR}/gt ${SRCDIR}/gt/uc ${SRCDIR}/gem ${SRCDIR}/display ${SRCDIR}/oa

.include "../kconfig.mk"

KMOD=	i915kms
SRCS=	\
	i915_active.c \
	i915_buddy.c \
	i915_cmd_parser.c \
	i915_drv.c \
	i915_getparam.c \
	i915_gem.c \
	i915_gem_evict.c \
	i915_gem_fence_reg.c \
	i915_gem_gtt.c \
	i915_globals.c \
	i915_irq.c \
	i915_memcpy.c \
	i915_params.c \
	i915_pci.c \
	i915_query.c \
	i915_request.c \
	i915_scatterlist.c \
	i915_scheduler.c \
	i915_suspend.c \
	i915_sw_fence.c \
	i915_sw_fence_work.c \
	i915_switcheroo.c \
	i915_syncmap.c \
	i915_sysfs.c \
	i915_trace_points.c \
	i915_user_extensions.c \
	i915_vgpu.c \
	i915_vma.c \
	intel_acpi.c \
	intel_atomic.c \
	intel_atomic_plane.c \
	intel_audio.c \
	intel_bw.c \
	intel_bios.c \
	intel_cdclk.c \
	intel_color.c \
	intel_combo_phy.c \
	intel_connector.c \
	intel_csr.c \
	intel_device_info.c \
	intel_display.c \
	intel_display_power.c \
	intel_dpio_phy.c \
	intel_dpll_mgr.c \
	intel_fbc.c \
	intel_fifo_underrun.c \
	intel_freebsd.c \
	intel_frontbuffer.c \
	intel_hdcp.c \
	intel_hotplug.c \
	intel_opregion.c \
	intel_overlay.c \
	intel_pch.c \
	intel_pm.c \
	intel_psr.c \
	intel_quirks.c \
	intel_runtime_pm.c \
	intel_sideband.c \
	intel_sprite.c \
	intel_uncore.c \
	i915_utils.c \
	intel_wakeref.c \
	intel_wopcm.c

# display/*
SRCS+=	dvo_ch7017.c \
	dvo_ch7xxx.c \
	dvo_ivch.c \
	dvo_ns2501.c \
	dvo_sil164.c \
	dvo_tfp410.c \
	icl_dsi.c \
	intel_crt.c \
	intel_ddi.c \
	intel_dp.c \
	intel_dp_aux_backlight.c \
	intel_dp_link_training.c \
	intel_dp_mst.c \
	intel_dsi.c \
	intel_dsi_dcs_backlight.c \
	intel_dsi_vbt.c \
	intel_dsb.c \
	intel_dvo.c \
	intel_gmbus.c \
	intel_hdmi.c \
	intel_lspcon.c \
	intel_lvds.c \
	intel_panel.c \
	intel_sdvo.c \
	intel_tc.c \
	intel_tv.c \
	intel_vdsc.c \
	intel_vga.c \
	vlv_dsi.c \
	vlv_dsi_pll.c

# gt/*
SRCS+=	gen6_renderstate.c \
	gen7_renderstate.c \
	gen8_renderstate.c \
	gen9_renderstate.c \
	intel_breadcrumbs.c \
	intel_context.c \
	intel_engine_cs.c \
	intel_engine_heartbeat.c \
	intel_engine_pm.c \
	intel_engine_pool.c \
	intel_engine_user.c \
	intel_gt.c \
	intel_gt_irq.c \
	intel_gt_pm.c \
	intel_gt_pm_irq.c \
	intel_gt_requests.c \
	intel_lrc.c \
	intel_mocs.c \
	intel_renderstate.c \
	intel_reset.c \
	intel_ring.c \
	intel_rps.c \
	intel_sseu.c \
	intel_timeline.c \
	intel_workarounds.c

# gt/uc*
SRCS+=	intel_guc.c \
	intel_guc_ads.c \
	intel_guc_ct.c \
	intel_guc_fw.c \
	intel_guc_log.c \
	intel_guc_submission.c \
	intel_huc.c \
	intel_huc_fw.c \
	intel_uc.c \
	intel_uc_fw.c

# gem/*
SRCS+=	i915_gem_busy.c \
	i915_gem_clflush.c \
	i915_gem_context.c \
	i915_gem_domain.c \
	i915_gem_dmabuf.c \
	i915_gem_execbuffer.c \
	i915_gem_fence.c \
	i915_gem_internal.c \
	i915_gem_mman.c \
	i915_gem_object.c \
	i915_gem_pages.c \
	i915_gem_phys.c \
	i915_gem_pm.c \
	i915_gem_shmem.c \
	i915_gem_shrinker.c \
	i915_gem_stolen.c \
	i915_gem_throttle.c \
	i915_gem_tiling.c \
	i915_gem_userptr.c \
	i915_gem_wait.c

# oa/*
SRCS+=	i915_oa_bdw.c \
	i915_oa_cflgt3.c \
	i915_oa_glk.c \
	i915_oa_kblgt2.c \
	i915_oa_sklgt3.c \
	i915_oa_bxt.c \
	i915_oa_chv.c \
	i915_oa_hsw.c \
	i915_oa_kblgt3.c \
	i915_oa_sklgt4.c \
	i915_oa_cflgt2.c \
	i915_oa_cnl.c \
	i915_oa_icl.c \
	i915_oa_sklgt2.c

.if !empty(KCONFIG:MCOMPAT)
SRCS+=	i915_ioc32.c
.endif

.if !empty(KCONFIG:MDEBUGFS)
SRCS+==	i915_debugfs.c \
	i915_gemfs.c \
	intel_pipe_crc.c
.endif

.if !empty(KCONFIG:MDRM_FBDEV_EMULATION)
SRCS+=	intel_fbdev.c
.endif

.if !empty(KCONFIG:MDRM_I915_CAPTURE_ERROR)
SRCS+=	i915_gpu_error.c
.endif

# i915_perf.c         # This one opens a can of worms. Hold off for now.
# intel_lpe_audio.c   # Need platform and irq_chip support

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H} -I${SRCDIR} -I${SRCDIR}/display

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=i915_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD} -DLINUXKPI_VERSION=50000
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS	+=			\
	opt_acpi.h		\
	opt_compat.h		\
	opt_drm.h		\
	opt_syscons.h		\
	acpi_if.h		\
	bus_if.h		\
	fb_if.h			\
	device_if.h		\
	pci_if.h		\
	pci_iov_if.h		\
	vnode_if.h

.include <bsd.kmod.mk>

# Re-instate -Wno-cast-qual for all files
# Reason: More or less all files need it due to use of test_bit() macro
# error: cast from 'const unsigned long *' to 'volatile unsigned long *'
CWARNFLAGS += -Wno-pointer-arith -Wno-format -Wno-cast-qual

CWARNFLAGS.intel_acpi.c=	-Wno-incompatible-pointer-types
CWARNFLAGS.intel_audio.c=	-Wno-unused-function
CWARNFLAGS.intel_dp.c=		-Wno-unused-variable
CWARNFLAGS.intel_dpll_mgr.c=	-Wno-compare-distinct-pointer-types
CWARNFLAGS.intel_fbdev.c=	-Wno-initializer-overrides
CWARNFLAGS.intel_freebsd.c=	-Wno-unused-function
CWARNFLAGS.intel_pm.c=		-Wno-incompatible-pointer-types-discards-qualifiers
CWARNFLAGS.i915_pci.c=		-Wno-initializer-overrides
CWARNFLAGS.i915_active.c=	-Wno-unused-function
CWARNFLAGS.i915_sw_fence.c=	-Wno-unused-function
CWARNFLAGS.i915_vma.c=		-Wno-unused-function
