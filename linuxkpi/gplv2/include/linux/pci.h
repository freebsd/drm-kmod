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

#if __FreeBSD_version < 1300135
static inline int
pci_bus_read_config(struct pci_bus *bus, unsigned int devfn,
		    int where, uint32_t *val, int size)
{
	device_t dev;
	int dom, busid, slot, func;

	dom = pci_get_domain(bus->self->dev.bsddev);
	busid = pci_get_bus(bus->self->dev.bsddev);
	slot = ((devfn >> 3) & 0x1f);
	func = devfn & 0x7;
	dev = pci_find_dbsf(dom, busid, slot, func);
	*val = pci_read_config(dev, where, size);
	return (0);
}

static inline int
pci_bus_read_config_word(struct pci_bus *bus, unsigned int devfn, int where, u16 *val)
{
	return (pci_bus_read_config(bus, devfn, where, (uint32_t *)val, 2));
}

static inline int
pci_bus_read_config_byte(struct pci_bus *bus, unsigned int devfn, int where, u8 *val)
{
	return (pci_bus_read_config(bus, devfn, where, (uint32_t *)val, 1));
}

static inline int
pci_bus_write_config(struct pci_bus *bus, unsigned int devfn, int where,
    uint32_t val, int size)
{
	device_t dev;
	int dom, busid, slot, func;

	dom = pci_get_domain(bus->self->dev.bsddev);
	busid = pci_get_bus(bus->self->dev.bsddev);
	slot = ((devfn >> 3) & 0x1f);
	func = devfn & 0x7;
	dev = pci_find_dbsf(dom, busid, slot, func);
	pci_write_config(dev, where, val, size);
	return (0);
}

static inline int
pci_bus_write_config_byte(struct pci_bus *bus, unsigned int devfn, int where,
    uint8_t val)
{
	return (pci_bus_write_config(bus, devfn, where, val, 1));
}

static inline int
pci_domain_nr(struct pci_bus *bus)
{

	return (0);
}

extern struct pci_dev *pci_get_bus_and_slot(unsigned int bus, unsigned int devfn);

static inline struct pci_dev *
pci_get_domain_bus_and_slot(int domain, unsigned int bus, unsigned int devfn)
{

	return (pci_get_bus_and_slot(bus, devfn));
}
#endif

void pci_dev_put(struct pci_dev *pdev);

#if __FreeBSD_version < 1300135
static inline bool
pci_is_root_bus(struct pci_bus *pbus)
{

	return (pbus->self == NULL);
}

#endif


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

#if __FreeBSD_version < 1300135
static inline void
linux_pci_save_state(struct pci_dev *pdev)
{

	pci_save_state(pdev->dev.bsddev);
}

static inline void
linux_pci_restore_state(struct pci_dev *pdev)
{

	pci_restore_state(pdev->dev.bsddev);
}
#endif

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

#if __FreeBSD_version < 1300135
#define	pci_get_class(class, dev)	linux_pci_get_class(class, dev)
#define	pci_save_state(dev)	linux_pci_save_state(dev)
#define	pci_restore_state(dev)	linux_pci_restore_state(dev)
#endif

#endif /* _LINUX_GPLV2_PCI_H_ */
