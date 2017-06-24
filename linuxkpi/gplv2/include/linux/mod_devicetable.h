#ifndef LINUX_MOD_DEVICETABLE_H
#define LINUX_MOD_DEVICETABLE_H

#include <sys/types.h>
#include <linux/types.h>

#define I2C_NAME_SIZE	20
#define I2C_MODULE_PREFIX "i2c:"

struct i2c_device_id {
	char name[I2C_NAME_SIZE];
	uintptr_t driver_data;	/* Data private to the driver */
};

enum dmi_field {
	DMI_NONE,
        DMI_BIOS_VENDOR,
        DMI_BIOS_VERSION,
        DMI_BIOS_DATE,
        DMI_SYS_VENDOR,
        DMI_PRODUCT_NAME,
        DMI_PRODUCT_VERSION,
        DMI_PRODUCT_SERIAL,
        DMI_PRODUCT_UUID,
        DMI_BOARD_VENDOR,
        DMI_BOARD_NAME,
        DMI_BOARD_VERSION,
        DMI_BOARD_SERIAL,
        DMI_BOARD_ASSET_TAG,
        DMI_CHASSIS_VENDOR,
        DMI_CHASSIS_TYPE,
        DMI_CHASSIS_VERSION,
        DMI_CHASSIS_SERIAL,
        DMI_CHASSIS_ASSET_TAG,
        DMI_STRING_MAX,
};

struct dmi_strmatch {
	unsigned char slot:7;
	unsigned char exact_match:1;
	char substr[79];
};

struct dmi_system_id {
	int (*callback)(const struct dmi_system_id *);
	const char *ident;
	struct dmi_strmatch matches[4];
	void *driver_data;
};

#define ACPI_ID_LEN	9

struct acpi_device_id {
	__u8 id[ACPI_ID_LEN];
	u_long driver_data;
	__u32 cls;
	__u32 cls_msk;
};


#define dmi_device_id dmi_system_id

#define DMI_MATCH(a, b)	{ .slot = a, .substr = b }
#define DMI_EXACT_MATCH(a, b)	{ .slot = a, .substr = b, .exact_match = 1 }

#endif
