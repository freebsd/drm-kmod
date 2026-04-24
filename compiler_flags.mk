# Linux' Makefile (scripts/Makefile.lib) always include
# <linux/compiler_types.h> from the compiler command line.
#
# At least `drivers/gpu/drm/drm_atomic_uapi.c` and
# `include/drm/drm_atomic_uap.h` depends on this to compile.
CFLAGS+=	-include ${SYSDIR}/compat/linuxkpi/common/include/linux/compiler_types.h
