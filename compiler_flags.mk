# To set some flags, we need to know the type and the version of the compiler.
# If these variables are not set, figure out their value here. This code was
# copied from `Mk/Uses/compiler.mk` from the FreeBSD ports tree.
.if !defined(COMPILER_TYPE)
_CCVERSION!=	${CC} --version
COMPILER_VERSION=	${_CCVERSION:M[0-9]*.[0-9]*:[1]:C/([0-9]+)\.([0-9]+)\..*/\1\2/}
.  if ${_CCVERSION:Mclang}
COMPILER_TYPE=	clang
.  else
COMPILER_TYPE=	gcc
.  endif
.endif

# Linux' Makefile (scripts/Makefile.lib) always include
# <linux/compiler_types.h> from the compiler command line.
#
# At least `drivers/gpu/drm/drm_atomic_uapi.c` and
# `include/drm/drm_atomic_uap.h` depends on this to compile.
CFLAGS+=	-include ${SYSDIR}/compat/linuxkpi/common/include/linux/compiler_types.h

# Globally silence a new warning from Clang 21.
# See https://lkml.org/lkml/2025/5/6/1681.
.if ${COMPILER_TYPE} == clang && ${COMPILER_VERSION} >= 211
CWARNFLAGS+=	-Wno-default-const-init-var-unsafe
.endif
