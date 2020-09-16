/*
 * acpi.h - ACPI Interface
 *
 * Copyright (C) 2001 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#if defined(__i386__) || defined(__amd64__) || defined(__aarch64__)
#ifndef _LINUX_GPLV2_ACPI_H_
#define _LINUX_GPLV2_ACPI_H_

#include <linux/errno.h>
#include <linux/resource_ext.h>
#include <linux/device.h>
#include <linux/property.h>
#include <linux/notifier.h>
#include <linux/list.h>
#include <linux/mod_devicetable.h>

#include <contrib/dev/acpica/include/acpi.h>
#include <acpi/acpi.h>
#include <acpi/acpi_bus.h>

static inline acpi_handle acpi_device_handle(struct acpi_device *adev)
{
	return adev ? adev->handle : NULL;
}

#define ACPI_COMPANION(dev)		to_acpi_device_node((dev)->fwnode)
#define ACPI_HANDLE_GET(dev)	acpi_device_handle(ACPI_COMPANION(dev))
#define ACPI_HANDLE(dev)	acpi_device_handle(ACPI_COMPANION(dev))

#define	ACPI_VIDEO_NOTIFY_PROBE			0x81

static inline const char *acpi_dev_name(struct acpi_device *adev)
{
	return dev_name(&adev->dev);
}

#endif /* _LINUX_GPLV2_ACPI_H_ */
#endif /* x86 */
