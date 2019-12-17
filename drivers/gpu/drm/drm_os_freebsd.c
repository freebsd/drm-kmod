
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <drm/drmP.h>

#include <dev/agp/agpreg.h>
#include <dev/pci/pcireg.h>
#include <linux/cdev.h>
#undef cdev

devclass_t drm_devclass;
const char *fb_mode_option = NULL;

MALLOC_DEFINE(DRM_MEM_DMA, "drm_dma", "DRM DMA Data Structures");
MALLOC_DEFINE(DRM_MEM_DRIVER, "drm_driver", "DRM DRIVER Data Structures");
MALLOC_DEFINE(DRM_MEM_KMS, "drm_kms", "DRM KMS Data Structures");

SYSCTL_NODE(_dev, OID_AUTO, drm, CTLFLAG_RW, 0, "DRM args");
SYSCTL_INT(_dev_drm, OID_AUTO, drm_debug, CTLFLAG_RWTUN, &drm_debug, 0, "drm debug flags");
extern int skip_ddb;
SYSCTL_INT(_dev_drm, OID_AUTO, skip_ddb, CTLFLAG_RWTUN, &skip_ddb, 0, "go straight to dumping core");
#if defined(DRM_DEBUG_LOG_ALL)
int drm_debug_persist = 1;
#else
int drm_debug_persist = 0;
#endif
SYSCTL_INT(_dev_drm, OID_AUTO, drm_debug_persist, CTLFLAG_RWTUN, &drm_debug_persist, 0, "keep drm debug flags post-load");
int drm_panic_on_error = 0;
SYSCTL_INT(_dev_drm, OID_AUTO, error_panic, CTLFLAG_RWTUN, &drm_panic_on_error, 0, "panic if an ERROR is hit");
int drm_always_interruptible;
SYSCTL_INT(_dev_drm, OID_AUTO, always_interruptible, CTLFLAG_RWTUN, &drm_always_interruptible, 0, "always allow a thread to be interrupted in driver wait");

static struct callout reset_debug_log_handle;

static void
clear_debug_func(void *arg __unused)
{
	drm_debug = 0;
}

void
cancel_reset_debug_log(void)
{
	callout_stop(&reset_debug_log_handle);
}

static void
reset_debug_log(void)
{
	if (drm_debug_persist)
		return;

	if (!callout_pending(&reset_debug_log_handle))
		callout_reset(&reset_debug_log_handle, 10*hz, clear_debug_func, NULL);
}

static int
sysctl_pci_id(SYSCTL_HANDLER_ARGS)
{
	uint16_t vid = arg2 & 0xFFFF;
	uint16_t pid = (arg2 >> 16) & 0xFFFF;
	char buf[32];

	snprintf(buf, sizeof(buf), "%x:%x", vid, pid);
	return (sysctl_handle_string(oidp, buf, sizeof(buf), req));
}

int
drm_dev_alias(struct device *ldev, struct drm_minor *minor, const char *minor_str)
{
	struct sysctl_oid_list *oid_list, *child;
	struct sysctl_ctx_list *ctx_list;
	struct linux_cdev *cdevp;
	struct sysctl_oid *node;
	device_t dev = ldev->parent->bsddev;
	char buf[32];
	char *devbuf;
	u32 tmp;

	MPASS(dev != NULL);
	ctx_list = device_get_sysctl_ctx(dev);
	child = SYSCTL_CHILDREN(device_get_sysctl_tree(dev));
	snprintf(buf, sizeof(buf), "%d", minor->index);
	node = SYSCTL_ADD_NODE(ctx_list, SYSCTL_STATIC_CHILDREN(_dev_drm), OID_AUTO, buf,
	    CTLFLAG_RD, NULL, "DRM properties");
	oid_list = SYSCTL_CHILDREN(node);
	tmp = pci_get_vendor(dev) + ((u32)pci_get_device(dev) << 16);
	SYSCTL_ADD_PROC(ctx_list, oid_list, OID_AUTO, "PCI_ID",
	    CTLTYPE_STRING | CTLFLAG_RD, NULL, tmp,
	    sysctl_pci_id, "A", "PCI vendor and device ID");

	/*
	 * FreeBSD won't automaticaly create the corresponding device
	 * node as linux must so we find the corresponding one created by
	 * register_chrdev in drm_drv.c and alias it.
	 */
	snprintf(buf, sizeof(buf), "dri/%s", minor_str);
	cdevp = linux_find_cdev("drm", DRM_MAJOR, minor->index);
	MPASS(cdevp != NULL);
	if (cdevp == NULL)
		return (-ENXIO);
	minor->bsd_device = cdevp->cdev;
	make_dev_alias(cdevp->cdev, buf, minor->index);
	reset_debug_log();
	return (0);
}

static const drm_pci_id_list_t *
drm_find_description(int vendor, int device, const drm_pci_id_list_t *idlist)
{
	int i = 0;

	for (i = 0; idlist[i].vendor != 0; i++) {
		if ((idlist[i].vendor == vendor) &&
		    ((idlist[i].device == device) ||
		    (idlist[i].device == 0))) {
			return (&idlist[i]);
		}
	}
	return (NULL);
}


/*
 * drm_probe_helper: called by a driver at the end of its probe
 * method.
 */
int
drm_probe_helper(device_t kdev, const drm_pci_id_list_t *idlist)
{
	DRM_DEBUG("entering %s\n", __func__);

	const drm_pci_id_list_t *id_entry;
	int vendor, device;

	vendor = pci_get_vendor(kdev);
	device = pci_get_device(kdev);

	if (pci_get_class(kdev) != PCIC_DISPLAY ||
	    (pci_get_subclass(kdev) != PCIS_DISPLAY_VGA &&
	     pci_get_subclass(kdev) != PCIS_DISPLAY_OTHER))
		return (-ENXIO);

	id_entry = drm_find_description(vendor, device, idlist);
	if (id_entry != NULL) {
		if (device_get_desc(kdev) == NULL) {
			DRM_DEBUG("%s desc: %s\n",
			    device_get_nameunit(kdev), id_entry->name);
			device_set_desc(kdev, id_entry->name);
		}
		return (0);
	}

	return (-ENXIO);
}

int
drm_generic_detach(device_t kdev)
{
	struct drm_device *dev;
	int i;

	dev = device_get_softc(kdev);

	drm_put_dev(dev);
#if 0
	/* Clean up PCI resources allocated by drm_bufs.c.  We're not really
	 * worried about resource consumption while the DRM is inactive (between
	 * lastclose and firstopen or unload) because these aren't actually
	 * taking up KVA, just keeping the PCI resource allocated.
	 */
	for (i = 0; i < DRM_MAX_PCI_RESOURCE; i++) {
		if (dev->pcir[i] == NULL)
			continue;
		bus_release_resource(dev->dev->bsddev, SYS_RES_MEMORY,
		    dev->pcirid[i], dev->pcir[i]);
		dev->pcir[i] = NULL;
	}

#endif	
	if (pci_disable_busmaster(dev->dev->bsddev))
		DRM_ERROR("Request to disable bus-master failed.\n");

	return (0);
}

static int
drm_device_find_capability(struct drm_device *dev, int cap)
{

	return (pci_find_cap(dev->dev->bsddev, cap, NULL) == 0);
}

#if 0
int
drm_pci_device_is_agp(struct drm_device *dev)
{
	if (dev->driver->device_is_agp != NULL) {
		int ret;

		/* device_is_agp returns a tristate, 0 = not AGP, 1 = definitely
		 * AGP, 2 = fall back to PCI capability
		 */
		ret = (*dev->driver->device_is_agp)(dev);
		if (ret != DRM_MIGHT_BE_AGP)
			return ret;
	}

	return (drm_device_find_capability(dev, PCIY_AGP));
}
#endif

int
drm_pci_device_is_pcie(struct drm_device *dev)
{
	DRM_DEBUG("entering %s\n", __func__);

	return (drm_device_find_capability(dev, PCIY_EXPRESS));
}

#if 0
void
drm_clflush_pages(vm_page_t *pages, unsigned long num_pages)
{

#if defined(__i386__) || defined(__amd64__)
	pmap_invalidate_cache_pages(pages, num_pages);
#else
	DRM_ERROR("drm_clflush_pages not implemented on this architecture");
#endif
}

void
drm_clflush_sg(struct sg_table *st)
{
#if defined(__i386__) || defined(__amd64__)
		struct scatterlist *sg;
		int i;

		mb();
		for_each_sg(st->sgl, sg, st->nents, i)
			drm_clflush_pages(&sg_page(sg), 1);
		mb();

		return;
#else
	printk(KERN_ERR "Architecture has no drm_cache.c support\n");
	WARN_ON_ONCE(1);
#endif
}

void
drm_clflush_virt_range(void *addr, unsigned long length)
{

#if defined(__i386__) || defined(__amd64__)
	pmap_invalidate_cache_range((vm_offset_t)addr,
	    (vm_offset_t)addr + length, TRUE);
#else
	DRM_ERROR("drm_clflush_virt_range not implemented on this architecture");
#endif
}
#endif

#define to_drm_minor(d) container_of(d, struct drm_minor, kdev)
#define to_drm_connector(d) container_of(d, struct drm_connector, kdev)

static char *drm_devnode(struct device *dev, umode_t *mode)
{
	DRM_DEBUG("entering %s\n", __func__);
	return kasprintf(GFP_KERNEL, "dri/%s", dev_name(dev));
}

#include <linux/pm_runtime.h>
int
drm_generic_suspend(device_t kdev)
{
	struct drm_device *dev;
	int error;

	DRM_DEBUG_KMS("Starting suspend\n");

	dev = device_get_softc(kdev);
	error = pm_generic_suspend(dev->dev);
	if (error)
		goto out;

	error = bus_generic_suspend(kdev);

out:
	DRM_DEBUG_KMS("Finished suspend: %d\n", error);

	return error;
}

int
drm_generic_resume(device_t kdev)
{
	struct drm_device *dev;
	int error;

	DRM_DEBUG_KMS("Starting resume\n");

	dev = device_get_softc(kdev);
	error = pm_generic_resume(dev->dev);
	if (error)
		goto out;

	error = bus_generic_resume(kdev);

out:
	DRM_DEBUG_KMS("Finished resume: %d\n", error);

	return error;
}

static int
drm_modevent(module_t mod, int type, void *data)
{
	switch (type) {
	case MOD_LOAD:
		TUNABLE_INT_FETCH("drm.debug", &drm_debug);
		callout_init(&reset_debug_log_handle, 1);
		break;
	case MOD_UNLOAD:
		callout_drain(&reset_debug_log_handle);
		break;
	}
	return (0);
}

static moduledata_t drm_mod = {
	"drmn",
	drm_modevent,
	0
};

DECLARE_MODULE(drmn, drm_mod, SI_SUB_DRIVERS, SI_ORDER_FIRST);
MODULE_VERSION(drmn, 2);
#ifdef CONFIG_AGP
MODULE_DEPEND(drmn, agp, 1, 1, 1);
#endif
MODULE_DEPEND(drmn, pci, 1, 1, 1);
MODULE_DEPEND(drmn, mem, 1, 1, 1);
MODULE_DEPEND(drmn, linuxkpi, 1, 1, 1);
MODULE_DEPEND(drmn, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(drmn, debugfs, 1, 1, 1);
