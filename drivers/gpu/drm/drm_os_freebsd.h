/**
 * \file drm_os_freebsd.h
 * OS abstraction macros.
 */

#ifndef _DRM_OS_FREEBSD_H_
#define	_DRM_OS_FREEBSD_H_

#include <sys/param.h>
#include <sys/bus.h>

#include <linux/mod_devicetable.h>
#include <linux/fb.h>

#define DRM_DEV_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)
#define DRM_DEV_UID	UID_ROOT
#define DRM_DEV_GID	GID_VIDEO

/* XXXKIB what is the right code for the FreeBSD ? */
/* kib@ used ENXIO here -- dumbbell@ */
#define	EREMOTEIO	EIO

#define	KTR_DRM		KTR_DEV

struct drm_minor;
struct device;

MALLOC_DECLARE(DRM_MEM_DRIVER);

int drm_dev_alias(struct device *dev, struct drm_minor *minor, const char *minor_str);
void cancel_reset_debug_log(void);

#endif /* _DRM_OS_FREEBSD_H_ */
