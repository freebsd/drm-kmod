#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/module.h>

MODULE_DEPEND(amdgpu, drmn, 2, 2, 2);
MODULE_DEPEND(amdgpu, ttm, 1, 1, 1);
MODULE_DEPEND(amdgpu, linuxkpi, 1, 1, 1);
MODULE_DEPEND(amdgpu, linuxkpi_video, 1, 1, 1);
MODULE_DEPEND(amdgpu, dmabuf, 1, 1, 1);
MODULE_DEPEND(amdgpu, firmware, 1, 1, 1);
#ifdef CONFIG_DEBUG_FS
MODULE_DEPEND(amdgpu, lindebugfs, 1, 1, 1);
#endif
