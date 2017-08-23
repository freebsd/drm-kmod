#ifndef _LINUX__PLATFORM_DEVICE_H_
#define	_LINUX__PLATFORM_DEVICE_H_

#include <linux/device.h>

struct platform_device {
	struct device dev;
};

static inline void
platform_device_unregister(struct platform_device *pdev)
{

	panic("%s: unimplemented", __func__);
}
	
static inline struct platform_device *
platform_device_register_simple(const char *name, int id,
    void *res, unsigned int num)
{

	panic("%s: unimplemented", __func__);
}

#endif /* _LINUX__PLATFORM_DEVICE_H_ */
