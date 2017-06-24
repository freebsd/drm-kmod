
#ifndef _LINUX_IRQDOMAIN_H
#define _LINUX_IRQDOMAIN_H

#include <linux/types.h>
#include <linux/irqhandler.h>
#include <linux/of.h>
#include <linux/radix-tree.h>
#include <linux/irq.h>


struct irq_domain_ops;
extern struct mutex irq_domain_mutex; 
extern struct irq_domain *irq_default_domain;

extern struct mutex revmap_trees_mutex;
extern struct list_head irq_domain_list;

struct irq_domain {
	struct list_head link;
	const char *name;
	const struct irq_domain_ops *ops;
	void *host_data;
	unsigned int flags;

	struct fwnode_handle *fwnode;	
	

	irq_hw_number_t hwirq_max;
	unsigned int revmap_direct_max_irq;
	unsigned int revmap_size;
	struct radix_tree_root revmap_tree;
	unsigned int linear_revmap[];

};

enum irq_domain_bus_token {
	DOMAIN_BUS_ANY		= 0,
	DOMAIN_BUS_WIRED,
	DOMAIN_BUS_PCI_MSI,
	DOMAIN_BUS_PLATFORM_MSI,
	DOMAIN_BUS_NEXUS,
	DOMAIN_BUS_IPI,
	DOMAIN_BUS_FSL_MC_MSI,
};

enum {
	IRQ_DOMAIN_FLAG_HIERARCHY	= (1 << 0),
	IRQ_DOMAIN_FLAG_AUTO_RECURSIVE	= (1 << 1),
	IRQ_DOMAIN_FLAG_IPI_PER_CPU	= (1 << 2),
	IRQ_DOMAIN_FLAG_IPI_SINGLE	= (1 << 3),
	IRQ_DOMAIN_FLAG_NONCORE		= (1 << 16),
};


struct irq_domain_ops {
	int (*match)(struct irq_domain *d, struct device_node *node,
		     enum irq_domain_bus_token bus_token);
	int (*map)(struct irq_domain *d, unsigned int virq, irq_hw_number_t hw);
	void (*unmap)(struct irq_domain *d, unsigned int virq);
	int (*alloc)(struct irq_domain *d, unsigned int virq,
		     unsigned int nr_irqs, void *arg);
	void (*free)(struct irq_domain *d, unsigned int virq,
		     unsigned int nr_irqs);

};

static inline struct irq_data *
irq_get_irq_data(unsigned int irq)
{
	struct irq_desc *desc = irq_to_desc(irq);

	return desc ? &desc->irq_data : NULL;
}

static inline struct irq_data *
irq_domain_get_irq_data(struct irq_domain *domain, unsigned int virq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);

	return (irq_data && irq_data->domain == domain) ? irq_data : NULL;
}

static inline void
irq_domain_check_hierarchy(struct irq_domain *domain)
{
	/* Hierarchy irq_domains must implement callback alloc() */
	if (domain->ops->alloc)
		domain->flags |= IRQ_DOMAIN_FLAG_HIERARCHY;
}

static inline struct device_node *
irq_domain_get_of_node(struct irq_domain *d)
{
	return to_of_node(d->fwnode);
}

static inline int
irq_domain_alloc_descs(int virq, unsigned int cnt, irq_hw_number_t hwirq,
			   int node)
{
	unsigned int hint;

	if (virq >= 0) {
		virq = irq_alloc_descs(virq, virq, cnt, node);
	} else {
		hint = hwirq % nr_irqs;
		if (hint == 0)
			hint++;
		virq = irq_alloc_descs_from(hint, cnt, node);
		if (virq <= 0 && hint > 1)
			virq = irq_alloc_descs_from(1, cnt, node);
	}

	return virq;
}

static inline struct irq_domain *
__irq_domain_add(struct fwnode_handle *fwnode, int size,
		 irq_hw_number_t hwirq_max, int direct_max,
		 const struct irq_domain_ops *ops,
		 void *host_data)
{
	struct irq_domain *domain;
	struct device_node *of_node;

	of_node = to_of_node(fwnode);

	domain = kzalloc_node(sizeof(*domain) + (sizeof(unsigned int) * size),
			      GFP_KERNEL, of_node_to_nid(of_node));
	if (WARN_ON(!domain))
		return (NULL);

	of_node_get(of_node);

	/* Fill structure */
	INIT_RADIX_TREE(&domain->revmap_tree, GFP_KERNEL);
	domain->ops = ops;
	domain->host_data = host_data;
	domain->fwnode = fwnode;
	domain->hwirq_max = hwirq_max;
	domain->revmap_size = size;
	domain->revmap_direct_max_irq = direct_max;
	irq_domain_check_hierarchy(domain);

	mutex_lock(&irq_domain_mutex);
	list_add(&domain->link, &irq_domain_list);
	mutex_unlock(&irq_domain_mutex);
	return (domain);
}

static inline struct irq_domain *
irq_domain_add_linear(struct device_node *of_node,
		      unsigned int size,
		      const struct irq_domain_ops *ops,
		      void *host_data)
{
	return __irq_domain_add(of_node_to_fwnode(of_node), size, size, 0, ops, host_data);
}

static inline void
irq_domain_remove(struct irq_domain *domain)
{
	mutex_lock(&irq_domain_mutex);
	WARN_ON(domain->revmap_tree.height);
	list_del(&domain->link);

	if (unlikely(irq_default_domain == domain))
		irq_default_domain = NULL;
	mutex_unlock(&irq_domain_mutex);

	of_node_put(irq_domain_get_of_node(domain));
	kfree(domain);
}

static inline int
irq_domain_associate(struct irq_domain *domain, unsigned int virq,
			 irq_hw_number_t hwirq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);
	int ret;

	if (WARN(hwirq >= domain->hwirq_max,
		 "error: hwirq 0x%x is too large for %s\n", (int)hwirq, domain->name))
		return -EINVAL;
	if (WARN(!irq_data, "error: virq%i is not allocated", virq))
		return -EINVAL;
	if (WARN(irq_data->domain, "error: virq%i is already associated", virq))
		return -EINVAL;

	mutex_lock(&irq_domain_mutex);
	irq_data->hwirq = hwirq;
	irq_data->domain = domain;
	if (domain->ops->map) {
		ret = domain->ops->map(domain, virq, hwirq);
		if (ret != 0) {
			irq_data->domain = NULL;
			irq_data->hwirq = 0;
			mutex_unlock(&irq_domain_mutex);
			return (ret);
		}
		if (!domain->name && irq_data->chip)
			domain->name = irq_data->chip->name;
	}

	if (hwirq < domain->revmap_size) {
		domain->linear_revmap[hwirq] = virq;
	} else {
		mutex_lock(&revmap_trees_mutex);
		radix_tree_insert(&domain->revmap_tree, hwirq, irq_data);
		mutex_unlock(&revmap_trees_mutex);
	}
	mutex_unlock(&irq_domain_mutex);

	irq_clear_status_flags(virq, IRQ_NOREQUEST);
	return (0);
}

static inline unsigned int
irq_find_mapping(struct irq_domain *domain, irq_hw_number_t hwirq)
{
	struct irq_data *data;

	/* Look for default domain if nececssary */
	if (domain == NULL)
		domain = irq_default_domain;
	if (domain == NULL)
		return (0);

	if (hwirq < domain->revmap_direct_max_irq) {
		data = irq_domain_get_irq_data(domain, hwirq);
		if (data && data->hwirq == hwirq)
			return (hwirq);
	}
	if (hwirq < domain->revmap_size)
		return domain->linear_revmap[hwirq];

	rcu_read_lock();
	data = radix_tree_lookup(&domain->revmap_tree, hwirq);
	rcu_read_unlock();
	return (data ? data->irq : 0);
}

static inline unsigned int
irq_create_mapping(struct irq_domain *domain, irq_hw_number_t hwirq)
{
	struct device_node *of_node;
	int virq;

	if (domain == NULL)
		domain = irq_default_domain;
	if (domain == NULL) {
		WARN(1, "%s(, %lx) called with NULL domain\n", __func__, hwirq);
		return 0;
	}
	of_node = irq_domain_get_of_node(domain);

	virq = irq_find_mapping(domain, hwirq);
	if (virq) 
		return (virq);
	virq = irq_domain_alloc_descs(-1, 1, hwirq, of_node_to_nid(of_node));
	if (virq <= 0)
		return (0);
	if (irq_domain_associate(domain, virq, hwirq)) {
		irq_free_desc(virq);
		return (0);
	}
	return (virq);
}

#endif
