# $FreeBSD$

KMOD=	i915kms
SRCS=	dvo_ch7017.c \
	dvo_ch7xxx.c \
	dvo_ivch.c \
	dvo_ns2501.c \
	dvo_sil164.c \
	dvo_tfp410.c \
	i915_cmd_parser.c \
	i915_debugfs.c \
	i915_drv.c \
	i915_gem.c \
	i915_gem_batch_pool.c \
	i915_gem_context.c \
	i915_gem_dmabuf.c \
	i915_gem_evict.c \
	i915_gem_execbuffer.c \
	i915_gem_fence.c \
	i915_gem_gtt.c \
	i915_gem_render_state.c \
	i915_gem_request.c \
	i915_gem_stolen.c \
	i915_gem_shrinker.c \
	i915_gem_tiling.c \
	i915_gem_userptr.c \
	i915_gpu_error.c \
	i915_guc_submission.c \
	i915_memcpy.c \
	i915_params.c \
	i915_pci.c \
	i915_irq.c \
	i915_suspend.c \
	i915_sw_fence.c \
	i915_sysfs.c \
	i915_trace_points.c \
	i915_vgpu.c \
	intel_acpi.c \
	intel_atomic.c \
	intel_atomic_plane.c \
	intel_audio.c \
	intel_bios.c \
	intel_breadcrumbs.c \
	intel_color.c \
	intel_crt.c \
	intel_csr.c \
	intel_ddi.c \
	intel_device_info.c \
	intel_display.c \
	intel_dp.c \
	intel_dp_aux_backlight.c \
	intel_dp_link_training.c \
	intel_dp_mst.c \
	intel_dpio_phy.c \
	intel_dpll_mgr.c \
	intel_dsi.c \
	intel_dsi_dcs_backlight.c \
	intel_dsi_panel_vbt.c \
	intel_dsi_pll.c \
	intel_dvo.c \
	intel_fbc.c \
	intel_engine_cs.c \
	intel_fbdev.c \
	intel_fifo_underrun.c \
	intel_freebsd.c \
	intel_frontbuffer.c \
	intel_guc_loader.c \
	intel_hdmi.c \
	intel_hotplug.c \
	intel_i2c.c \
	intel_lrc.c \
	intel_lvds.c \
	intel_modes.c \
	intel_mocs.c \
	intel_opregion.c \
	intel_overlay.c \
	intel_panel.c \
	intel_pm.c \
	intel_psr.c \
	intel_renderstate_gen6.c \
	intel_renderstate_gen7.c \
	intel_renderstate_gen8.c \
	intel_renderstate_gen9.c \
	intel_ringbuffer.c \
	intel_runtime_pm.c \
	intel_sdvo.c \
	intel_sideband.c \
	intel_sprite.c \
	intel_tv.c \
	intel_uncore.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${.CURDIR:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= -include ${.CURDIR:H}/drm/drm_os_config.h

.if ${MACHINE_CPUARCH} == "amd64"
SRCS+= i915_ioc32.c
.endif

SRCS	+=								\
	opt_acpi.h							\
	opt_compat.h							\
	opt_drm.h							\
	opt_syscons.h							\
	acpi_if.h							\
	bus_if.h							\
	fb_if.h								\
	device_if.h							\
	pci_if.h							\
	vnode_if.h

.include <bsd.kmod.mk>
CWARNFLAGS += -Wno-pointer-arith -Wno-format

CWARNFLAGS.intel_audio.c=	-Wno-cast-qual
CWARNFLAGS.intel_bios.c=	-Wno-cast-qual
CWARNFLAGS.intel_csr.c=		-Wno-cast-qual
CWARNFLAGS.intel_dp.c=		-Wno-enum-conversion -Wno-format -Wno-unused
CWARNFLAGS.intel_ddi.c=		-Wno-compare-distinct-pointer-types		
CWARNFLAGS.intel_display.c=	-Wno-tautological-pointer-compare -Wno-enum-conversion -Wno-cast-qual -Wno-incompatible-pointer-types-discards-qualifiers
CWARNFLAGS.intel_dpll_mgr.c=	-Wno-compare-distinct-pointer-types
CWARNFLAGS.intel_dsi_panel_vbt.c= -Wno-cast-qual
CWARNFLAGS.intel_engine_cs.c= -Wno-cast-qual
CWARNFLAGS.intel_guc_loader.c= 	-Wno-cast-qual
CWARNFLAGS.intel_hdmi.c=	-Wno-enum-conversion
CWARNFLAGS.intel_hotplug.c=	-Wno-missing-prototypes
CWARNFLAGS.intel_lrc.c=		-Wno-cast-qual
CWARNFLAGS.intel_lvds.c=	-Wno-unused
CWARNFLAGS.intel_overlay.c=	-Wno-tautological-pointer-compare
CWARNFLAGS.intel_pm.c=		-Wno-cast-qual -Wno-incompatible-pointer-types-discards-qualifiers -Wno-unused-const-variable
CWARNFLAGS.intel_psr.c=		-Wno-cast-qual
CWARNFLAGS.intel_runtime_pm.c=		-Wno-cast-qual 
CWARNFLAGS.intel_sdvo.c=	-Wno-cast-qual -Wno-enum-conversion
CWARNFLAGS.intel_tv.c=		-Wno-unused
CWARNFLAGS.i915_debugfs.c=	-Wno-unused-function -Wno-cast-qual -Wno-format -Wno-dangling-else
CWARNFLAGS.i915_gem.c=		-Wno-tautological-pointer-compare -Wno-pointer-arith -Wno-cast-qual -Wno-int-conversion
CWARNFLAGS.i915_drv.c=		-Wno-initializer-overrides -Wno-cast-qual -Wno-missing-prototypes
CWARNFLAGS.i915_gem_gtt.c=	-Wno-tautological-pointer-compare
CWARNFLAGS.i915_gem_tiling.c=	-Wno-tautological-pointer-compare
CWARNFLAGS.i915_gem_execbuffer.c=	-Wno-tautological-pointer-compare  -Wno-pointer-arith
CWARNFLAGS.i915_pci.c=		-Wno-initializer-overrides 
