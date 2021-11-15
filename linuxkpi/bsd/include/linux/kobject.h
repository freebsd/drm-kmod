#ifndef	_BSD_LKPI_LINUX_KOBJECT_H_
#define	_BSD_LKPI_LINUX_KOBJECT_H_

#include_next <linux/kobject.h>

extern const struct sysfs_ops __kobj_sysfs_ops;
#define	kobj_sysfs_ops	__kobj_sysfs_ops

#endif	/* _BSD_LKPI_LINUX_KOBJECT_H_ */
