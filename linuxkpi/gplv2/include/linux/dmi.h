#ifndef __DMI_H__
#define __DMI_H__

#include <linux/list.h>
#include <linux/kobject.h>
#include <linux/mod_devicetable.h>

/* enum dmi_field is in mod_devicetable.h */
extern struct kobject *dmi_kobj;
extern int dmi_check_system(const struct dmi_system_id *list);
const struct dmi_system_id *dmi_first_match(const struct dmi_system_id *list);
extern const char * dmi_get_system_info(int field);
extern const struct dmi_device * dmi_find_device(int type, const char *name,
	const struct dmi_device *from);
extern void dmi_scan_machine(void);
#endif
