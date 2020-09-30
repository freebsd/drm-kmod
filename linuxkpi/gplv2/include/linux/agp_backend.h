#ifndef _LINUX_GPLV2_AGP_BACKEND_H_
#define	_LINUX_GPLV2_AGP_BACKEND_H_

#include <linux/types.h>

struct agp_version {
	u16 major;
	u16 minor;
};

struct agp_kern_info {
	struct agp_version version;
#ifdef __linux__
	struct pci_dev *device;
	enum chipset_type chipset;
#else
	u16 vendor;
	u16 device;
#endif
	unsigned long mode;
	unsigned long aper_base;
	size_t aper_size;
	int max_memory;		/* In pages */
	int current_memory;
	bool cant_use_aperture;
	unsigned long page_mask;
#ifdef __linux__
	const struct vm_operations_struct *vm_ops;
#endif
};

#endif /* _LINUX_GPLV2_AGP_BACKEND_H_ */
