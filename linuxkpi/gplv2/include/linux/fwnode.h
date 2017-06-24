#ifndef _LINUX_FWNODE_H_
#define _LINUX_FWNODE_H_

enum fwnode_type {
	FWNODE_INVALID = 0,
	FWNODE_OF,
	FWNODE_ACPI,
	FWNODE_ACPI_DATA,
	FWNODE_PDATA,
	FWNODE_IRQCHIP,
};

struct fwnode_handle {
	enum fwnode_type type;
	struct fwnode_handle *secondary;
};

#endif
