/* Public domain. */

#ifndef _DRM_LEASE_H
#define _DRM_LEASE_H

#include <drm/drm_device.h>
#include <drm/drm_file.h>

struct drm_master;

#define drm_lease_owner(m)		(m)
#define drm_lease_held(f, id)		(true)
#define _drm_lease_held(f, id)		(true)
#define drm_lease_filter_crtcs(f, in)	(in)

static inline void
drm_lease_revoke(struct drm_master *m)
{
}

static inline void
drm_lease_destroy(struct drm_master *m)
{
}

int drm_mode_create_lease_ioctl(struct drm_device *dev,
    void *data, struct drm_file *lessor_priv);
int drm_mode_list_lessees_ioctl(struct drm_device *dev,
    void *data, struct drm_file *lessor_priv);
int drm_mode_get_lease_ioctl(struct drm_device *dev,
    void *data, struct drm_file *lessee_priv);
int drm_mode_revoke_lease_ioctl(struct drm_device *dev,
    void *data, struct drm_file *lessor_priv);

#endif
