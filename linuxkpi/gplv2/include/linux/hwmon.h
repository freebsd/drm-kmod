#ifndef _LINUX_HWMON_H_
#define _LINUX_HWMON_H_

struct device;
struct attribute_group;

struct device *hwmon_device_register(struct device *dev);
struct device *
hwmon_device_register_with_groups(struct device *dev, const char *name,
				  void *drvdata,
				  const struct attribute_group **groups);
struct device *
devm_hwmon_device_register_with_groups(struct device *dev, const char *name,
				       void *drvdata,
				       const struct attribute_group **groups);

void hwmon_device_unregister(struct device *dev);
void devm_hwmon_device_unregister(struct device *dev);

#endif
