
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_print.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_os_freebsd.h>

#include <sys/types.h>
#include <sys/bus.h>
#include <dev/agp/agpreg.h>
#include <dev/pci/pcireg.h>
#include <sys/reboot.h>
#include <linux/cdev.h>
#undef cdev

devclass_t drm_devclass;
const char *fb_mode_option = NULL;

MALLOC_DEFINE(DRM_MEM_DMA, "drm_dma", "DRM DMA Data Structures");
MALLOC_DEFINE(DRM_MEM_DRIVER, "drm_driver", "DRM DRIVER Data Structures");
MALLOC_DEFINE(DRM_MEM_KMS, "drm_kms", "DRM KMS Data Structures");

SYSCTL_NODE(_dev, OID_AUTO, drm, CTLFLAG_RW, 0, "DRM args (compat)");
SYSCTL_INT(_dev_drm, OID_AUTO, __drm_debug, CTLFLAG_RWTUN, &__drm_debug, 0, "drm debug flags (compat)");
SYSCTL_NODE(_hw, OID_AUTO, dri, CTLFLAG_RW, 0, "DRI args");
SYSCTL_INT(_hw_dri, OID_AUTO, __drm_debug, CTLFLAG_RWTUN, &__drm_debug, 0, "drm debug flags");
extern int skip_ddb;
SYSCTL_INT(_dev_drm, OID_AUTO, skip_ddb, CTLFLAG_RWTUN, &skip_ddb, 0, "go straight to dumping core (compat)");
SYSCTL_INT(_hw_dri, OID_AUTO, skip_ddb, CTLFLAG_RWTUN, &skip_ddb, 0, "go straight to dumping core");
#if defined(DRM_DEBUG_LOG_ALL)
int drm_debug_persist = 1;
#else
int drm_debug_persist = 0;
#endif
SYSCTL_INT(_dev_drm, OID_AUTO, drm_debug_persist, CTLFLAG_RWTUN, &drm_debug_persist, 0, "keep drm debug flags post-load (compat)");
SYSCTL_INT(_hw_dri, OID_AUTO, drm_debug_persist, CTLFLAG_RWTUN, &drm_debug_persist, 0, "keep drm debug flags post-load");
int drm_panic_on_error = 0;
SYSCTL_INT(_dev_drm, OID_AUTO, error_panic, CTLFLAG_RWTUN, &drm_panic_on_error, 0, "panic if an ERROR is hit (compat)");
SYSCTL_INT(_hw_dri, OID_AUTO, error_panic, CTLFLAG_RWTUN, &drm_panic_on_error, 0, "panic if an ERROR is hit");
int drm_always_interruptible;
SYSCTL_INT(_dev_drm, OID_AUTO, always_interruptible, CTLFLAG_RWTUN, &drm_always_interruptible, 0, "always allow a thread to be interrupted in driver wait (compat)");
SYSCTL_INT(_hw_dri, OID_AUTO, always_interruptible, CTLFLAG_RWTUN, &drm_always_interruptible, 0, "always allow a thread to be interrupted in driver wait");

static struct callout reset_debug_log_handle;

static void
clear_debug_func(void *arg __unused)
{
	__drm_debug = 0;
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

/* Framebuffer related code */

/* Call restore out of vt(9) locks. */
void
vt_restore_fbdev_mode(void *arg, int pending)
{
	struct drm_fb_helper *fb_helper;
	struct vt_kms_softc *sc;
	struct mm_struct mm;

	sc = (struct vt_kms_softc *)arg;
	fb_helper = sc->fb_helper;
	linux_set_current(curthread);
	if(!fb_helper) {
		DRM_DEBUG("fb helper is null!\n");
		return;
	}
	drm_fb_helper_restore_fbdev_mode_unlocked(fb_helper);
}

int
vt_kms_postswitch(void *arg)
{
	struct vt_kms_softc *sc;

	sc = (struct vt_kms_softc *)arg;

	if (!kdb_active && panicstr == NULL) {
		taskqueue_enqueue(taskqueue_thread, &sc->fb_mode_task);

		/* XXX the VT_ACTIVATE IOCTL must be synchronous */
		if (curthread->td_proc->p_pid != 0 &&
		    taskqueue_member(taskqueue_thread, curthread) == 0)
			taskqueue_drain(taskqueue_thread, &sc->fb_mode_task);
	} else {
#ifdef DDB
		db_trace_self_depth(10);
		mdelay(1000);
#endif
		if (skip_ddb) {
			spinlock_enter();
			doadump(0);
			EVENTHANDLER_INVOKE(shutdown_final, RB_NOSYNC);
		}
		linux_set_current(curthread);
		if(!sc->fb_helper) {
			DRM_DEBUG("fb helper is null!\n");
			return -1;
		}
		drm_fb_helper_restore_fbdev_mode_unlocked(sc->fb_helper);
	}
	return (0);
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

static int
drm_modevent(module_t mod, int type, void *data)
{
	switch (type) {
	case MOD_LOAD:
		TUNABLE_INT_FETCH("drm.debug", &__drm_debug);
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
