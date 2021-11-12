#ifndef	_BSD_LKPI_LINUX_DEVICE_H_
#define	_BSD_LKPI_LINUX_DEVICE_H_

#include_next <linux/device.h>

#include <linux/pm.h>
#include <linux/idr.h>
#include <linux/ratelimit.h>	/* via linux/dev_printk.h */

static inline const char *
__dev_driver_string(const struct device *dev)
{
	driver_t *drv;
	const char *str = "";

	if (dev->bsddev != NULL) {
		drv = device_get_driver(dev->bsddev);
		if (drv != NULL)
			str = drv->name;
	}

	return (str);
}
#define	dev_driver_string(dev)	__dev_driver_string(dev)

#endif /* _BSD_LKPI_LINUX_DEVICE_H_ */
