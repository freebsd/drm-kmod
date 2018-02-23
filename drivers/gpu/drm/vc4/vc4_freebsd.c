#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/module.h>


MODULE_DEPEND(vc4, drmn, 2, 2, 2);
MODULE_DEPEND(vc4, agp, 1, 1, 1);
MODULE_DEPEND(vc4, linuxkpi, 1, 1, 1);
MODULE_DEPEND(vc4, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(vc4, debugfs, 1, 1, 1);
