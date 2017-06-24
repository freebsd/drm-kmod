#ifndef _LINUX_OF_DEVICE_H
#define _LINUX_OF_DEVICE_H
#include <linux/of.h>
#include <linux/mod_devicetable.h>

struct device;
struct device_driver;
struct kobj_uevent_env;

static inline int of_driver_match_device(struct device *dev,
					 const struct device_driver *drv)
{
	return 0;
}

static inline void of_device_uevent(struct device *dev,
			struct kobj_uevent_env *env) { }

static inline const void *of_device_get_match_data(const struct device *dev)
{
	return NULL;
}

static inline int of_device_get_modalias(struct device *dev,
				   char *str, ssize_t len)
{
	return -ENODEV;
}

static inline int of_device_uevent_modalias(struct device *dev,
				   struct kobj_uevent_env *env)
{
	return -ENODEV;
}

static inline void of_device_node_put(struct device *dev) { }

static inline const struct of_device_id *__of_match_device(
		const struct of_device_id *matches, const struct device *dev)
{
	return NULL;
}
#define of_match_device(matches, dev)	\
	__of_match_device(of_match_ptr(matches), (dev))

static inline struct device_node *of_cpu_device_node_get(int cpu)
{
	return NULL;
}
static inline void of_dma_configure(struct device *dev, struct device_node *np)
{}
#endif
