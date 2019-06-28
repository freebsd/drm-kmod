# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/i915

.PATH:	${SRCDIR} ${SRCDIR}/gt ${SRCDIR}/gem ${SRCDIR}/display ${SRCDIR}/oa

.include "../kconfig.mk"

KMOD=	i915kms
SRCS=	\
	i915_active.c \
	i915_cmd_parser.c \
	i915_drv.c \
	i915_gem.c \
	i915_gem_batch_pool.c \
	i915_gem_evict.c \
	i915_gem_fence_reg.c \
	i915_gem_gtt.c \
	i915_gem_render_state.c \
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
	intel_guc.c \
	intel_guc_ads.c \
	intel_guc_ct.c \
	intel_guc_fw.c \
	intel_guc_log.c \
	intel_guc_submission.c \
	intel_hdcp.c \
	intel_hotplug.c \
	intel_huc.c \
	intel_huc_fw.c \
	intel_opregion.c \
	intel_overlay.c \
	intel_pm.c \
	intel_psr.c \
	intel_quirks.c \
	intel_renderstate_gen6.c \
	intel_renderstate_gen7.c \
	intel_renderstate_gen8.c \
	intel_renderstate_gen9.c \
	intel_runtime_pm.c \
	intel_sideband.c \
	intel_sprite.c \
	intel_uc.c \
	intel_uc_fw.c \
	intel_uncore.c \
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
	vlv_dsi.c \
	vlv_dsi_pll.c

# gt/*
SRCS+=	intel_breadcrumbs.c \
	intel_context.c \
	intel_engine_cs.c \
	intel_engine_pm.c \
	intel_gt_pm.c \
	intel_hangcheck.c \
	intel_lrc.c \
	intel_mocs.c \
	intel_reset.c \
	intel_ringbuffer.c \
	intel_sseu.c \
	intel_timeline.c \
	intel_workarounds.c

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

CWARNFLAGS.dvo_ch7017.c=	-Wno-cast-qual
CWARNFLAGS.dvo_ch7xxx.c=	-Wno-cast-qual
CWARNFLAGS.dvo_ivch.c=		-Wno-cast-qual
CWARNFLAGS.dvo_ns2501.c=	-Wno-cast-qual
CWARNFLAGS.dvo_sil164.c=	-Wno-cast-qual
CWARNFLAGS.dvo_tfp410.c=	-Wno-cast-qual

CWARNFLAGS.intel_acpi.c=	-Wno-incompatible-pointer-types
CWARNFLAGS.intel_audio.c=	-Wno-cast-qual
CWARNFLAGS.intel_bios.c=	-Wno-cast-qual
CWARNFLAGS.intel_color.c=	-Wno-missing-prototypes
CWARNFLAGS.intel_csr.c=		-Wno-cast-qual
CWARNFLAGS.intel_dp.c=		-Wno-enum-conversion -Wno-format -Wno-unused
CWARNFLAGS.intel_ddi.c=		-Wno-enum-conversion -Wno-compare-distinct-pointer-types -Wno-unused
CWARNFLAGS.intel_display.c=	-Wno-tautological-pointer-compare -Wno-enum-conversion -Wno-cast-qual -Wno-incompatible-pointer-types-discards-qualifiers
CWARNFLAGS.intel_dpll_mgr.c=	-Wno-compare-distinct-pointer-types -Wno-missing-prototypes
CWARNFLAGS.intel_device_info.c=	-Wno-sometimes-uninitialized
CWARNFLAGS.intel_dsi_panel_vbt.c= -Wno-cast-qual
CWARNFLAGS.intel_engine_cs.c= -Wno-cast-qual
CWARNFLAGS.intel_fbdev.c=	-Wno-initializer-overrides
CWARNFLAGS.intel_guc_ct.c=	-Wno-address-of-packed-member # probably false positive
CWARNFLAGS.intel_guc_submission.c= -Wno-cast-qual -Wno-unused-function
CWARNFLAGS.intel_hdcp.c=	-Wno-missing-prototypes
CWARNFLAGS.intel_hdmi.c=	-Wno-enum-conversion
CWARNFLAGS.intel_hotplug.c=	-Wno-missing-prototypes
CWARNFLAGS.intel_lrc.c=		-Wno-cast-qual -Wno-unused-variable
CWARNFLAGS.intel_lvds.c=	-Wno-unused
CWARNFLAGS.intel_overlay.c=	-Wno-tautological-pointer-compare
CWARNFLAGS.intel_pm.c=		-Wno-cast-qual -Wno-incompatible-pointer-types-discards-qualifiers -Wno-unused-const-variable
CWARNFLAGS.intel_psr.c=		-Wno-cast-qual
CWARNFLAGS.intel_runtime_pm.c=		-Wno-cast-qual 
CWARNFLAGS.intel_sdvo.c=	-Wno-cast-qual -Wno-enum-conversion
CWARNFLAGS.intel_tv.c=		-Wno-unused

CWARNFLAGS.i915_cmd_parser.c= -Wno-cast-qual
CWARNFLAGS.i915_debugfs.c=	-Wno-unused-function -Wno-cast-qual -Wno-format -Wno-dangling-else
CWARNFLAGS.i915_drv.c=		-Wno-initializer-overrides -Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.i915_gem.c=		-Wno-tautological-pointer-compare -Wno-pointer-arith -Wno-cast-qual -Wno-int-conversion
CWARNFLAGS.i915_gem_batch_pool.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_context.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_dmabuf.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_evict.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_execbuffer.c= -Wno-tautological-pointer-compare -Wno-tautological-constant-out-of-range-compare -Wno-pointer-arith -Wno-cast-qual
CWARNFLAGS.i915_gem_fence_reg.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_gtt.c=	-Wno-tautological-pointer-compare -Wno-cast-qual
CWARNFLAGS.i915_gem_internal.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_render_state.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_request.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_shrinker.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_stolen.c= -Wno-cast-qual
CWARNFLAGS.i915_gem_tiling.c=	-Wno-tautological-pointer-compare -Wno-cast-qual
CWARNFLAGS.i915_gem_userptr.c= -Wno-cast-qual
CWARNFLAGS.i915_gpu_error.c= -Wno-cast-qual -Wno-unused-function
CWARNFLAGS.i915_pci.c=		-Wno-initializer-overrides -Wno-unused-variable
CWARNFLAGS.i915_sw_fence.c= -Wno-unused-function
CWARNFLAGS.i915_sysfs.c=	-Wno-unused-variable
