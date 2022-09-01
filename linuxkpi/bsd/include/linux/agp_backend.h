#ifndef _BSD_LKPI_LINUX_AGP_BACKEND_H_
#define	_BSD_LKPI_LINUX_AGP_BACKEND_H_

struct agp_version {
	uint16_t	major;
	uint16_t	minor;
};

struct agp_kern_info {
	struct agp_version version;
	uint16_t	vendor;
	uint16_t	device;
	unsigned long	mode;
	unsigned long	aper_base;
	size_t		aper_size;
	int		max_memory;
	int		current_memory;
	bool		cant_use_aperture;
	unsigned long	page_mask;
};

struct agp_memory;

#endif /* _BSD_LKPI_LINUX_AGP_BACKEND_H_ */
