#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/module.h>

MODULE_DEPEND(radeonkms, drmn, 2, 2, 2);
MODULE_DEPEND(radeonkms, ttm, 1, 1, 1);
MODULE_DEPEND(radeonkms, linuxkpi, 1, 1, 1);
MODULE_DEPEND(radeonkms, linuxkpi_video, 1, 1, 1);
MODULE_DEPEND(radeonkms, dmabuf, 1, 1, 1);
MODULE_DEPEND(radeonkms, firmware, 1, 1, 1);
#ifdef CONFIG_DEBUG_FS
MODULE_DEPEND(radeonkms, lindebugfs, 1, 1, 1);
#endif
