/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __I915_PERF_H__
#define __I915_PERF_H__

#include <linux/types.h>

struct drm_device;
struct drm_file;
struct drm_i915_private;
struct intel_context;
struct intel_engine_cs;

#ifdef CONFIG_I915_PERF // Not yet. i915_perf.c opens a can of worms...
void i915_perf_init(struct drm_i915_private *i915);
void i915_perf_fini(struct drm_i915_private *i915);
void i915_perf_register(struct drm_i915_private *i915);
void i915_perf_unregister(struct drm_i915_private *i915);
#else
static inline void
i915_perf_init(struct drm_i915_private *dev_priv)
{

	return;
}

static inline void
i915_perf_fini(struct drm_i915_private *dev_priv)
{

	return;
}

static inline void
i915_perf_register(struct drm_i915_private *dev_priv)
{

	return;
}

static inline void
i915_perf_unregister(struct drm_i915_private *dev_priv)
{

	return;
}
#endif

#if defined(CONFIG_I915_PERF)
int i915_perf_open_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file);
int i915_perf_add_config_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file);
int i915_perf_remove_config_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file);
void i915_oa_init_reg_state(struct intel_engine_cs *engine,
			    struct intel_context *ce,
			    u32 *reg_state);
#else
static inline int
i915_perf_open_ioctl(struct drm_device *dev, void *data,
    struct drm_file *file)
{

	return (0);
}

static inline int
i915_perf_add_config_ioctl(struct drm_device *dev, void *data,
    struct drm_file *file)
{

	return (0);
}

static inline int
i915_perf_remove_config_ioctl(struct drm_device *dev, void *data,
    struct drm_file *file)
{

	return (0);
}

static inline void
i915_oa_init_reg_state(struct intel_engine_cs *engine,
    struct intel_context *ce,
    u32 *reg_state)
{

	return;
}
#endif

#endif /* __I915_PERF_H__ */
