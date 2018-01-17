#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <linux/device.h>
#include <linux/acpi.h>
#include <drm/drmP.h>

#include <drm/drm_crtc_helper.h>

MODULE_DEPEND(amdgpu, drmn, 1, 1, 1);
MODULE_DEPEND(amdgpu, agp, 1, 1, 1);
MODULE_DEPEND(amdgpu, linuxkpi, 1, 1, 1);
MODULE_DEPEND(amdgpu, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(amdgpu, firmware, 1, 1, 1);
#ifdef CONFIG_DEBUG_FS
MODULE_DEPEND(amdgpu, debugfs, 1, 1, 1);
#endif
