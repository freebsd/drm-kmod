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
	fw = firmware_get(name);
	if (fw == NULL) {
		pause("fwwait", hz/2);
		fw = firmware_get(name);
	}
	if (fw == NULL && ((index(name, '/') != NULL) || (index(name, '.') != NULL))) {
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
			device_printf(device->bsddev, "failed to load firmware image %s\n", mapped_name);
			rc = -ENOENT;
			goto fail;
		}
	retry:
		pause("fwwait", hz/4);
		fw = firmware_get(name);
		if (fw == NULL) {
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
		device_printf(device->bsddev, "failed to load firmware image %s\n", name);
		if (mapped_name)
			device_printf(device->bsddev, "failed to load firmware image %s\n", mapped_name);
		rc = -ENOENT;
		goto fail;
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

static bool
dmi_found(const struct dmi_system_id *dsi)
{
	char *hw_vendor, *hw_prod;
	int i, slot;
	bool res;

	hw_vendor = kern_getenv("smbios.planar.maker");
	hw_prod = kern_getenv("smbios.planar.product");
	res = true;
	for (i = 0; i < nitems(dsi->matches); i++) {
		slot = dsi->matches[i].slot;
		switch (slot) {
		case DMI_NONE:
			break;
		case DMI_SYS_VENDOR:
		case DMI_BOARD_VENDOR:
			if (hw_vendor != NULL &&
			    !strcmp(hw_vendor, dsi->matches[i].substr)) {
				break;
			} else {
				res = false;
				goto out;
			}
		case DMI_PRODUCT_NAME:
		case DMI_BOARD_NAME:
			if (hw_prod != NULL &&
			    !strcmp(hw_prod, dsi->matches[i].substr)) {
				break;
			} else {
				res = false;
				goto out;
			}
		default:
			res = false;
			goto out;
		}
	}
out:
	freeenv(hw_vendor);
	freeenv(hw_prod);

	return (res);
}

int
dmi_check_system(const struct dmi_system_id *sysid)
{
	const struct dmi_system_id *dsi;
	bool res;

	for (res = false, dsi = sysid; dsi->matches[0].slot != 0 ; dsi++) {
		if (dmi_found(dsi)) {
			res = true;
			if (dsi->callback != NULL && dsi->callback(dsi))
				break;
		}
	}
	return (res);
}


