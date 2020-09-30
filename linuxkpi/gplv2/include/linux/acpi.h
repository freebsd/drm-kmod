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

/* FreeBSD ACPI code has a typedef for BOOLEAN which conflicts with amdgpu,
 * so let's change it to defining something else */
#define BOOLEAN ACPI_BOOLEAN
#include <contrib/dev/acpica/include/acpi.h>
#include <acpi/acpi.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>
#undef BOOLEAN

static inline acpi_handle acpi_device_handle(struct acpi_device *adev)
{
	return adev ? adev->handle : NULL;
}

#define ACPI_COMPANION(dev)		to_acpi_device_node((dev)->fwnode)
#define ACPI_HANDLE_GET(dev)	acpi_device_handle(ACPI_COMPANION(dev))
#define ACPI_HANDLE(dev)	acpi_device_handle(ACPI_COMPANION(dev))

#define ACPI_VIDEO_OUTPUT_SWITCHING			0x0001
#define ACPI_VIDEO_DEVICE_POSTING			0x0002
#define ACPI_VIDEO_ROM_AVAILABLE			0x0004
#define ACPI_VIDEO_BACKLIGHT				0x0008
#define ACPI_VIDEO_BACKLIGHT_FORCE_VENDOR		0x0010
#define ACPI_VIDEO_BACKLIGHT_FORCE_VIDEO		0x0020
#define ACPI_VIDEO_OUTPUT_SWITCHING_FORCE_VENDOR	0x0040
#define ACPI_VIDEO_OUTPUT_SWITCHING_FORCE_VIDEO		0x0080
#define ACPI_VIDEO_BACKLIGHT_DMI_VENDOR			0x0100
#define ACPI_VIDEO_BACKLIGHT_DMI_VIDEO			0x0200
#define ACPI_VIDEO_OUTPUT_SWITCHING_DMI_VENDOR		0x0400
#define ACPI_VIDEO_OUTPUT_SWITCHING_DMI_VIDEO		0x0800

#define	ACPI_VIDEO_NOTIFY_PROBE			0x81

extern long acpi_is_video_device(acpi_handle handle);

static inline const char *acpi_dev_name(struct acpi_device *adev)
{
	return dev_name(&adev->dev);
}

void acpi_scan_drop_device(acpi_handle handle, void *context);


struct pci_dev *acpi_get_pci_dev(acpi_handle handle);

int acpi_bus_get_device(acpi_handle handle, struct acpi_device **device);

#endif /* _LINUX_GPLV2_ACPI_H_ */
#endif /* x86 */
