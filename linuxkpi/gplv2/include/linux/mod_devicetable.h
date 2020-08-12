#ifndef LINUX_MOD_DEVICETABLE_H
#define LINUX_MOD_DEVICETABLE_H

#include_next <linux/mod_devicetable.h>

#include <sys/types.h>
#include <linux/types.h>

#define I2C_NAME_SIZE	20
#define I2C_MODULE_PREFIX "i2c:"

struct i2c_device_id {
	char name[I2C_NAME_SIZE];
	uintptr_t driver_data;	/* Data private to the driver */
};

#define ACPI_ID_LEN	9

struct acpi_device_id {
	__u8 id[ACPI_ID_LEN];
	u_long driver_data;
	__u32 cls;
	__u32 cls_msk;
};

#endif
