/* Public domain. */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/list.h>

#include <drm/drm_device.h>
#include <drm/drm_managed.h>

MALLOC_DECLARE(DRM_MEM_KMS);

struct drmm_node {
	void *p;
	size_t size;
	drmm_func_t func;
	struct list_head list;
};

void *
drmm_kzalloc(struct drm_device *dev, size_t size, int flags)
{
	void *p;
	struct drmm_node *node = malloc(sizeof(*node), DRM_MEM_KMS, flags | M_ZERO);
	if (node == NULL)
		return NULL;
	p = kzalloc(size, flags);
	if (p == NULL) {
		free(node, DRM_MEM_KMS);
		return NULL;
	}
	INIT_LIST_HEAD(&node->list);
	node->p = p;
	node->size = size;
	spin_lock(&dev->managed.lock);
	list_add(&node->list, &dev->managed.resources);
	spin_unlock(&dev->managed.lock);
	return p;
}

void *
drmm_kcalloc(struct drm_device *dev, size_t n, size_t size, int flags)
{
	void *p;
	struct drmm_node *node = malloc(sizeof(*node), DRM_MEM_KMS, flags | M_ZERO);
	if (node == NULL)
		return NULL;
	p = kcalloc(n, size, flags);
	if (p == NULL) {
		free(node, DRM_MEM_KMS);
		return NULL;
	}
	INIT_LIST_HEAD(&node->list);
	node->p = p;
	node->size = n * size;
	spin_lock(&dev->managed.lock);
	list_add(&node->list, &dev->managed.resources);
	spin_unlock(&dev->managed.lock);
	return p;
}

char *
drmm_kstrdup(struct drm_device *dev, const char *s, int flags)
{
	char *p;
	struct drmm_node *node = malloc(sizeof(*node), DRM_MEM_KMS, flags | M_ZERO);
	if (node == NULL)
		return NULL;
	p = kstrdup(s, flags);
	if (p == NULL) {
		free(node, DRM_MEM_KMS);
		return NULL;
	}
	INIT_LIST_HEAD(&node->list);
	node->p = p;
	node->size = strlen(s) + 1;
	spin_lock(&dev->managed.lock);
	list_add(&node->list, &dev->managed.resources);
	spin_unlock(&dev->managed.lock);
	return p;
}

void
drmm_kfree(struct drm_device *dev, void *p)
{
	struct drmm_node *n, *m = NULL;

	if (p == NULL)
		return;

	spin_lock(&dev->managed.lock);
	list_for_each_entry(n, &dev->managed.resources, list) {
		if (n->p == p) {
			list_del(&n->list);
			m = n;
			break;
		}
	}
	spin_unlock(&dev->managed.lock);
	
	if (m != NULL) {
		free(m->p, DRM_MEM_KMS);
		free(m, DRM_MEM_KMS);
	}
}

int
drmm_add_action(struct drm_device *dev, drmm_func_t f, void *cookie)
{
	struct drmm_node *node = malloc(sizeof(*node), DRM_MEM_KMS, M_WAITOK | M_ZERO);
	if (node == NULL)
		return -ENOMEM;
	INIT_LIST_HEAD(&node->list);
	node->func = f;
	node->p = cookie;
	spin_lock(&dev->managed.lock);
	list_add(&node->list, &dev->managed.resources);
	spin_unlock(&dev->managed.lock);

	return 0;
}

int
drmm_add_action_or_reset(struct drm_device *dev, drmm_func_t f, void *cookie)
{
	struct drmm_node *node = malloc(sizeof(*node), DRM_MEM_KMS, M_WAITOK | M_ZERO);
	if (node == NULL) {
		f(dev, cookie);
		return -ENOMEM;
	}
	INIT_LIST_HEAD(&node->list);
	node->func = f;
	node->p = cookie;
	spin_lock(&dev->managed.lock);
	list_add(&node->list, &dev->managed.resources);
	spin_unlock(&dev->managed.lock);

	return 0;
}

void
drm_managed_release(struct drm_device *dev)
{
	struct drmm_node *n, *t;
	list_for_each_entry_safe(n, t, &dev->managed.resources, list) {
		list_del(&n->list);
		if (n->func)
			n->func(dev, n->p);
		else
			free(n->p, DRM_MEM_KMS);
		free(n, DRM_MEM_KMS);
	}
}

void
drmm_add_final_kfree(struct drm_device *dev, void *p)
{
	dev->managed.final_kfree = p;
}
