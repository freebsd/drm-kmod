#ifndef _BSD_LKPI_LINUX_COMPAT_H_
#define	_BSD_LKPI_LINUX_COMPAT_H_

#include_next <linux/compat.h>

#ifndef compat_ptr
#define	compat_ptr(x)	((void *)(unsigned long)x)
#endif
#ifndef ptr_to_compat
#define	ptr_to_compat(x)	((unsigned long)x)
#endif

#endif	/* _BSD_LKPI_LINUX_COMPAT_H_ */
