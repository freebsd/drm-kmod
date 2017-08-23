#ifndef _LINUX_HWMON_SYSFS_H
#define _LINUX_HWMON_SYSFS_H

#include <linux/device.h>

struct sensor_device_attribute{
	struct device_attribute dev_attr;
	int index;
};
#define to_sensor_dev_attr(_dev_attr) \
	container_of(_dev_attr, struct sensor_device_attribute, dev_attr)

#define SENSOR_ATTR(_name, _mode, _show, _store, _index)	\
	{ .dev_attr = __ATTR(_name, _mode, _show, _store),	\
	  .index = _index }

#define SENSOR_DEVICE_ATTR(_name, _mode, _show, _store, _index)	\
struct sensor_device_attribute sensor_dev_attr_##_name		\
	= SENSOR_ATTR(_name, _mode, _show, _store, _index)
#endif
