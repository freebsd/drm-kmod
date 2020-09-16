/*
 *  acpi_bus.h - ACPI Bus Driver ($Revision: 22 $)
 *
 *  Copyright (C) 2001, 2002 Andy Grover <andrew.grover@intel.com>
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef __ACPI_ACPI_BUS_H__
#define __ACPI_ACPI_BUS_H__

struct device;

/* acpi_utils.h */
acpi_status
acpi_evaluate_integer(acpi_handle handle,
		      acpi_string pathname,
		      struct acpi_object_list *arguments, unsigned long long *data);

bool acpi_has_method(acpi_handle handle, char *name);

bool acpi_check_dsm(acpi_handle handle, const u8 *uuid, int rev, u64 funcs);
union acpi_object *acpi_evaluate_dsm(acpi_handle handle, const u8 *uuid,
			int rev, int func, union acpi_object *argv4);

static inline union acpi_object *
acpi_evaluate_dsm_typed(acpi_handle handle, const u8 *uuid, int rev, int func,
			union acpi_object *argv4, acpi_object_type type)
{
	union acpi_object *obj;

	obj = acpi_evaluate_dsm(handle, uuid, rev, func, argv4);
	if (obj && obj->Type != type) {
		ACPI_FREE(obj);
		obj = NULL;
	}

	return obj;
}

#ifdef CONFIG_ACPI

struct acpi_driver;
struct acpi_device;

/*
 * ACPI Scan Handler
 * -----------------
 */

struct acpi_hotplug_profile {
	struct kobject kobj;
	int (*scan_dependent)(struct acpi_device *adev);
	void (*notify_online)(struct acpi_device *adev);
	bool enabled:1;
	bool demand_offline:1;
};

/*
 * ACPI Driver
 * -----------
 */

typedef int (*acpi_op_add) (struct acpi_device * device);
typedef int (*acpi_op_remove) (struct acpi_device * device);
typedef void (*acpi_op_notify) (struct acpi_device * device, u32 event);

struct acpi_device_ops {
	acpi_op_add add;
	acpi_op_remove remove;
	acpi_op_notify notify;
};

struct acpi_driver {
	char name[80];
	char class[80];
	const struct acpi_device_id *ids; /* Supported Hardware IDs */
	unsigned int flags;
	struct acpi_device_ops ops;
	struct device_driver drv;
	struct module *owner;
};

/*
 * ACPI Device
 * -----------
 */

/* Status (_STA) */

struct acpi_device_status {
	u32 present:1;
	u32 enabled:1;
	u32 show_in_ui:1;
	u32 functional:1;
	u32 battery_present:1;
	u32 reserved:27;
};

/* Plug and Play */

typedef char acpi_bus_id[8];
typedef unsigned long acpi_bus_address;
typedef char acpi_device_name[40];
typedef char acpi_device_class[20];

struct acpi_hardware_id {
	struct list_head list;
	const char *id;
};

struct acpi_pnp_type {
	u32 hardware_id:1;
	u32 bus_address:1;
	u32 platform_id:1;
	u32 reserved:29;
};

struct acpi_device_pnp {
	acpi_bus_id bus_id;		/* Object name */
	struct acpi_pnp_type type;	/* ID type */
	acpi_bus_address bus_address;	/* _ADR */
	char *unique_id;		/* _UID */
	struct list_head ids;		/* _HID and _CIDs */
	acpi_device_name device_name;	/* Driver-determined */
	acpi_device_class device_class;	/*        "          */
	union acpi_object *str_obj;	/* unicode string for _STR method */
};

#define acpi_device_name(d)	((d)->pnp.device_name)
#define acpi_device_class(d)	((d)->pnp.device_class)

/* Device */
struct acpi_device {
	acpi_handle handle;		/* no handle for fixed hardware */
	struct fwnode_handle fwnode;
	struct list_head del_list;
	struct acpi_device_status status;
	struct acpi_device_pnp pnp;
	struct device dev;
};

static inline bool is_acpi_device_node(struct fwnode_handle *fwnode)
{
	return fwnode && fwnode->type == FWNODE_ACPI;
}

static inline struct acpi_device *to_acpi_device_node(struct fwnode_handle *fwnode)
{
	return is_acpi_device_node(fwnode) ?
		container_of(fwnode, struct acpi_device, fwnode) : NULL;
}

#define to_acpi_device(d)	container_of(d, struct acpi_device, dev)

/*
 * Events
 * ------
 */

struct acpi_bus_event {
	struct list_head node;
	acpi_device_class device_class;
	acpi_bus_id bus_id;
	u32 type;
	u32 data;
};

extern int register_acpi_notifier(struct notifier_block *);
extern int unregister_acpi_notifier(struct notifier_block *);

/*
 * External Functions
 */

struct of_device_id;

const struct acpi_device_id *linux_acpi_match_device(struct acpi_device *device, const struct acpi_device_id *ids,
    const struct of_device_id *of_ids);

struct acpi_pci_root {
	struct acpi_device * device;
	struct pci_bus *bus;
	u16 segment;

	u32 osc_support_set;	/* _OSC state of support bits */
	u32 osc_control_set;	/* _OSC state of control bits */
	phys_addr_t mcfg_addr;
};

/* helper */

#ifdef CONFIG_ACPI_SLEEP
u32 acpi_target_system_state(void);
#else
static inline u32 acpi_target_system_state(void) { return ACPI_STATE_S0; }
#endif

#else	/* CONFIG_ACPI */

#endif	/* CONFIG_ACPI */

#endif /*__ACPI_BUS_H__*/
