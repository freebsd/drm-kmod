#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <linux/device.h>
#include <linux/acpi.h>
#include <drm/drmP.h>

#include <drm/drm_crtc_helper.h>

MODULE_DEPEND(radeonkms, drmn, 1, 1, 1);
MODULE_DEPEND(radeonkms, agp, 1, 1, 1);
MODULE_DEPEND(radeonkms, linuxkpi, 1, 1, 1);
MODULE_DEPEND(radeonkms, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(radeonkms, firmware, 1, 1, 1);
#ifdef CONFIG_DEBUG_FS
MODULE_DEPEND(radeonkms, debugfs, 1, 1, 1);
#endif
