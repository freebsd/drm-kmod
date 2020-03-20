.PATH:	${.CURDIR}/gplv2/src

KMOD=	linuxkpi_gplv2

SRCS=	linux_kmod_gplv2.c	\
	linux_anon_inodefs.c 	\
	linux_anon_inodes.c 	\
	linux_async.c		\
	linux_backlight.c	\
	linux_compat.c		\
	linux_component.c	\
	linux_device.c		\
	linux_dmabuf.c		\
	linux_dmafence.c	\
	linux_dmi.c		\
	linux_firmware.c	\
	linux_fs.c		\
	linux_fs_attr.c		\
	linux_genalloc.c	\
	linux_i2c.c		\
	linux_interval_tree.c	\
	linux_irq.c		\
	linux_llist.c		\
	linux_mtrr.c		\
	linux_notifier.c	\
	linux_page.c		\
	linux_rbtree.c		\
	linux_relay.c		\
	linux_reservation.c	\
	linux_sort.c		\
	linux_sync_file.c


.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "aarch64"
SRCS+=	linux_acpi.c	\
	linux_video.c 	\
	opt_acpi.h
.endif

SRCS+=	bus_if.h		\
	device_if.h		\
	pci_if.h		\
	pci_iov_if.h		\
	vnode_if.h



CFLAGS+= -include ${.CURDIR:H}/drivers/gpu/drm/drm_os_config.h
CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR}/dummy/include
CFLAGS+= -I${.CURDIR}/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS.gcc+= -Wno-cast-qual

.include <bsd.kmod.mk>
CWARNFLAGS+=	-Wno-pointer-arith
CWARNFLAGS.linux_acpi.c=       -Wno-cast-qual

