/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef INTEL_ENGINE_SYSFS_H
#define INTEL_ENGINE_SYSFS_H

struct drm_i915_private;

void intel_engines_add_sysfs(struct drm_i915_private *i915);
#ifdef __FreeBSD__
void intel_engines_remove_sysfs(struct drm_i915_private *i915);
#endif

#endif /* INTEL_ENGINE_SYSFS_H */
