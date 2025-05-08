# Definition of LINUXKPI_VERSION
#
# This is used to track linuxkpi compatability with
# the source tree and external components such as
# nvidia-drm

LINUXKPI_VERSION_NUMBER=	60800

CFLAGS+=	-DLINUXKPI_VERSION=${LINUXKPI_VERSION_NUMBER}
