# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/i915

.PATH:	${SRCDIR} \
	${SRCDIR}/display \
	${SRCDIR}/gem \
	${SRCDIR}/gt \
	${SRCDIR}/gt/uc \
	${SRCDIR}/oa \
	${SRCDIR}/pxp \
	${SRCDIR}/soc

.include "../kconfig.mk"
.include "../linuxkpi_version.mk"

KMOD=	i915kms
SRCS=	\
	i915_active.c \
	i915_cmd_parser.c \
	i915_config.c \
	i915_deps.c \
	i915_driver.c \
	i915_drm_client.c \
	i915_getparam.c \
	i915_gem.c \
	i915_gem_evict.c \
	i915_gem_gtt.c \
	i915_gem_ww.c \
	i915_ioctl.c \
	i915_irq.c \
	i915_memcpy.c \
	i915_mitigations.c \
	i915_mm.c \
	i915_module.c \
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
	i915_user_extensions.c \
	i915_vgpu.c \
	i915_ttm_buddy_manager.c \
	i915_vma.c \
	i915_vma_resource.c \
	intel_acpi.c \
	intel_atomic.c \
	intel_atomic_plane.c \
	intel_audio.c \
	intel_bw.c \
	intel_bios.c \
	intel_cdclk.c \
	intel_clock_gating.c \
	intel_color.c \
	intel_combo_phy.c \
	intel_connector.c \
	intel_crtc.c \
	intel_cursor.c \
	intel_device_info.c \
	intel_display.c \
	intel_display_power.c \
	intel_dmc.c \
	intel_dpio_phy.c \
	intel_dpll.c \
	intel_dpll_mgr.c \
	intel_fbc.c \
	intel_fdi.c \
	intel_fifo_underrun.c \
	intel_freebsd.c \
	intel_frontbuffer.c \
	intel_hdcp.c \
	intel_hotplug.c \
	intel_memory_region.c \
	intel_opregion.c \
	intel_overlay.c \
	intel_pcode.c \
	intel_psr.c \
	intel_quirks.c \
	intel_region_ttm.c \
	intel_runtime_pm.c \
	intel_sbi.c \
	intel_snps_phy.c \
	intel_step.c \
	intel_sprite.c \
	intel_uncore.c \
	i915_utils.c \
	intel_wakeref.c \
	vlv_sideband.c \
	vlv_suspend.c

# display/*
SRCS+=	dvo_ch7017.c \
	dvo_ch7xxx.c \
	dvo_ivch.c \
	dvo_ns2501.c \
	dvo_sil164.c \
	dvo_tfp410.c \
	g4x_dp.c \
	g4x_hdmi.c \
	hsw_ips.c \
	i9xx_plane.c \
	i9xx_wm.c \
	icl_dsi.c \
	intel_backlight.c \
	intel_crt.c \
	intel_crtc_state_dump.c \
	intel_cx0_phy.c \
	intel_ddi.c \
	intel_ddi_buf_trans.c \
	intel_display_device.c \
	intel_display_driver.c \
	intel_display_irq.c \
	intel_display_params.c \
	intel_display_power_map.c \
	intel_display_power_well.c \
	intel_display_reset.c \
	intel_display_rps.c \
	intel_display_wa.c \
	intel_dkl_phy.c \
	intel_dp.c \
	intel_dp_aux.c \
	intel_dp_aux_backlight.c \
	intel_dp_hdcp.c \
	intel_dp_link_training.c \
	intel_dp_mst.c \
	intel_dpt.c \
	intel_dpt_common.c \
	intel_drrs.c \
	intel_dsi.c \
	intel_dsi_dcs_backlight.c \
	intel_dsi_vbt.c \
	intel_dsb.c \
	intel_dsb_buffer.c \
	intel_fb.c \
	intel_fb_bo.c \
	intel_fb_pin.c \
	intel_dvo.c \
	intel_global_state.c \
	intel_gmbus.c \
	intel_hdcp_gsc.c \
	intel_hdcp_gsc_message.c \
	intel_hdmi.c \
	intel_hotplug_irq.c \
	intel_hti.c \
	intel_link_bw.c \
	intel_load_detect.c \
	intel_lspcon.c \
	intel_lvds.c \
	intel_modeset_lock.c \
	intel_modeset_setup.c \
	intel_modeset_verify.c \
	intel_panel.c \
	intel_pch_display.c \
	intel_pch_refclk.c \
	intel_plane_initial.c \
	intel_pmdemand.c \
	intel_pps.c \
	intel_qp_tables.c \
	intel_sdvo.c \
	intel_sprite_uapi.c \
	intel_tc.c \
	intel_tv.c \
	intel_vblank.c \
	intel_vdsc.c \
	intel_vrr.c \
	intel_vga.c \
	intel_wm.c \
	skl_scaler.c \
	skl_universal_plane.c \
	skl_watermark.c \
	vlv_dsi.c \
	vlv_dsi_pll.c

# gt/*
SRCS+=	gen2_engine_cs.c \
	gen6_engine_cs.c \
	gen6_ppgtt.c \
	gen6_renderstate.c \
	gen7_renderclear.c \
	gen7_renderstate.c \
	gen8_engine_cs.c \
	gen8_ppgtt.c \
	gen8_renderstate.c \
	gen9_renderstate.c \
	intel_breadcrumbs.c \
	intel_context.c \
	intel_context_sseu.c \
	intel_engine_cs.c \
	intel_engine_heartbeat.c \
	intel_engine_pm.c \
	intel_engine_user.c \
	intel_execlists_submission.c \
	intel_ggtt.c \
	intel_ggtt_fencing.c \
	intel_ggtt_gmch.c \
	intel_gsc.c \
	intel_gt.c \
	intel_gt_buffer_pool.c \
	intel_gt_clock_utils.c \
	intel_gt_irq.c \
	intel_gt_mcr.c \
	intel_gt_pm.c \
	intel_gt_pm_irq.c \
	intel_gt_requests.c \
	intel_gt_sysfs.c \
	intel_gt_sysfs_pm.c \
	intel_gtt.c \
	intel_guc_rc.c \
	intel_guc_slpc.c \
	intel_llc.c \
	intel_lrc.c \
	intel_migrate.c \
	intel_mocs.c \
	intel_ppgtt.c \
	intel_rc6.c \
	intel_region_lmem.c \
	intel_renderstate.c \
	intel_reset.c \
	intel_ring.c \
	intel_ring_submission.c \
	intel_rps.c \
	intel_sa_media.c \
	intel_sseu.c \
	intel_timeline.c \
	intel_tlb.c \
	intel_wopcm.c \
	intel_workarounds.c \
	shmem_utils.c \
	sysfs_engines.c

# gt/uc*
SRCS+=	intel_gsc_fw.c \
	intel_gsc_proxy.c \
	intel_gsc_uc.c \
	intel_gsc_uc_debugfs.c \
	intel_gsc_uc_heci_cmd_submit.c \
	intel_guc.c \
	intel_guc_ads.c \
	intel_guc_capture.c \
	intel_guc_ct.c \
	intel_guc_fw.c \
	intel_guc_hwconfig.c \
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
	i915_gem_create.c \
	i915_gem_domain.c \
	i915_gem_dmabuf.c \
	i915_gem_execbuffer.c \
	i915_gem_internal.c \
	i915_gem_lmem.c \
	i915_gem_mman.c \
	i915_gem_object.c \
	i915_gem_pages.c \
	i915_gem_phys.c \
	i915_gem_pm.c \
	i915_gem_region.c \
	i915_gem_shmem.c \
	i915_gem_shrinker.c \
	i915_gem_stolen.c \
	i915_gem_throttle.c \
	i915_gem_tiling.c \
	i915_gem_ttm.c \
	i915_gem_ttm_move.c \
	i915_gem_ttm_pm.c \
	i915_gem_userptr.c \
	i915_gem_wait.c

# pxp/*
.if !empty(KCONFIG:MDRM_I915_PXP)
SRCS+=	intel_pxp.c \
	intel_pxp_cmd.c \
	intel_pxp_debugfs.c \
	intel_pxp_gsccs.c \
	intel_pxp_huc.c \
	intel_pxp_irq.c \
	intel_pxp_pm.c \
	intel_pxp_session.c \
	intel_pxp_tee.c
.endif

# soc/*
SRCS+=	intel_dram.c \
	intel_gmch.c \
	intel_pch.c

.if !empty(KCONFIG:MCOMPAT)
SRCS+=	i915_ioc32.c
.endif

.if !empty(KCONFIG:MDEBUG_FS)
SRCS+=	i915_debugfs.c \
	i915_debugfs_params.c \
	intel_display_debugfs.c \
	intel_display_debugfs_params.c \
	intel_gt_debugfs.c \
	intel_gt_engines_debugfs.c \
	intel_gt_pm_debugfs.c \
	intel_guc_debugfs.c \
	intel_guc_log_debugfs.c \
	intel_huc_debugfs.c \
	intel_pipe_crc.c \
	intel_sseu_debugfs.c \
	intel_uc_debugfs.c
.endif

.if !empty(KCONFIG:MDRM_FBDEV_EMULATION)
SRCS+=	intel_fbdev.c \
	intel_fbdev_fb.c
.endif

.if !empty(KCONFIG:MDRM_I915_CAPTURE_ERROR)
SRCS+=	i915_gpu_error.c
.endif

.if !empty(KCONFIG:MHWMON)
SRCS+=	i915_hwmon.c
.endif

# i915_perf.c         # This one opens a can of worms. Hold off for now.
# intel_lpe_audio.c   # Need platform and irq_chip support

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/bsd/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H} -I${SRCDIR} -I${SRCDIR}/display

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=i915_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD}
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}
CFLAGS+= -DI915

SRCS	+=			\
	opt_acpi.h		\
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
CWARNFLAGS.intel_backlight.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_combo_phy.c=	-Wno-uninitialized
CWARNFLAGS.intel_ddi.c=		-Wno-unused-but-set-variable -Wno-missing-prototypes
CWARNFLAGS.intel_dp.c=		-Wno-unused-variable -Wno-sometimes-uninitialized -Wno-constant-logical-operand
CWARNFLAGS.intel_dp_aux_backlight.c=	-Wno-shift-count-overflow
CWARNFLAGS.intel_dpll_mgr.c=	-Wno-compare-distinct-pointer-types
CWARNFLAGS.intel_display.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_display_device.c=	-Wno-initializer-overrides
CWARNFLAGS.intel_display_irq.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_dpll.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_fb.c=		-Wno-unused-but-set-variable
CWARNFLAGS.intel_fbc.c=		-Wno-unused-variable
CWARNFLAGS.intel_fbdev.c=	-Wno-initializer-overrides
CWARNFLAGS.intel_freebsd.c=	-Wno-unused-function
CWARNFLAGS.i915_gem_context.c=	-Wno-unused-function
CWARNFLAGS.intel_ggtt.c=	-Wno-integer-overflow
CWARNFLAGS.intel_gmch.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_gt_mcr.c=	-Wno-tautological-constant-out-of-range-compare
CWARNFLAGS.intel_guc_capture.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_guc_submission.c=	-Wno-unused-function
CWARNFLAGS.intel_hdmi.c=	-Wno-unused-but-set-variable -Wno-unused-variable
CWARNFLAGS.intel_hotplug.c=	-Wno-uninitialized
CWARNFLAGS.intel_psr.c=		-Wno-unused-but-set-variable
CWARNFLAGS.intel_region_ttm.c=	-Wno-missing-prototypes
CWARNFLAGS.intel_reset.c=	-Wno-unused-but-set-variable
CWARNFLAGS.intel_sseu.c=	-Wno-incompatible-pointer-types-discards-qualifiers
CWARNFLAGS.intel_workarounds.c=	-Wno-unused-variable
CWARNFLAGS.i915_debugfs.c=	-Wno-unused-but-set-variable
CWARNFLAGS.i915_driver.c=	-Wno-unused-but-set-variable
CWARNFLAGS.i915_irq.c=		-Wno-unused-but-set-variable
CWARNFLAGS.i915_gem_execbuffer.c=	-Wno-unused-but-set-variable -Wno-sometimes-uninitialized
CWARNFLAGS.i915_gem_mman.c=	-Wno-unused-but-set-variable
CWARNFLAGS.i915_gem_shmem.c=	-Wno-unused-but-set-variable
CWARNFLAGS.i915_gem_ttm_move.c=	-Wno-unused-variable
CWARNFLAGS.i915_pci.c=		-Wno-initializer-overrides
CWARNFLAGS.i915_active.c=	-Wno-unused-function
CWARNFLAGS.i915_sw_fence.c=	-Wno-unused-function
CWARNFLAGS.i915_vma.c=		-Wno-unused-function -Wno-unused-but-set-variable
CWARNFLAGS.i915_vma_resource.c=	-Wno-incompatible-pointer-types
CWARNFLAGS.i9xx_plane.c=	-Wno-unused-but-set-variable
CWARNFLAGS.g4x_dp.c=		-Wno-shift-count-overflow
CWARNFLAGS.skl_universal_plane.c=	-Wno-unused-but-set-variable
CWARNFLAGS.vlv_dsi.c=		-Wno-unused-but-set-variable

EXPORT_SYMS=	YES
