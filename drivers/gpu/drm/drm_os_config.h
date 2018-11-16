#ifndef DRM_OS_CONFIG_H_
#define DRM_OS_CONFIG_H_

#define CONFIG_DEBUG_FS 1
#ifdef __amd64__
#define COMPAT_FREEBSD32 1
#endif
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
// For platforms with SSE4.1 (needed for GuC)
#define CONFIG_AS_MOVNTDQA
#endif

#ifdef __powerpc64__
#define CONFIG_PPC64	1
#define CONFIG_64BIT	1
#define AIM		1
#define CONFIG_PCI 1
#undef CONFIG_ACPI
#undef CONFIG_ACPI_SLEEP
#undef CONFIG_DRM_I915_KMS
#undef CONFIG_INTEL_IOMMU
#undef CONFIG_AS_MOVNTDQA
#endif



#ifdef _KERNEL
#define	__KERNEL__
#endif

#if !defined(__powerpc__)
#define	CONFIG_AGP	1
#endif

#define	CONFIG_MTRR	1

#define	CONFIG_FB	1

#undef	CONFIG_VGA_CONSOLE

#define CONFIG_BACKLIGHT_CLASS_DEVICE 1

// Do we need this? Need to replace {read,write}_iter
// impl in drm_dp_aux_dev.c
#define CONFIG_DRM_DP_AUX_CHARDEV 1

#define CONFIG_SMP 1

#define CONFIG_PM 1

/*
 * DMI stands for "Desktop Management Interface".  It is part
 * of and an antecedent to, SMBIOS, which stands for System
 * Management BIOS.  See further: http://www.dmtf.org/standards
 */
#define CONFIG_DMI 1

#define CONFIG_DRM_LOAD_EDID_FIRMWARE 1

// for i915_error_printf function declaration in i915_drv.h
#define CONFIG_DRM_I915_CAPTURE_ERROR 1

/* #define	CONFIG_DRM_I915_DEBUG_GEM 1 */

#define CONFIG_DRM_I915_ALPHA_SUPPORT 1

#define CONFIG_DRM_AMD_POWERPLAY 1


// Enable amdgpu driver for older SI and CIK cards
// Not enabled by default in Linux v4.13
// Also require sysnobs compat.linuxkpi.{si,cik}_support=1
// to enable amdgpu on those GPUs
#define CONFIG_DRM_AMDGPU_SI
#define CONFIG_DRM_AMDGPU_CIK
	 

// Let try to do without this CONFIG_LOCKDEP. Opens a can of worms.
// FreeBSD does some lock checking even without this macro.
// See $SRC/sys/compat/linuxkpi/common/include/linux/lockdep.h
// For the functions that we implement, override IS_ENABLED(CONFIG_LOCKDEP)
// by using #if IS_ENABLED(CONFIG_LOCKDEP) || defined(__FreeBSD__) in
// drm drivers
//#define CONFIG_LOCKDEP 1


// Uncomment this or remove #ifdefs in source files when enabling i915_perf
//#define CONFIG_I915_PERF 

// Overallocation of the fbdev buffer
// Defines the fbdev buffer overallocation in percent. Default is 100.
// Typical values for double buffering will be 200, triple buffering 300.
#define CONFIG_DRM_FBDEV_OVERALLOC 100


// From v4.12 Intel start using this in intel_uncore.c
// Probably only used in Atom SOCs. Should be easy to port.
// Only include header in asm/iosf_mbi.h for now and keep disabled
//#define CONFIG_IOSF_MBI 1


// Enable new amd display controller
#define	CONFIG_DRM_AMD_DC 1
#ifdef __amd64__
#define	CONFIG_DRM_AMD_DC_DCN1_0 1
#endif

#define	CONFIG_DRM_VMWGFX_FBCON 1

#endif
