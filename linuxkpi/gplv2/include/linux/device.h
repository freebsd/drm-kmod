#ifndef	_LINUX_GPLV2_DEVICE_H_
#define	_LINUX_GPLV2_DEVICE_H_

#include_next <linux/device.h>

/* allows to add/remove a custom action to devres stack */
int devm_add_action(struct device *dev, void (*action)(void *), void *data);

static inline int devm_add_action_or_reset(struct device *dev,
					   void (*action)(void *), void *data)
{
	int ret;

	ret = devm_add_action(dev, action, data);
	if (ret)
		action(data);

	return ret;
}

#endif	/* _LINUX_DEVICE_H_ */
