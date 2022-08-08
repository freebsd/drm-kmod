#ifndef _LINUX_GPLV2_PCI_H_
#define	_LINUX_GPLV2_PCI_H_

#include <sys/param.h>
#include <sys/bus.h>
#include <machine/bus.h>
#include <sys/rman.h>
#include <machine/resource.h>

#include_next <linux/pci.h>

#define	DEFINE_RES_MEM(_start, _size)		\
	{					\
	 .bsd_res = NULL,			\
	 .start = (_start),			\
	 .end = (_start) + (_size) - 1,		\
	}

struct linux_resource {
	struct resource *bsd_res;
	resource_size_t start;
	resource_size_t end;
};
#define	resource	linux_resource

static inline resource_size_t
resource_size(const struct linux_resource *res)
{
	return res->end - res->start + 1;
}

static inline bool
resource_contains(struct linux_resource *a, struct linux_resource *b)
{
	return a->start <= b->start && a->end >= b->end;
}

#endif /* _LINUX_GPLV2_PCI_H_ */
