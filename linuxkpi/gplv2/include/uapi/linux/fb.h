#ifndef _UAPI_LINUX_FB_H
#define _UAPI_LINUX_FB_H

#include <linux/types.h>
#include <linux/i2c.h>

/* Definitions of frame buffers                                         */

#define FB_MAX                  32      /* sufficient for now */


#ifdef CONFIG_FB_BACKLIGHT
/* Settings for the generic backlight code */
#define FB_BACKLIGHT_LEVELS     128
#define FB_BACKLIGHT_MAX        0xFF
#endif

/*
 * Display rotation support
 */
#define FB_ROTATE_UR      0
#define FB_ROTATE_CW      1
#define FB_ROTATE_UD      2
#define FB_ROTATE_CCW     3

#define FBIO_WAITFORVSYNC	_IOW('F', 0x20, __u32)

#endif
