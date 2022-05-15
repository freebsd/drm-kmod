#include <linux/device.h>

/*
 * Custom devres actions allow inserting a simple function call
 * into the teadown sequence.
 */

struct action_devres {
	void *data;
	void (*action)(void *);
};

static void devm_action_release(struct device *dev, void *res)
{
	struct action_devres *devres = res;

	devres->action(devres->data);
}

/**
 * devm_add_action() - add a custom action to list of managed resources
 * @dev: Device that owns the action
 * @action: Function that should be called
 * @data: Pointer to data passed to @action implementation
 *
 * This adds a custom action to the list of managed resources so that
 * it gets executed as part of standard resource unwinding.
 */
int devm_add_action(struct device *dev, void (*action)(void *), void *data)
{
	struct action_devres *devres;

	devres = devres_alloc(devm_action_release,
			      sizeof(struct action_devres), GFP_KERNEL);
	if (!devres)
		return -ENOMEM;

	devres->data = data;
	devres->action = action;

	devres_add(dev, devres);
	return 0;
}
EXPORT_SYMBOL_GPL(devm_add_action);
