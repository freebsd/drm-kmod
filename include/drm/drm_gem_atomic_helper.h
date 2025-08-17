/* Public domain. */

#ifndef __DRM_GEM_ATOMIC_HELPER_H__
#define __DRM_GEM_ATOMIC_HELPER_H__

#include <linux/iosys-map.h>

#include <drm/drm_format_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_plane.h>

int drm_gem_plane_helper_prepare_fb(struct drm_plane *,
    struct drm_plane_state *);

#endif
