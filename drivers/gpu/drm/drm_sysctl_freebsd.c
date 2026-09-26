/*-
 * Copyright 2003 Eric Anholt
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * ERIC ANHOLT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

/** @file drm_sysctl.c
 * Implementation of various sysctls for controlling DRM behavior and reporting
 * debug information.
 */

#include <drm/drm_auth.h>
#include <drm/drm_drv.h>
#include <drm/drm_print.h>
#include <drm/drm_vblank.h>
#include <uapi/drm/drm.h>
#include "drm_internal.h"

#include <sys/sysctl.h>


static int drm_add_busid_modesetting(struct drm_device *dev, struct sysctl_ctx_list *ctx,
	   struct sysctl_oid *top);

SYSCTL_DECL(_hw_drm);
SYSCTL_DECL(_hw_dri);

#define DRM_SYSCTL_HANDLER_ARGS	(SYSCTL_HANDLER_ARGS)

extern int drm_vblank_offdelay;
extern unsigned int drm_timestamp_precision;

/*
 * hw.dri itself is a static node of this module (drm_os_freebsd.c), and so
 * are these two leaves: they describe drm.ko, not any one device, and must
 * live exactly as long as drm.ko does.
 */
SYSCTL_INT(_hw_dri, OID_AUTO, vblank_offdelay, CTLFLAG_RW,
    &drm_vblank_offdelay, 0, "");
SYSCTL_UINT(_hw_dri, OID_AUTO, timestamp_precision, CTLFLAG_RW,
    &drm_timestamp_precision, 0, "");

static int	   drm_name_info DRM_SYSCTL_HANDLER_ARGS;
static int	   drm_clients_info DRM_SYSCTL_HANDLER_ARGS;
static int	   drm_vblank_info DRM_SYSCTL_HANDLER_ARGS;

struct drm_sysctl_list {
	const char *name;
	int	   (*f) DRM_SYSCTL_HANDLER_ARGS;
} drm_sysctl_list[] = {
	{"name",    drm_name_info},
	{"clients", drm_clients_info},
	{"vblank",    drm_vblank_info},
};
#define DRM_SYSCTL_ENTRIES (sizeof(drm_sysctl_list)/sizeof(drm_sysctl_list[0]))

struct drm_sysctl_info {
	struct sysctl_ctx_list ctx;
	char		       name[2];
};

int
drm_sysctl_init(struct drm_device *dev)
{
	struct drm_sysctl_info *info;
	struct sysctl_oid *oid;
	struct sysctl_oid *top;
	int		  i;

	info = malloc(sizeof *info, DRM_MEM_DRIVER, M_WAITOK | M_ZERO);
	sysctl_ctx_init(&info->ctx);
	dev->sysctl = info;

	/*
	 * Only this device's own hw.dri.<N> subtree goes into its context.
	 * hw.dri is static; claiming it here (as SYSCTL_ADD_NODE on an existing
	 * node does) made sysctl_ctx_free() fail with EBUSY on every unload --
	 * a static OID cannot be removed -- having removed nothing, after which
	 * the hw.dri.<N> OIDs pointed into freed memory.
	 */

	/* Find the next free slot under hw.dri */
	i = 0;
#ifdef SYSCTL_FOREACH
	SYSCTL_FOREACH(oid, SYSCTL_STATIC_CHILDREN(_hw_dri))
#else
	SLIST_FOREACH(oid, SYSCTL_STATIC_CHILDREN(_hw_dri), oid_link)
#endif
	{
		if (i == oid->oid_name[0] - '0' && oid->oid_name[1] == 0)
			i++;
	}
	if (i > 9) {
		drm_sysctl_cleanup(dev);
		return (-ENOSPC);
	}

	dev->sysctl_node_idx = i;
	/* Add the hw.dri.x for our device */
	info->name[0] = '0' + i;
	info->name[1] = 0;
	top = SYSCTL_ADD_NODE(&info->ctx, SYSCTL_STATIC_CHILDREN(_hw_dri),
	    OID_AUTO, info->name, CTLFLAG_RW, NULL, NULL);
	if (!top) {
		drm_sysctl_cleanup(dev);
		return (-ENOMEM);
	}

	for (i = 0; i < DRM_SYSCTL_ENTRIES; i++) {
		oid = SYSCTL_ADD_OID(&info->ctx,
			SYSCTL_CHILDREN(top),
			OID_AUTO,
			drm_sysctl_list[i].name,
			CTLTYPE_STRING | CTLFLAG_RD,
			dev,
			0,
			drm_sysctl_list[i].f,
			"A",
			NULL);
		if (!oid) {
			drm_sysctl_cleanup(dev);
			return (-ENOMEM);
		}
	}
#ifdef notyet
	if (dev->driver->sysctl_init != NULL)
		dev->driver->sysctl_init(dev, &info->ctx, top);
#endif

	drm_add_busid_modesetting(dev, &info->ctx, top);

	return (0);
}

int drm_sysctl_cleanup(struct drm_device *dev)
{
	int error;

	if (dev->sysctl == NULL)
		return (0);

	error = sysctl_ctx_free(&dev->sysctl->ctx);
	free(dev->sysctl, DRM_MEM_DRIVER);
	dev->sysctl = NULL;
	return (-error);
}

static int
drm_add_busid_modesetting(struct drm_device *dev, struct sysctl_ctx_list *ctx,
    struct sysctl_oid *top)
{
	struct sysctl_oid *oid;
	int domain, bus, slot, func;
	struct pci_dev *pdev = to_pci_dev(dev->dev);

	domain = pci_domain_nr(pdev->bus);
	bus    = pdev->bus->number;
	slot   = PCI_SLOT(pdev->devfn);
	func   = PCI_FUNC(pdev->devfn);

	snprintf(dev->busid_str, sizeof(dev->busid_str),
	    "pci:%04x:%02x:%02x.%d", domain, bus, slot, func);
	oid = SYSCTL_ADD_STRING(ctx, SYSCTL_CHILDREN(top), OID_AUTO, "busid",
	    CTLFLAG_RD, dev->busid_str, 0, NULL);
	if (oid == NULL)
		return (-ENOMEM);
	dev->modesetting = (dev->driver->driver_features & DRIVER_MODESET) != 0;
	oid = SYSCTL_ADD_INT(ctx, SYSCTL_CHILDREN(top), OID_AUTO,
	    "modesetting", CTLFLAG_RD, &dev->modesetting, 0, NULL);
	if (oid == NULL)
		return (-ENOMEM);

	return (0);
}


#define DRM_SYSCTL_PRINT(fmt, arg...)				\
do {								\
	snprintf(buf, sizeof(buf), fmt, ##arg);			\
	retcode = SYSCTL_OUT(req, buf, strlen(buf));		\
	if (retcode)						\
		goto done;					\
} while (0)

static int drm_name_info DRM_SYSCTL_HANDLER_ARGS
{
	struct drm_device *dev = arg1;
	struct drm_minor *minor;
	struct drm_master *master;
	char buf[128];
	int retcode;
	int hasunique = 0;

	/* FIXME: This still uses primary minor. */
	minor = dev->primary;
	DRM_SYSCTL_PRINT("%s 0x%jx", dev->driver->name,
	    (uintmax_t)dev2udev(minor->bsd_device));

	mutex_lock(&dev->struct_mutex);
	master = dev->master;
	if (master != NULL && master->unique) {
		snprintf(buf, sizeof(buf), " %s", master->unique);
		hasunique = 1;
	}
	mutex_unlock(&dev->struct_mutex);

	if (hasunique)
		SYSCTL_OUT(req, buf, strlen(buf));

	SYSCTL_OUT(req, "", 1);

done:
	return retcode;
}

static int drm_clients_info DRM_SYSCTL_HANDLER_ARGS
{
	struct drm_device *dev = arg1;
	struct drm_file *priv, *tempprivs;
	char buf[128];
	int retcode;
	int privcount, i;

	mutex_lock(&dev->struct_mutex);

	privcount = 0;
	list_for_each_entry(priv, &dev->filelist, lhead)
		privcount++;

	tempprivs = malloc(sizeof(struct drm_file) * privcount, DRM_MEM_DRIVER,
	    M_NOWAIT);
	if (tempprivs == NULL) {
		mutex_unlock(&dev->struct_mutex);
		return ENOMEM;
	}
	i = 0;
	list_for_each_entry(priv, &dev->filelist, lhead)
		tempprivs[i++] = *priv;

	mutex_unlock(&dev->struct_mutex);

	DRM_SYSCTL_PRINT(
	    "\na dev            pid   uid      magic     ioctls\n");
	for (i = 0; i < privcount; i++) {
		priv = &tempprivs[i];
		DRM_SYSCTL_PRINT("%c %-12s %5d %5d %10u %10lu\n",
			       priv->authenticated ? 'y' : 'n',
			       devtoname(priv->minor->bsd_device),
			       priv->pid,
				 0,
			       priv->magic,
			       priv->ioctl_count);
	}

	SYSCTL_OUT(req, "", 1);
done:
	free(tempprivs, DRM_MEM_DRIVER);
	return retcode;
}

static int drm_vblank_info DRM_SYSCTL_HANDLER_ARGS
{
	struct drm_device *dev = arg1;
	char buf[128];
	int retcode;
	int i;

	mutex_lock(&dev->struct_mutex);
	DRM_SYSCTL_PRINT("\ncrtc ref count    last     enabled inmodeset\n");
	if (dev->vblank == NULL)
		goto done;
	for (i = 0 ; i < dev->num_crtcs ; i++) {
		DRM_SYSCTL_PRINT("  %02d  %02d %08d %08d %02d      %02d\n",
		    i, dev->vblank[i].refcount.counter,
		    dev->vblank[i].count,
		    dev->vblank[i].last,
		    dev->vblank[i].enabled,
		    dev->vblank[i].inmodeset);
	}
done:
	mutex_unlock(&dev->struct_mutex);

	SYSCTL_OUT(req, "", -1);
	return retcode;
}
