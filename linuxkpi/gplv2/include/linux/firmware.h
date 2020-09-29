#ifndef _LINUX_FIRMWARE_H
#define _LINUX_FIRMWARE_H

#if !defined(LINUXKPI_COOKIE) || (LINUXKPI_COOKIE < 1600256818)
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/gfp.h>

#define FW_ACTION_NOHOTPLUG 0
#define FW_ACTION_HOTPLUG 1

struct linux_firmware {
	size_t size;
	const u8 *data;
	struct page **pages;

	/* firmware loader private fields */
	void *priv;
};

struct module;
struct device;

struct builtin_fw {
	char *name;
	void *data;
	unsigned long size;
};

/* We have to play tricks here much like stringify() to get the
   __COUNTER__ macro to be expanded as we want it */
#define __fw_concat1(x, y) x##y
#define __fw_concat(x, y) __fw_concat1(x, y)

#define DECLARE_BUILTIN_FIRMWARE(name, blob)				     \
	DECLARE_BUILTIN_FIRMWARE_SIZE(name, &(blob), sizeof(blob))

#define DECLARE_BUILTIN_FIRMWARE_SIZE(name, blob, size)			     \
	static const struct builtin_fw __fw_concat(__builtin_fw,__COUNTER__) \
	__used __section(.builtin_fw) = { name, blob, size }

int request_firmware(const struct linux_firmware **fw, const char *name,
		     struct device *device);
int request_firmware_nowait(
	struct module *module, bool uevent,
	const char *name, struct device *device, gfp_t gfp, void *context,
	void (*cont)(const struct linux_firmware *fw, void *context));

#define	request_firmware_direct(f,n,d) request_firmware(f,n,d)

void release_firmware(const struct linux_firmware *fw);
#define firmware linux_firmware
#else

#include_next <linux/firmware.h>

#define	request_firmware_direct(f,n,d) request_firmware(f,n,d)
#endif


#endif
