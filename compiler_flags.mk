# Linux' Makefile (scripts/Makefile.lib) always include
# <linux/compiler_types.h> from the compiler command line.
#
# At least `drivers/gpu/drm/drm_atomic_uapi.c` and
# `include/drm/drm_atomic_uap.h` depends on this to compile.
CFLAGS+=	-include ${SYSDIR}/compat/linuxkpi/common/include/linux/compiler_types.h

CWARNFLAGS+=	-Wno-pointer-sign

# Globally silence a new warning from Clang 21.
# See https://lkml.org/lkml/2025/5/6/1681.
.if ${COMPILER_TYPE} == clang && ${COMPILER_VERSION} >= 210100
CWARNFLAGS+=	-Wno-default-const-init-var-unsafe
.endif
