# Definition of LINUXKPI_VERSION
#
# This is used to track linuxkpi compatability with
# the source tree and external components such as
# nvidia-drm

LINUXKPI_VERSION_NUMBER=	60900

CFLAGS+=	-DLINUXKPI_VERSION=${LINUXKPI_VERSION_NUMBER}

# Linux' Makefile (scripts/Makefile.lib) always include
# <linux/compiler_types.h> from the compiler command line.
#
# At least `drivers/gpu/drm/drm_atomic_uapi.c` and
# `include/drm/drm_atomic_uap.h` depends on this to compile.
CFLAGS+=	-include ${SYSDIR}/compat/linuxkpi/common/include/linux/compiler_types.h
