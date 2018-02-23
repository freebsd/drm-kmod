#ifndef DRM_OS_CONFIG_H_
#define DRM_OS_CONFIG_H_

#define CONFIG_DEBUG_FS 1
#define COMPAT_FREEBSD32 1
#ifdef COMPAT_FREEBSD32
#define CONFIG_COMPAT 1
#endif
#ifdef notyet
#define CONFIG_MMU_NOTIFIER 1
#endif
#ifdef __i386__
#define	CONFIG_X86	1
#endif
#ifdef __amd64__
#define	CONFIG_X86	1
#define	CONFIG_X86_64	1
#define CONFIG_64BIT	1
#endif
#ifdef __ia64__
#define	CONFIG_IA64	1
#define CONFIG_64BIT	1
#endif

#if defined(__i386__) || defined(__amd64__)
#define CONFIG_PCI 1
#define	CONFIG_ACPI 1
#define	CONFIG_ACPI_SLEEP 1
#define	CONFIG_DRM_I915_KMS 1
#undef	CONFIG_INTEL_IOMMU
#endif

// For platforms with SSE4.1 (needed for GuC logging)
/* #define CONFIG_AS_MOVNTDQA */

#define	CONFIG_AGP	1
#define	CONFIG_MTRR	1

#define	CONFIG_FB	1

#undef	CONFIG_VGA_CONSOLE

#define CONFIG_BACKLIGHT_CLASS_DEVICE 1

#define CONFIG_DRM_DP_AUX_CHARDEV 1

#define CONFIG_SMP 1

#define CONFIG_PM 1

#define CONFIG_DRM_LOAD_EDID_FIRMWARE 1

// for i915_error_printf function declaration in i915_drv.h
#define CONFIG_DRM_I915_CAPTURE_ERROR 1

#define CONFIG_DRM_I915_ALPHA_SUPPORT 1

#define CONFIG_DRM_AMD_POWERPLAY 1


// Linux Makefile drm/amd/amdgpu/Makefile:
// amdgpu-$(CONFIG_DRM_AMDGPU_SI)+= si.o gmc_v6_0.o gfx_v6_0.o si_ih.o
//                                  si_dma.o dce_v6_0.o si_dpm.o si_smc.o
// Since we always build those files, define here to avoid build error
#define CONFIG_DRM_AMDGPU_SI
#define CONFIG_DRM_AMDGPU_CIK
	 

// Let try to do without this CONFIG_LOCKDEP. Opens a can of worms.
// FreeBSD does some lock checking even without this macro.
// See $SRC/sys/compat/linuxkpi/common/include/linux/lockdep.h
// For the functions that we implement, override IS_ENABLED(CONFIG_LOCKDEP)
// by using #if IS_ENABLED(CONFIG_LOCKDEP) || defined(__FreeBSD__) in
// drm drivers
//#define CONFIG_LOCKDEP 1


// Uncomment this or remove #ifdefs in i915_drv.c when enabling i915_perf
//#define CONFIG_I915_PERF 

#endif
