/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2017 Intel Corporation
 */

#ifndef __I915_GEMFS_H__
#define __I915_GEMFS_H__

struct drm_i915_private;

#ifdef __linux__
int i915_gemfs_init(struct drm_i915_private *i915);

void i915_gemfs_fini(struct drm_i915_private *i915);
#elif defined(__FreeBSD__)
static inline int
i915_gemfs_init(struct drm_i915_private *i915)
{
	return -ENODEV;
}

static inline void
i915_gemfs_fini(struct drm_i915_private *i915)
{
	return;
}
#endif

#endif
