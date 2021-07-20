/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __I915_PERF_H__
#define __I915_PERF_H__

#include <linux/kref.h>
#include <linux/types.h>

#include "i915_perf_types.h"

struct drm_device;
struct drm_file;
struct drm_i915_private;
struct i915_oa_config;
struct intel_context;
struct intel_engine_cs;

#ifdef CONFIG_I915_PERF // Not yet. i915_perf.c opens a can of worms...
void i915_perf_init(struct drm_i915_private *i915);
void i915_perf_fini(struct drm_i915_private *i915);
void i915_perf_register(struct drm_i915_private *i915);
void i915_perf_unregister(struct drm_i915_private *i915);
int i915_perf_ioctl_version(void);
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

static inline int i915_perf_ioctl_version(void)
{
	return 1;
}

#endif
void i915_perf_sysctl_register(void);
void i915_perf_sysctl_unregister(void);

#if defined(CONFIG_I915_PERF)
int i915_perf_open_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file);
int i915_perf_add_config_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file);
int i915_perf_remove_config_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file);

void i915_oa_init_reg_state(const struct intel_context *ce,
			    const struct intel_engine_cs *engine);
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
i915_oa_init_reg_state(const struct intel_context *ce,
    const struct intel_engine_cs *engine)
{

	return;
}
#endif

struct i915_oa_config *
i915_perf_get_oa_config(struct i915_perf *perf, int metrics_set);

static inline struct i915_oa_config *
i915_oa_config_get(struct i915_oa_config *oa_config)
{
	if (kref_get_unless_zero(&oa_config->ref))
		return oa_config;
	else
		return NULL;
}

void i915_oa_config_release(struct kref *ref);
static inline void i915_oa_config_put(struct i915_oa_config *oa_config)
{
	if (!oa_config)
		return;

#ifdef FREEBSD_NOTYET
	kref_put(&oa_config->ref, i915_oa_config_release);
#endif
}

#endif /* __I915_PERF_H__ */
