#ifndef _BSD_LKPI_LINUX_DEVICE_H_
#define	_BSD_LKPI_LINUX_DEVICE_H_

#include_next <linux/device.h>

/* This macro belongs to linux/printk.h in modern Linux */
#ifndef dev_info_once
#define dev_info_once(dev, ...) do {		\
	static bool __dev_info_once;		\
	if (!__dev_info_once) {			\
		__dev_info_once = 1;		\
		dev_info(dev, __VA_ARGS__);	\
	}					\
} while (0)
#endif

#endif	/* _BSD_LKPI_LINUX_DEVICE_H_ */
