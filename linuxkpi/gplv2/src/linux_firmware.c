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

	*lkfwp = NULL;
	mapped_name = NULL;
	lkfw = malloc(sizeof(*lkfw), M_LKPI_FW, M_WAITOK);

	retries = 0;
	device_printf(device->bsddev, "try to load firmware with "
	    "name: %s\n", name);
	fw = firmware_get(name);
	if (fw == NULL) {
		pause("fwwait", hz/2);
		device_printf(device->bsddev, "retry to load firmware "
		    "with name: %s\n", name);
		fw = firmware_get(name);
	}
	if (fw == NULL &&
	    ((index(name, '/') != NULL) || (index(name, '.') != NULL))) {
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
			    "module with mapped name: %s\n", mapped_name);
			rc = -ENOENT;
			goto fail;
		}
		device_printf(device->bsddev, "successfully linked firmware "
		    "module with mapped name: %s\n", mapped_name);
	retry:
		pause("fwwait", hz/4);
		device_printf(device->bsddev, "try (%d) to load firmware "
		    "image with name: %s\n", retries, name);
		fw = firmware_get(name);
		if (fw == NULL) {
			device_printf(device->bsddev, "try (%d) to load "
			    "firmware image with mapped name: %s\n",
			    retries, name);
			fw = firmware_get(mapped_name);
			if (fw == NULL) {
				if (retries++ < 10)
					goto retry;
			}
		}

#ifdef __notyet__
		/* XXX leave dangling ref */
		linker_file_unload(result,  0);
#endif
	}
	if (fw == NULL) {
		device_printf(device->bsddev, "failed to load firmware "
		    "image with name: %s\n", name);
		if (mapped_name)
			device_printf(device->bsddev, "failed to load firmware "
			    "image with mapped name: %s\n", mapped_name);
		rc = -ENOENT;
		goto fail;
	}

	device_printf(device->bsddev, "successfully loaded firmware "
	    "image with name: %s\n", name);

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
