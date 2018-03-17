#ifndef __DMI_H__
#define __DMI_H__

#include <linux/list.h>
#include <linux/kobject.h>
#include <linux/mod_devicetable.h>

#ifdef CONFIG_DMI

/* enum dmi_field is in mod_devicetable.h */
extern struct kobject *dmi_kobj;
extern int dmi_check_system(const struct dmi_system_id *list);
const struct dmi_system_id *dmi_first_match(const struct dmi_system_id *list);
extern const char * dmi_get_system_info(int field);
extern const struct dmi_device * dmi_find_device(int type, const char *name,
	const struct dmi_device *from);
extern void dmi_scan_machine(void);
extern bool dmi_match(enum dmi_field f, const char *str);

#else

struct dmi_header;

static inline int dmi_check_system(const struct dmi_system_id *list) { return 0; }
static inline const char * dmi_get_system_info(int field) { return NULL; }
static inline const struct dmi_device * dmi_find_device(int type, const char *name,
	const struct dmi_device *from) { return NULL; }
static inline void dmi_scan_machine(void) { return; }
static inline void dmi_memdev_walk(void) { }
static inline void dmi_set_dump_stack_arch_desc(void) { }
static inline bool dmi_get_date(int field, int *yearp, int *monthp, int *dayp)
{
	if (yearp)
		*yearp = 0;
	if (monthp)
		*monthp = 0;
	if (dayp)
		*dayp = 0;
	return false;
}
static inline int dmi_name_in_vendors(const char *s) { return 0; }
static inline int dmi_name_in_serial(const char *s) { return 0; }
#define dmi_available 0
static inline int dmi_walk(void (*decode)(const struct dmi_header *, void *),
	void *private_data) { return -ENXIO; }
static inline bool dmi_match(enum dmi_field f, const char *str)
	{ return false; }
static inline void dmi_memdev_name(u16 handle, const char **bank,
		const char **device) { }
static inline const struct dmi_system_id *
	dmi_first_match(const struct dmi_system_id *list) { return NULL; }
#endif

#endif
