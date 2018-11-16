#ifndef	_LINUX_GPLV2_DEVICE_H_
#define	_LINUX_GPLV2_DEVICE_H_

#include_next <linux/device.h>

#include <linux/numa.h>
#include <linux/pm.h>

typedef void (*dr_release_t)(struct device *dev, void *res);
typedef int (*dr_match_t)(struct device *dev, void *res, void *match_data);

struct devres_node {
	struct list_head		entry;
	dr_release_t			release;
};

struct devres {
	struct devres_node		node;
	unsigned long long		data[];	/* guarantee ull alignment */
};

struct devres_group {
	struct devres_node		node[2];
	void				*id;
	int				color;
};

static inline void *
devm_kmalloc(struct device *dev, size_t size, gfp_t gfp)
{
	/*
	 * this allocates managed memory which is automatically freed
	 * on unload - we're instead just leaking for now
	 */
	DODGY();
	return (kmalloc(size, gfp));
}

static inline void *
devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
{
	return devm_kmalloc(dev, size, gfp | __GFP_ZERO);
}

/*
 * drivers/base/devres.c - device resource management
 *
 * Copyright (c) 2006  SUSE Linux Products GmbH
 * Copyright (c) 2006  Tejun Heo <teheo@suse.de>
 *
 * This file is released under the GPLv2.
 */

static inline void
group_open_release(struct device *dev, void *res)
{
	/* noop */
}

static inline void
group_close_release(struct device *dev, void *res)
{
	/* noop */
}

static inline struct devres_group *
node_to_group(struct devres_node *node)
{
	if (node->release == &group_open_release)
		return container_of(node, struct devres_group, node[0]);
	if (node->release == &group_close_release)
		return container_of(node, struct devres_group, node[1]);
	return NULL;
}


static inline void
add_dr(struct device *dev, struct devres_node *node)
{
	BUG_ON(!list_empty(&node->entry));
	list_add_tail(&node->entry, &dev->devres_head);
}


static inline void *
devres_open_group(struct device *dev, void *id, gfp_t gfp)
{
	struct devres_group *grp;
	unsigned long flags __unused;

	grp = kmalloc(sizeof(*grp), gfp);
	if (unlikely(!grp))
		return NULL;

	grp->node[0].release = &group_open_release;
	grp->node[1].release = &group_close_release;
	INIT_LIST_HEAD(&grp->node[0].entry);
	INIT_LIST_HEAD(&grp->node[1].entry);
	grp->id = grp;
	if (id)
		grp->id = id;

	spin_lock_irqsave(&dev->devres_lock, flags);
	add_dr(dev, &grp->node[0]);
	spin_unlock_irqrestore(&dev->devres_lock, flags);
	return grp->id;
}

static inline struct devres_group *
find_group(struct device *dev, void *id)
{
	struct devres_node *node;

	list_for_each_entry_reverse(node, &dev->devres_head, entry) {
		struct devres_group *grp;
		if (node->release != &group_open_release)
			continue;
		grp = container_of(node, struct devres_group, node[0]);
		if (id) {
			if (grp->id == id)
				return grp;
		} else if (list_empty(&grp->node[1].entry))
			return grp;
	}
	return NULL;
}

static inline void
devres_close_group(struct device *dev, void *id)
{
	struct devres_group *grp;
	unsigned long flags __unused;

	spin_lock_irqsave(&dev->devres_lock, flags);

	grp = find_group(dev, id);
	if (grp)
		add_dr(dev, &grp->node[1]);
	else
		WARN_ON(1);

	spin_unlock_irqrestore(&dev->devres_lock, flags);
}

static inline int
linux_remove_nodes(struct device *dev,
			struct list_head *first, struct list_head *end,
			struct list_head *todo)
{
	int cnt = 0, nr_groups = 0;
	struct list_head *cur;

	cur = first;
	while (cur != end) {
		struct devres_node *node;
		struct devres_group *grp;

		node = list_entry(cur, struct devres_node, entry);
		cur = cur->next;

		grp = node_to_group(node);
		if (grp) {
			grp->color = 0;
			nr_groups++;
		} else {
			if (&node->entry == first)
				first = first->next;
			list_move_tail(&node->entry, todo);
			cnt++;
		}
	}

	if (!nr_groups)
		return cnt;

	cur = first;
	while (cur != end) {
		struct devres_node *node;
		struct devres_group *grp;

		node = list_entry(cur, struct devres_node, entry);
		cur = cur->next;

		grp = node_to_group(node);
		BUG_ON(!grp || list_empty(&grp->node[0].entry));

		grp->color++;
		if (list_empty(&grp->node[1].entry))
			grp->color++;

		BUG_ON(grp->color <= 0 || grp->color > 2);
		if (grp->color == 2) {
			list_move_tail(&grp->node[0].entry, todo);
			list_del_init(&grp->node[1].entry);
		}
	}

	return cnt;
}

static inline int
linux_release_nodes(struct device *dev, struct list_head *first,
			 struct list_head *end, unsigned long flags)
{
	LIST_HEAD(todo);
	int cnt;
	struct devres *dr, *tmp;

	cnt = linux_remove_nodes(dev, first, end, &todo);

	spin_unlock_irqrestore(&dev->devres_lock, flags);
	list_for_each_entry_safe_reverse(dr, tmp, &todo, node.entry) {
		dr->node.release(dev, dr->data);
		kfree(dr);
	}

	return cnt;
}

extern void devres_add(struct device *dev, void *res);
extern void *devres_alloc_node(dr_release_t release, size_t size, gfp_t gfp,
			       int nid);
extern void devres_free(void *res);
extern int devres_release(struct device *dev, dr_release_t release,
			  dr_match_t match, void *match_data);
extern void *devres_remove(struct device *dev, dr_release_t release,
			   dr_match_t match, void *match_data);

static inline void *
devres_alloc(dr_release_t release, size_t size, gfp_t gfp)
{
	return devres_alloc_node(release, size, gfp, NUMA_NO_NODE);
}

static inline int
devres_release_group(struct device *dev, void *id)
{
	struct devres_group *grp;
	unsigned long flags;
	int cnt = 0;

	spin_lock_irqsave(&dev->devres_lock, flags);

	grp = find_group(dev, id);
	if (grp) {
		struct list_head *first = &grp->node[0].entry;
		struct list_head *end = &dev->devres_head;

		if (!list_empty(&grp->node[1].entry))
			end = grp->node[1].entry.next;

		cnt = linux_release_nodes(dev, first, end, flags);
	} else {
		WARN_ON(1);
		spin_unlock_irqrestore(&dev->devres_lock, flags);
	}

	return cnt;
}

/*
 * hwmon.h - part of lm_sensors, Linux kernel modules for hardware monitoring
 *
 * This file declares helper functions for the sysfs class "hwmon",
 * for use by sensors drivers.
 *
 * Copyright (C) 2005 Mark M. Hoffman <mhoffman@lightlink.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#include <linux/idr.h>

extern struct ida *hwmon_idap;

static inline char *
strpbrk(const char *s, const char *b)
{
	const char *p;

	do {
		for (p = b; *p != '\0' && *p != *s; ++p)
			;
		if (*p != '\0')
			return ((char *)(uintptr_t)s);
	} while (*s++);

	return (NULL);
}

static struct class hwmon_class = {
	.name = "hwmon",
	.owner = THIS_MODULE,
};

#define HWMON_ID_PREFIX "hwmon"
#define HWMON_ID_FORMAT HWMON_ID_PREFIX "%d"

struct hwmon_device {
	const char *name;
	struct device dev;
};

static inline struct device *
hwmon_device_register_with_groups(struct device *dev, const char *name,
				  void *drvdata,
				  const struct attribute_group **groups)
{
	struct hwmon_device *hwdev;
	int err, id;

	/* Do not accept invalid characters in hwmon name attribute */
	if (name && (!strlen(name) || strpbrk(name, "-* \t\n")))
		return ERR_PTR(-EINVAL);

	id = ida_simple_get(hwmon_idap, 0, 0, GFP_KERNEL);
	if (id < 0)
		return ERR_PTR(id);

	hwdev = kzalloc(sizeof(*hwdev), GFP_KERNEL);
	if (hwdev == NULL) {
		err = -ENOMEM;
		goto ida_remove;
	}

	hwdev->name = name;
	hwdev->dev.class = &hwmon_class;
	hwdev->dev.parent = dev;
	hwdev->dev.groups = groups;
	dev_set_drvdata(&hwdev->dev, drvdata);
	dev_set_name(&hwdev->dev, HWMON_ID_FORMAT, id);
	err = device_register(&hwdev->dev);
	if (err)
		goto free;

	return &hwdev->dev;

free:
	kfree(hwdev);
ida_remove:
	ida_simple_remove(hwmon_idap, id);
	return ERR_PTR(err);
}


static inline void
hwmon_device_unregister(struct device *dev)
{
	int id;

	if (likely(sscanf(dev_name(dev), HWMON_ID_FORMAT, &id) == 1)) {
		device_unregister(dev);
		ida_simple_remove(hwmon_idap, id);
	} else
		device_printf(dev->bsddev,
			"hwmon_device_unregister() failed: bad class ID!\n");
}

#endif /* _LINUX_GPLV2_DEVICE_H_ */
