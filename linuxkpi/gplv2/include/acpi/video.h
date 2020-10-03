#include <sys/param.h>
#if __FreeBSD_version < 1300118
#ifndef _LINUX_ACPI_VIDEO_H_
#define _LINUX_ACPI_VIDEO_H_

#include <sys/systm.h>

#define ACPI_VIDEO_CLASS   "video"

enum acpi_backlight_type {
        acpi_backlight_undef = -1,
        acpi_backlight_none = 0,
        acpi_backlight_video,
        acpi_backlight_vendor,
        acpi_backlight_native,
};

enum acpi_backlight_type acpi_video_get_backlight_type(void);

#endif
#else
#include_next <acpi/video.h>
#endif
