#ifndef _LINUX_OF_H
#define _LINUX_OF_H

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/mod_devicetable.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/property.h>
#include <linux/list.h>
#include <linux/err.h>

struct device_node {
	const char *name;
	const char *type;
	struct fwnode_handle fwnode;

};

static inline bool
is_of_node(struct fwnode_handle *fwnode)
{
	return !IS_ERR_OR_NULL(fwnode) && fwnode->type == FWNODE_OF;
}

static inline struct device_node *
to_of_node(struct fwnode_handle *fwnode)
{
	return is_of_node(fwnode) ?
		container_of(fwnode, struct device_node, fwnode) : NULL;
}

static inline struct fwnode_handle
*of_node_to_fwnode(struct device_node *node)
{
	return node ? &node->fwnode : NULL;
}


static inline struct device_node *
of_node_get(struct device_node *node)
{
	return node;
}

static inline void of_node_put(struct device_node *node) { }

static inline int
of_node_to_nid(struct device_node *device)
{
	return (-1);
}

#endif
