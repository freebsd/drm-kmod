#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/linker.h>
#include <sys/malloc.h>
#include <sys/firmware.h>

#include <linux/firmware.h>
#include <linux/device.h>
#include <linux/dmi.h>
#undef firmware

MALLOC_DEFINE(M_LKPI_FW, "lkpifw", "LinuxKPI firmware");

int
request_firmware(const struct linux_firmware **lkfwp, const char *name,
		     struct device *device)
{
	struct linux_firmware *lkfw;
	const struct firmware *fw;
	char *mapped_name, *pindex;
	linker_file_t result;
	int rc, retries;

	fw = NULL;
	*lkfwp = NULL;
	mapped_name = NULL;
	lkfw = malloc(sizeof(*lkfw), M_LKPI_FW, M_WAITOK);
	retries = 0;

	/*
	 * First try with mapped name since that's all our firmware modules
	 */
	if ((index(name, '/') != NULL) || (index(name, '.') != NULL)) {
		mapped_name = strdup(name, M_LKPI_FW);
		if (mapped_name == NULL) {
			rc = -ENOMEM;
			goto fail;
		}
		while ((pindex = index(mapped_name, '/')) != NULL)
			*pindex = '_';
		while ((pindex = index(mapped_name, '.')) != NULL)
			*pindex = '_';
		if (linker_reference_module(mapped_name, NULL, &result)) {
			device_printf(device->bsddev, "failed to link firmware "
			    "kernel module with mapped name: %s\n", mapped_name);
			goto fail_mapped;
		}
	retry:
		pause("fwwait", hz/2);
		fw = firmware_get(name);
		if (fw == NULL) {
			device_printf(device->bsddev, "fail (%d) to get firmware "
			    "image with name: %s\n", retries, name);
			fw = firmware_get(mapped_name);
			if (fw == NULL) {
				device_printf(device->bsddev, "fail (%d) to get "
				    "firmware image with mapped name: %s\n",
				    retries, name);
				if (retries++ < 10) {
					goto retry;
				}
			} else {
				device_printf(device->bsddev, "successfully loaded firmware "
				    "image with mapped name: %s\n", mapped_name);
			}
		} else {
			device_printf(device->bsddev, "successfully loaded firmware "
			    "image with name: %s\n", name);
		}
#ifdef __notyet__
		/* XXX leave dangling ref */
		linker_file_unload(result,  0);
#endif
	}

fail_mapped:
	/*
	 * Then try the original name
	 */
	if (fw == NULL) {
		fw = firmware_get(name);
		if (fw == NULL) {
			pause("fwwait", hz/2);
			fw = firmware_get(name);
		}
		if (fw) {
			device_printf(device->bsddev, "successfully loaded firmware "
			    "image with name: %s\n", name);
		} else {
			device_printf(device->bsddev, "failed to load firmware "
			    "with name: %s\n", name);
			rc = -ENOENT;
			goto fail;
		}
	}

	free(mapped_name, M_LKPI_FW);
	lkfw->priv = __DECONST(void *, fw);
	lkfw->size = fw->datasize;
	lkfw->data = fw->data;
	lkfw->pages = NULL;
	*lkfwp = lkfw;
	return (0);
fail:
	free(mapped_name, M_LKPI_FW);
	free(lkfw, M_LKPI_FW);
	return (rc);
}

void
release_firmware(const struct linux_firmware *lkfw)
{
	struct firmware *fw;	

	if (lkfw == NULL)
		return;

	fw = lkfw->priv;
	free(__DECONST(void *, lkfw), M_LKPI_FW);
	firmware_put(fw, 0);
}
