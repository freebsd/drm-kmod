
#include <sys/param.h>

#if __FreeBSD_version < 1300118

#include <linux/dmi.h>

bool
dmi_match(enum dmi_field f, const char *str)
{
	const char *info = dmi_get_system_info(f);

	if (info == NULL || str == NULL)
		return info == str;

	return !strcmp(info, str);
}

const char *
dmi_get_system_info(int field)
{

	UNIMPLEMENTED();
	return NULL;
}

const struct dmi_system_id *
dmi_first_match(const struct dmi_system_id *list)
{

	// v4.16 - used to find panel orientation
	// Return NULL for now and let it have panel
	// orientation unknown.
	return NULL;
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

#endif /* __FreeBSD_version */
