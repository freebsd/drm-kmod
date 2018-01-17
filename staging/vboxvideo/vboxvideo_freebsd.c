#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/module.h>



MODULE_DEPEND(vboxvideo, drmn, 2, 2, 2);
MODULE_DEPEND(vboxvideo, agp, 1, 1, 1);
MODULE_DEPEND(vboxvideo, linuxkpi, 1, 1, 1);
MODULE_DEPEND(vboxvideo, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(vboxvideo, debugfs, 1, 1, 1);
