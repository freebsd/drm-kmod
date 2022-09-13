/*
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * Copyright (c) 2009-2010, Code Aurora Forum.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _DRM_AGPSUPPORT_H_
#define _DRM_AGPSUPPORT_H_

#include <linux/agp_backend.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <uapi/drm/drm.h>

struct drm_device;
struct drm_file;

struct drm_agp_head {
	struct agp_kern_info agp_info;
	struct list_head memory;
	unsigned long mode;
#ifdef __linux__
	struct agp_bridge_data *bridge;
#elif defined(__FreeBSD__)
	device_t bridge;
#endif
	int enabled;
	int acquired;
	unsigned long base;
#ifdef __linux__
	int agp_mtrr;
#endif
	int cant_use_aperture;
	unsigned long page_mask;
};

#if IS_ENABLED(CONFIG_AGP)

void drm_free_agp(struct agp_memory * handle, int pages);
int drm_bind_agp(struct agp_memory * handle, unsigned int start);
int drm_unbind_agp(struct agp_memory * handle);

struct drm_agp_head *drm_agp_init(struct drm_device *dev);
void drm_legacy_agp_clear(struct drm_device *dev);
int drm_agp_acquire(struct drm_device *dev);
int drm_agp_acquire_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv);
int drm_agp_release(struct drm_device *dev);
int drm_agp_release_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv);
int drm_agp_enable(struct drm_device *dev, struct drm_agp_mode mode);
int drm_agp_enable_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int drm_agp_info(struct drm_device *dev, struct drm_agp_info *info);
int drm_agp_info_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv);
int drm_agp_alloc(struct drm_device *dev, struct drm_agp_buffer *request);
int drm_agp_alloc_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int drm_agp_free(struct drm_device *dev, struct drm_agp_buffer *request);
int drm_agp_free_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv);
int drm_agp_unbind(struct drm_device *dev, struct drm_agp_binding *request);
int drm_agp_unbind_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int drm_agp_bind(struct drm_device *dev, struct drm_agp_binding *request);
int drm_agp_bind_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv);

#else /* CONFIG_AGP */

static inline void drm_free_agp(struct agp_memory * handle, int pages)
{
}

static inline int drm_bind_agp(struct agp_memory * handle, unsigned int start)
{
	return -ENODEV;
}

static inline int drm_unbind_agp(struct agp_memory * handle)
{
	return -ENODEV;
}

static inline struct drm_agp_head *drm_agp_init(struct drm_device *dev)
{
	return NULL;
}

static inline void drm_legacy_agp_clear(struct drm_device *dev)
{
}

static inline int drm_agp_acquire(struct drm_device *dev)
{
	return -ENODEV;
}

static inline int drm_agp_release(struct drm_device *dev)
{
	return -ENODEV;
}

static inline int drm_agp_enable(struct drm_device *dev,
				 struct drm_agp_mode mode)
{
	return -ENODEV;
}

static inline int drm_agp_info(struct drm_device *dev,
			       struct drm_agp_info *info)
{
	return -ENODEV;
}

static inline int drm_agp_alloc(struct drm_device *dev,
				struct drm_agp_buffer *request)
{
	return -ENODEV;
}

static inline int drm_agp_free(struct drm_device *dev,
			       struct drm_agp_buffer *request)
{
	return -ENODEV;
}

static inline int drm_agp_unbind(struct drm_device *dev,
				 struct drm_agp_binding *request)
{
	return -ENODEV;
}

static inline int drm_agp_bind(struct drm_device *dev,
			       struct drm_agp_binding *request)
{
	return -ENODEV;
}

#endif /* CONFIG_AGP */

#endif /* _DRM_AGPSUPPORT_H_ */
