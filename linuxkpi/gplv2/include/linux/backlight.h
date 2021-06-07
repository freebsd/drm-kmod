#ifndef _LINUX_GPLV2_BACKLIGHT_H_
#define _LINUX_GPLV2_BACKLIGHT_H_

#include_next <linux/device.h>

#include_next <linux/backlight.h>

static inline int backlight_enable(struct backlight_device *bd)
{
	if (!bd)
		return 0;

	bd->props.power = 0; // FB_BLANK_UNBLANK

	backlight_update_status(bd);
	return 1;
}

static inline int backlight_disable(struct backlight_device *bd)
{
	if (!bd)
		return 0;

	bd->props.power = 4; // FB_BLANK_POWERDOWN

	backlight_update_status(bd);
	return 1;
}

static inline struct backlight_device *
devm_of_find_backlight(struct device *dev)
{
	return NULL;
}

#endif /* _LINUX_GPLV2_BACKLIGHT_H_ */
