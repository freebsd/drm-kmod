#ifndef LINUX_MOD_DEVICETABLE_H
#define LINUX_MOD_DEVICETABLE_H

#include <sys/types.h>
#include <linux/types.h>

#ifndef I2C_NAME_SIZE
#define I2C_NAME_SIZE	20
#endif
#ifndef I2C_MODULE_PREFIX
#define I2C_MODULE_PREFIX "i2c:"
#endif

#include_next <linux/mod_devicetable.h>
#endif
