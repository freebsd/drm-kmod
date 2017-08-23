/*
 * Componentized device handling.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This is work in progress.  We gather up the component devices into a list,
 * and bind them when instructed.  At the moment, we're specific to the DRM
 * subsystem, and only handles one master device, but this doesn't have to be
 * the case.
 */

#ifndef _LINUX_GPLV2_COMPONENT_H_
#define _LINUX_GPLV2_COMPONENT_H_

#include <linux/stddef.h>
#include <linux/device.h>

extern struct mutex component_mutex;

extern struct list_head linux_component_list;
extern struct list_head linux_component_masters;

struct linux_component;

struct component_master_ops {
	int (*bind)(struct device *master);
	void (*unbind)(struct device *master);
};

struct component_ops {
	int (*bind)(struct device *comp, struct device *master,
		    void *master_data);
	void (*unbind)(struct device *comp, struct device *master,
		       void *master_data);
};

struct linux_component_match_array {
	void *data;
	int (*compare)(struct device *, void *);
	void (*release)(struct device *, void *);
	struct linux_component *component;
	bool duplicate;
};

struct linux_component_match {
	size_t alloc;
	size_t num;
	struct linux_component_match_array *compare;
};

struct linux_component_master {
	struct list_head node;
	bool bound;

	const struct component_master_ops *ops;
	struct device *dev;
	struct linux_component_match *match;
};

struct linux_component {
	struct list_head node;
	struct linux_component_master *master;
	bool bound;

	const struct component_ops *ops;
	struct device *dev;
};

static inline void
remove_component(struct linux_component_master *master, struct linux_component *c)
{
	size_t i;

	/* Detach the component from this master. */
	for (i = 0; i < master->match->num; i++)
		if (master->match->compare[i].component == c)
			master->match->compare[i].component = NULL;
}


static inline struct linux_component *
find_component(struct linux_component_master *master,
	int (*compare)(struct device *, void *), void *compare_data)
{
	struct linux_component *c;

	list_for_each_entry(c, &linux_component_list, node) {
		if (c->master && c->master != master)
			continue;

		if (compare(c->dev, compare_data))
			return c;
	}

	return NULL;
}


static inline int
find_components(struct linux_component_master *master)
{
	struct linux_component_match *match = master->match;
	size_t i;
	int ret = 0;

	/*
	 * Scan the array of match functions and attach
	 * any components which are found to this master.
	 */
	for (i = 0; i < match->num; i++) {
		struct linux_component_match_array *mc = &match->compare[i];
		struct linux_component *c;

		dev_dbg(master->dev, "Looking for component %zu\n", i);

		if (match->compare[i].component)
			continue;

		c = find_component(master, mc->compare, mc->data);
		if (!c) {
			ret = -ENXIO;
			break;
		}

		dev_dbg(master->dev, "found component %s, duplicate %u\n", dev_name(c->dev), !!c->master);

		/* Attach this component to the master */
		match->compare[i].duplicate = !!c->master;
		match->compare[i].component = c;
		c->master = master;
	}
	return ret;
}

static inline int
try_to_bring_up_master(struct linux_component_master *master,
	struct linux_component *component)
{
	int ret;

	dev_dbg(master->dev, "trying to bring up master\n");

	if (find_components(master)) {
		dev_dbg(master->dev, "master has incomplete components\n");
		return 0;
	}

	if (component && component->master != master) {
		dev_dbg(master->dev, "master is not for this component (%s)\n",
			dev_name(component->dev));
		return 0;
	}

	if (!devres_open_group(master->dev, NULL, GFP_KERNEL))
		return -ENOMEM;

	/* Found all components */
	ret = master->ops->bind(master->dev);
	if (ret < 0) {
		devres_release_group(master->dev, NULL);
		dev_info(master->dev, "master bind failed: %d\n", ret);
		return ret;
	}

	master->bound = true;
	return 1;
}

static inline int
try_to_bring_up_masters(struct linux_component *component)
{
	struct linux_component_master *m;
	int ret = 0;

	list_for_each_entry(m, &linux_component_masters, node) {
		if (!m->bound) {
			ret = try_to_bring_up_master(m, component);
			if (ret != 0)
				break;
		}
	}

	return ret;
}

static inline void
take_down_master(struct linux_component_master *master)
{
	if (master->bound) {
		master->ops->unbind(master->dev);
		devres_release_group(master->dev, NULL);
		master->bound = false;
	}
}

static inline int
component_add(struct device *dev, const struct component_ops *ops)
{
	struct linux_component *component;
	int ret;

	component = kzalloc(sizeof(*component), GFP_KERNEL);
	if (!component)
		return -ENOMEM;

	component->ops = ops;
	component->dev = dev;

	dev_dbg(dev, "adding component (ops %ps)\n", ops);

	mutex_lock(&component_mutex);
	list_add_tail(&component->node, &linux_component_list);

	ret = try_to_bring_up_masters(component);
	if (ret < 0) {
		if (component->master)
			remove_component(component->master, component);
		list_del(&component->node);

		kfree(component);
	}
	mutex_unlock(&component_mutex);

	return (ret < 0 ? ret : 0);
}

static inline void
component_del(struct device *dev, const struct component_ops *ops)
{
	struct linux_component *c, *component = NULL;

	mutex_lock(&component_mutex);
	list_for_each_entry(c, &linux_component_list, node)
		if (c->dev == dev && c->ops == ops) {
			list_del(&c->node);
			component = c;
			break;
		}

	if (component && component->master) {
		take_down_master(component->master);
		remove_component(component->master, component);
	}

	mutex_unlock(&component_mutex);

	WARN_ON(!component);
	kfree(component);
}

int component_bind_all(struct device *master, void *master_data);
void component_unbind_all(struct device *master, void *master_data);


static inline void
component_master_del(struct device *dev, const struct component_master_ops *ops)
{
	UNIMPLEMENTED();
}


int component_master_add_with_match(struct device *,
	const struct component_master_ops *, struct linux_component_match *);
void component_match_add_release(struct device *master,
	struct linux_component_match **matchptr,
	void (*release)(struct device *, void *),
	int (*compare)(struct device *, void *), void *compare_data);

static inline void component_match_add(struct device *master,
	struct linux_component_match **matchptr,
	int (*compare)(struct device *, void *), void *compare_data)
{
	component_match_add_release(master, matchptr, NULL, compare,
				    compare_data);
}

#endif /* _LINUX_GPLV2_COMPONENT_H_ */
