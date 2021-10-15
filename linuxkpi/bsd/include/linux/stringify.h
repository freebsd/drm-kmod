#ifndef	__BSD_LKPI_LINUX_STRINGIFY_H_
#define	__BSD_LKPI_LINUX_STRINGIFY_H_

#include <sys/param.h>
#if (__FreeBSD_version >= 1400015) || \
    ((__FreeBSD_version < 1400000) && (__FreeBSD_version >= 1300512))
#include_next <linux/stringify.h>
#endif

#endif	/* __BSD_LKPI_LINUX_STRINGIFY_H_ */
