#ifndef	__GPLV2_LINUX_CPU_H
#define	__GPLV2_LINUX_CPU_H
#include <sys/param.h>
#if (__FreeBSD_version >= 1400015) || \
    ((__FreeBSD_version < 1400000) && (__FreeBSD_version >= 1300512))
#include_next <linux/cpu.h>
#endif
#endif
