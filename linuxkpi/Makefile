.PATH:	${.CURDIR}/gplv2/src

KMOD=	linuxkpi_gplv2

SRCS=	linux_kmod_gplv2.c	\
	linux_acpi.c		\
	linux_async.c		\
	linux_backlight.c	\
	linux_compat.c		\
	linux_component.c	\
	linux_device.c		\
	linux_dmabuf.c		\
	linux_fence.c		\
	linux_firmware.c	\
	linux_fs.c		\
	linux_fs_attr.c		\
	linux_hrtimer.c		\
	linux_i2c.c		\
	linux_irq.c		\
	linux_mtrr.c		\
	linux_notifier.c	\
	linux_page.c		\
	linux_reservation.c	\
	linux_seq_file.c	\
	linux_sort.c		\
	linux_video.c		\
	interval_tree.c

SRCS+=	opt_acpi.h

SRCS+=	bus_if.h		\
	device_if.h		\
	pci_if.h		\
	vnode_if.h

CFLAGS+= -include ${.CURDIR}/../drm/drm_os_config.h
CFLAGS+= -I${.CURDIR}/../include
CFLAGS+= -I${.CURDIR}/dummy/include
CFLAGS+= -I${.CURDIR}/gplv2/include
CFLAGS+= -I${SRCTOP}/sys/compat/linuxkpi/common/include
CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'

.include <bsd.kmod.mk>
CWARNFLAGS+=	-Wno-pointer-arith
CWARNFLAGS.linux_acpi.c=       -Wno-cast-qual
