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

#define BSD_TO_LINUX_RESOURCE(r)		\
	{					\
	 .bsd_res = (r),			\
	 .start = rman_get_start(r),		\
	 .end = rman_get_end(r),		\
	}

struct linux_resource {
	struct resource *bsd_res;
	resource_size_t start;
	resource_size_t end;
	/* const char *name; */
	/* unsigned long flags; */
	/* unsigned long desc; */
	/* struct resource *parent, *sibling, *child; */
};

struct pci_dev *linux_pci_get_class(unsigned int class, struct pci_dev *from);

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

void pci_dev_put(struct pci_dev *pdev);

static inline struct pci_dev *
pci_upstream_bridge(struct pci_dev *dev)
{

	UNIMPLEMENTED();
	return (NULL);
}

static inline bool
pci_is_thunderbolt_attached(struct pci_dev *pdev)
{
	UNIMPLEMENTED();
	return false;
}

static inline void *
pci_platform_rom(struct pci_dev *pdev, size_t *size)
{

	UNIMPLEMENTED();
	return (NULL);
}

static inline void
pci_ignore_hotplug(struct pci_dev *pdev)
{

	UNIMPLEMENTED();
}

static inline void *
pci_alloc_consistent(struct pci_dev *hwdev, size_t size, dma_addr_t *dma_handle)
{

	return (dma_alloc_coherent(hwdev == NULL ? NULL : &hwdev->dev, size,
	    dma_handle, GFP_ATOMIC));
}

static inline int
pcie_get_readrq(struct pci_dev *dev)
{
	u16 ctl;

	if (pcie_capability_read_word(dev, PCI_EXP_DEVCTL, &ctl))
		return (-EINVAL);

	return 128 << ((ctl & PCI_EXP_DEVCTL_READRQ) >> 12);
}

#endif /* _LINUX_GPLV2_PCI_H_ */
