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
