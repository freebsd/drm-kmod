/*-
 * Copyright (c) 2022 Beckhoff Automation GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <linux/device.h>
#include <linux/err.h>

#include <drm/drm_accel.h>
#include <drm/drm_connector.h>
#include <drm/drm_device.h>
#include <drm/drm_print.h>
#include <drm/drm_property.h>
#include <drm/drm_sysfs.h>

#include "drm_internal.h"
#include "drm_crtc_internal.h"

#define to_drm_connector(d) dev_get_drvdata(d)

static struct device_type drm_sysfs_device_minor = {
	.name = "drm_minor"
};

static struct device_type drm_sysfs_device_connector = {
	.name = "drm_connector",
};

struct class *drm_class;

static char *drm_devnode(struct device *dev, umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "dri/%s", dev_name(dev));
}

static CLASS_ATTR_STRING(version, S_IRUGO, "drm 1.1.0 20060810");

int drm_sysfs_init(void)
{
	int err;

	drm_class = class_create("drm");
	if (drm_class == NULL)
		return PTR_ERR(drm_class);

	drm_class->devnode = drm_devnode;
	return 0;
}

void drm_sysfs_destroy(void)
{
	if (IS_ERR_OR_NULL(drm_class))
		return;
	class_destroy(drm_class);
	drm_class = NULL;
}

void drm_sysfs_hotplug_event(struct drm_device *dev)
{
	struct sbuf *sb = sbuf_new_auto();

	drm_dbg_kms(dev, "generating hotplug event\n");

	sbuf_printf(sb, "cdev=dri/%s", dev_name(dev->primary->kdev));
	sbuf_finish(sb);
	devctl_notify("DRM", "CONNECTOR", "HOTPLUG", sbuf_data(sb));
	sbuf_delete(sb);
}

void drm_sysfs_connector_hotplug_event(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct sbuf *sb = sbuf_new_auto();

	drm_dbg_kms(connector->dev,
		    "[CONNECTOR:%d:%s] generating connector hotplug event\n",
		    connector->base.id, connector->name);

	sbuf_printf(sb, "cdev=dri/%s connector=%u",
	    dev_name(dev->primary->kdev), connector->base.id);
	sbuf_finish(sb);
	devctl_notify("DRM", "CONNECTOR", "HOTPLUG", sbuf_data(sb));
	sbuf_delete(sb);
}

void drm_sysfs_connector_property_event(struct drm_connector *connector,
					struct drm_property *property)
{
	struct drm_device *dev = connector->dev;
	struct sbuf *sb = sbuf_new_auto();

	drm_dbg_kms(connector->dev,
		    "[CONNECTOR:%d:%s] generating connector property event for [PROP:%d:%s]\n",
		    connector->base.id, connector->name,
		    property->base.id, property->name);

	sbuf_printf(sb, "cdev=dri/%s connector=%u property=%u",
	    dev_name(dev->primary->kdev), connector->base.id, property->base.id);
	sbuf_finish(sb);
	devctl_notify("DRM", "CONNECTOR", "HOTPLUG", sbuf_data(sb));
	sbuf_delete(sb);
}

static void drm_sysfs_release(struct device *dev)
{
	kfree(dev);
}

/*
 * Connector properties
 */
static ssize_t status_store(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct drm_connector *connector = to_drm_connector(device);
	struct drm_device *dev = connector->dev;
	enum drm_connector_force old_force;
	int ret;

	ret = mutex_lock_interruptible(&dev->mode_config.mutex);
	if (ret)
		return ret;

	old_force = connector->force;

	if (sysfs_streq(buf, "detect"))
		connector->force = 0;
	else if (sysfs_streq(buf, "on"))
		connector->force = DRM_FORCE_ON;
	else if (sysfs_streq(buf, "on-digital"))
		connector->force = DRM_FORCE_ON_DIGITAL;
	else if (sysfs_streq(buf, "off"))
		connector->force = DRM_FORCE_OFF;
	else
		ret = -EINVAL;

	if (old_force != connector->force || !connector->force) {
		drm_dbg_kms(dev, "[CONNECTOR:%d:%s] force updated from %d to %d or reprobing\n",
			    connector->base.id, connector->name,
			    old_force, connector->force);

		connector->funcs->fill_modes(connector,
					     dev->mode_config.max_width,
					     dev->mode_config.max_height);
	}

	mutex_unlock(&dev->mode_config.mutex);

	return ret ? ret : count;
}

static ssize_t status_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	enum drm_connector_status status;

	status = READ_ONCE(connector->status);

	return sysfs_emit(buf, "%s\n",
			  drm_get_connector_status_name(status));
}

static ssize_t dpms_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	int dpms;

	dpms = READ_ONCE(connector->dpms);

	return sysfs_emit(buf, "%s\n", drm_get_dpms_name(dpms));
}

static ssize_t enabled_show(struct device *device,
			    struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	bool enabled;

	enabled = READ_ONCE(connector->encoder);

	return sysfs_emit(buf, enabled ? "enabled\n" : "disabled\n");
}

static ssize_t edid_show(struct file *filp, struct kobject *kobj,
			 struct bin_attribute *attr, char *buf, loff_t off,
			 size_t count)
{
	struct device *connector_dev = kobj_to_dev(kobj);
	struct drm_connector *connector = to_drm_connector(connector_dev);
	unsigned char *edid;
	size_t size;
	ssize_t ret = 0;

	mutex_lock(&connector->dev->mode_config.mutex);
	if (!connector->edid_blob_ptr)
		goto unlock;

	edid = connector->edid_blob_ptr->data;
	size = connector->edid_blob_ptr->length;
	if (!edid)
		goto unlock;

	if (off >= size)
		goto unlock;

	if (off + count > size)
		count = size - off;
	memcpy(buf, edid + off, count);

	ret = count;
unlock:
	mutex_unlock(&connector->dev->mode_config.mutex);

	return ret;
}

static ssize_t modes_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	struct drm_display_mode *mode;
	int written = 0;

	mutex_lock(&connector->dev->mode_config.mutex);
	list_for_each_entry(mode, &connector->modes, head) {
		written += scnprintf(buf + written, PAGE_SIZE - written, "%s\n",
				    mode->name);
	}
	mutex_unlock(&connector->dev->mode_config.mutex);

	return written;
}

static ssize_t connector_id_show(struct device *device,
				 struct device_attribute *attr,
				 char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);

	return sysfs_emit(buf, "%d\n", connector->base.id);
}

static DEVICE_ATTR_RW(status);
static DEVICE_ATTR_RO(enabled);
static DEVICE_ATTR_RO(dpms);
static DEVICE_ATTR_RO(modes);
static DEVICE_ATTR_RO(connector_id);

static struct attribute *connector_dev_attrs[] = {
	&dev_attr_status.attr,
	&dev_attr_enabled.attr,
	&dev_attr_dpms.attr,
	&dev_attr_modes.attr,
	&dev_attr_connector_id.attr,
	NULL
};

static struct bin_attribute edid_attr = {
	.attr.name = "edid",
	.attr.mode = 0444,
	.size = 0,
	.read = edid_show,
};

static struct bin_attribute *connector_bin_attrs[] = {
	&edid_attr,
	NULL
};

static const struct attribute_group connector_dev_group = {
	.attrs = connector_dev_attrs,
	.bin_attrs = connector_bin_attrs,
};

static const struct attribute_group *connector_dev_groups[] = {
	&connector_dev_group,
	NULL
};

int drm_sysfs_connector_add(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	int rv;

	if (connector->kdev)
		return 0;

	connector->kdev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (connector->kdev == NULL) {
		rv = -ENOMEM;
		goto err;
	}

	connector->kdev->class = drm_class;
	connector->kdev->parent = dev->primary->kdev;
	connector->kdev->groups = connector_dev_groups;
	connector->kdev->release = drm_sysfs_release;
	connector->kdev->type = &drm_sysfs_device_connector;
	device_initialize(connector->kdev);
	dev_set_drvdata(connector->kdev, connector);
	rv = kobject_set_name(&connector->kdev->kobj, "card%d-%s",
	    dev->primary->index, connector->name);
	if (rv != 0)
		goto err;

	rv = device_add(connector->kdev);
	if (rv)
		goto err;

	return 0;

err:
	put_device(connector->kdev);
	return (rv);
}

int drm_sysfs_connector_add_late(struct drm_connector *connector)
{
	return 0;
}

void drm_sysfs_connector_remove_early(struct drm_connector *connector)
{
}

void drm_sysfs_connector_remove(struct drm_connector *connector)
{
	if (connector->kdev == NULL)
		return;

	device_unregister(connector->kdev);
	connector->kdev = NULL;
}

struct device *drm_sysfs_minor_alloc(struct drm_minor *minor)
{
	const char *minor_str;
	struct device *kdev;
	int rv;

	kdev = kzalloc(sizeof(*kdev), GFP_KERNEL);
	if (kdev == NULL)
		return ERR_PTR(-ENOMEM);

#ifdef __linux__
	device_initialize(kdev);
#endif

	if (minor->type == DRM_MINOR_ACCEL) {
		minor_str = "accel%d";
		accel_set_device_instance_params(kdev, minor->index);
	} else {
		if (minor->type == DRM_MINOR_RENDER)
			minor_str = "renderD%d";
		else
			minor_str = "card%d";

		kdev->devt = MKDEV(DRM_MAJOR, minor->index);
		kdev->class = drm_class;
		kdev->type = &drm_sysfs_device_minor;
	}

	kdev->parent = minor->dev->dev;
	kdev->release = drm_sysfs_release;
#ifdef __FreeBSD__
	/* FreeBSD depends on kdev->devt initialized already */
	device_initialize(kdev);
#endif
	dev_set_drvdata(kdev, minor);

	rv = dev_set_name(kdev, minor_str, minor->index);
	if (rv < 0)
		goto err;

	rv = drm_dev_alias(kdev, minor, minor_str);
	if (rv < 0)
		goto err;
	return kdev;

err:
	put_device(kdev);
	return ERR_PTR(rv);
}

int drm_class_device_register(struct device *dev)
{
	if (IS_ERR_OR_NULL(drm_class))
		return (-ENOENT);

	dev->class = drm_class;
	return device_register(dev);
}

void drm_class_device_unregister(struct device *dev)
{
	return device_unregister(dev);
}
