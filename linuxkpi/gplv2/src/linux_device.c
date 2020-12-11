#include <linux/device.h>
#include <linux/pci.h>

#undef resource

static MALLOC_DEFINE(M_DEVRES, "devres", "Linux compat devres");

static struct devres *
dr_node_alloc(dr_release_t release, size_t size, int flags)
{
	size_t tot_size = sizeof(struct devres) + size;
	struct devres *dr;

	if ((dr = malloc(tot_size, M_DEVRES, flags|M_ZERO)) == NULL)
		return (NULL);
	
	INIT_LIST_HEAD(&dr->node.entry);
	dr->node.release = release;
	return (dr);
}

static void
dr_list_insert(struct device *dev, struct devres_node *node)
{
	list_add_tail(&node->entry, &dev->devres_head);
}

static struct devres *
dr_list_lookup(struct device *dev, dr_release_t release,
	       dr_match_t match, void *match_data)
{
	struct devres_node *node;

	list_for_each_entry_reverse(node, &dev->devres_head, entry) {
		struct devres *dr = container_of(node, struct devres, node);

		if (node->release != release)
			continue;
		if (match && !match(dev, dr->data, match_data))
			continue;
		return (dr);
	}

	return (NULL);
}

void *
devres_alloc_node(dr_release_t release, size_t size, gfp_t gfp, int nid __unused)
{
	struct devres *dr;

	if ((dr = dr_node_alloc(release, size, gfp)) == NULL)
		return (NULL);
	return (dr->data);
}

void
devres_add(struct device *dev, void *res)
{
	struct devres *dr = container_of(res, struct devres, data);
	unsigned long flags __unused;

	spin_lock_irqsave(&dev->devres_lock, flags);
	dr_list_insert(dev, &dr->node);
	spin_unlock_irqrestore(&dev->devres_lock, flags);
}

void *
devres_remove(struct device *dev, dr_release_t release,
	      dr_match_t match, void *match_data)
{
	struct devres *dr;
	unsigned long flags __unused;

	spin_lock_irqsave(&dev->devres_lock, flags);
	dr = dr_list_lookup(dev, release, match, match_data);
	if (dr)
		list_del_init(&dr->node.entry);
	spin_unlock_irqrestore(&dev->devres_lock, flags);
	return (dr ? dr->data : NULL);
}

int
devres_release(struct device *dev, dr_release_t release,
		       dr_match_t match, void *match_data)
{
	void *res;

	res = devres_remove(dev, release, match, match_data);
	if (__predict_false(res == 0))
		return -ENOENT;

	(*release)(dev, res);
	devres_free(res);
	return 0;
}

void
devres_free(void *res)
{
	if (res) {
		struct devres *dr = container_of(res, struct devres, data);

		BUG_ON(!list_empty(&dr->node.entry));
		free(dr, M_DEVRES);
	}
}

#if __FreeBSD_version < 1300135
struct pci_dev *
pci_get_bus_and_slot(unsigned int bus, unsigned int devfn)
{
	device_t dev;
	struct pci_dev *pdev;
	struct pci_bus *pbus;

	dev = pci_find_bsf(bus, devfn >> 16, devfn & 0xffff);
	if (dev == NULL)
		return (NULL);

	pdev = malloc(sizeof(*pdev), M_DEVBUF, M_WAITOK|M_ZERO);
	pdev->devfn = devfn;
	pdev->dev.bsddev = dev;
	pbus = malloc(sizeof(*pbus), M_DEVBUF, M_WAITOK|M_ZERO);
	pbus->self = pdev;
	pdev->bus = pbus;
	return (pdev);
}
#endif

void
pci_dev_put(struct pci_dev *pdev)
{
	if (pdev == NULL)
		return;

	MPASS(pdev->bus);
	MPASS(pdev->bus->self == pdev);
	free(pdev->bus, M_DEVBUF);
	free(pdev, M_DEVBUF);
}

#if 0
struct pci_dev *
linux_pci_get_class(unsigned int class, struct pci_dev *from)
{
	device_t dev;
	struct pci_dev *pdev;
	struct pci_bus *pbus;
	int pcic, pcis;

	/* Only return one device for now. */
	if (from != NULL)
		return (NULL);

	class >>= 8;
	if (class == PCI_CLASS_BRIDGE_ISA) {
		pcis = PCIS_BRIDGE_ISA;
		pcic = PCIC_BRIDGE;
	} else if (class == PCI_CLASS_DISPLAY_VGA) {
		pcis = PCIS_DISPLAY_VGA;
		pcic = PCIC_DISPLAY;
	} else if (class == PCI_CLASS_DISPLAY_OTHER) {
		pcis = PCIS_DISPLAY_OTHER;
		pcic = PCIC_DISPLAY;
	} else {
		log(LOG_WARNING, "unrecognized class %x in %s\n", class, __FUNCTION__);
		return (NULL);
	}

	dev = pci_find_class(pcic, pcis);
	if (dev == NULL)
		return (NULL);

	pdev = malloc(sizeof(*pdev), M_DEVBUF, M_WAITOK|M_ZERO);
	/* XXX do we need to initialize pdev more here ? */
	pdev->devfn = PCI_DEVFN(pci_get_slot(dev), pci_get_function(dev));
	pdev->vendor = pci_get_vendor(dev);
	pdev->device = pci_get_device(dev);
	pdev->dev.bsddev = dev;
	pbus = malloc(sizeof(*pbus), M_DEVBUF, M_WAITOK|M_ZERO);
	pbus->self = pdev;
	pdev->bus = pbus;
	pdev->bus->number = pci_get_bus(dev);
	return (pdev);
}
#endif
