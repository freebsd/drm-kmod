#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>

#include <sys/kdb.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <dev/vt/vt.h>

extern struct vt_device *main_vd;

static void
vt_dummy_bitblt_bitmap(struct vt_device *vd, const struct vt_window *vw,
    const uint8_t *pattern, const uint8_t *mask,
    unsigned int width, unsigned int height,
    unsigned int x, unsigned int y, term_color_t fg, term_color_t bg)
{
}

static void
vt_dummy_bitblt_text(struct vt_device *vd, const struct vt_window *vw,
    const term_rect_t *area)
{
}

static int
vt_dummy_init(struct vt_device *vd)
{
	return (CN_INTERNAL);
}

static struct vt_driver vt_dummy_driver = {
	.vd_name = "dummy",
	.vd_init = vt_dummy_init,
	.vd_bitblt_text = vt_dummy_bitblt_text,
	.vd_bitblt_bmp = vt_dummy_bitblt_bitmap,
	.vd_priority = VD_PRIORITY_GENERIC + 9, /* > efifb, < fb */
	.vd_suspend = vt_suspend,
	.vd_resume = vt_resume,
};

#define FB_MAJOR		29   /* /dev/fb* framebuffers */
#define FBPIXMAPSIZE	(1024 * 8)

#undef fb_info
static struct sx linux_fb_mtx;
SX_SYSINIT(linux_fb_mtx, &linux_fb_mtx, "linux fb");

static struct linux_fb_info *registered_fb[FB_MAX];
static int num_registered_fb;
static struct class *fb_class;

static int __unregister_framebuffer(struct linux_fb_info *fb_info);

extern int vt_fb_attach(struct fb_info *info);
extern void vt_fb_detach(struct fb_info *info);

#include <sys/reboot.h>

int skip_ddb;

void
fb_info_print(struct fb_info *t)
{
	printf("start FB_INFO:\n");
	printf("type=%d height=%d width=%d depth=%d\n",
	       t->fb_type, t->fb_height, t->fb_width, t->fb_depth);
	printf("cmsize=%d size=%d\n",
	       t->fb_cmsize, t->fb_size);
	printf("pbase=0x%lx vbase=0x%lx\n",
	       t->fb_pbase, t->fb_vbase);
	printf("name=%s flags=0x%x stride=%d bpp=%d\n",
	       t->fb_name, t->fb_flags, t->fb_stride, t->fb_bpp);
	printf("cmap[0]=%x cmap[1]=%x cmap[2]=%x cmap[3]=%x\n",
	       t->fb_cmap[0], t->fb_cmap[1], t->fb_cmap[2], t->fb_cmap[3]);
	printf("end FB_INFO\n");
}

/* Call restore out of vt(9) locks. */
static void
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

static int
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

static d_open_t		fb_open;
static d_close_t	fb_close;
static d_read_t		fb_read;
static d_write_t	fb_write;
static d_ioctl_t	fb_ioctl;
static d_mmap_t		fb_mmap;

static struct cdevsw fb_cdevsw = {
	.d_version =	D_VERSION,
	.d_flags =	D_NEEDGIANT,
	.d_open =	fb_open,
	.d_close =	fb_close,
	.d_read =	fb_read,
	.d_write =	fb_write,
	.d_ioctl =	fb_ioctl,
	.d_mmap =	fb_mmap,
	.d_name =	"fb",
};

static int framebuffer_dev_unit = 0;

static int
fb_open(struct cdev *dev, int oflags, int devtype, struct thread *td)
{

	return (0);
}

static int
fb_close(struct cdev *dev, int fflag, int devtype, struct thread *td)
{

	return (0);
}

static int
fb_ioctl(struct cdev *dev, u_long cmd, caddr_t data, int fflag,
    struct thread *td)
{
	struct linux_fb_info *info;
	struct fbtype *t;
	int error;

	error = 0;
	info = dev->si_drv1;

	switch (cmd) {
	case FBIOGTYPE:
		t = (struct fbtype *)data;
		t->fb_type = FBTYPE_PCIMISC;
		t->fb_height = info->var.yres;
		t->fb_width = info->var.xres;
		t->fb_depth = info->var.bits_per_pixel;
		t->fb_cmsize = info->cmap.len;
		t->fb_size = info->screen_size;
		break;

	case FBIO_GETWINORG:	/* get frame buffer window origin */
		*(u_int *)data = 0;
		break;

	case FBIO_GETDISPSTART:	/* get display start address */
		((video_display_start_t *)data)->x = 0;
		((video_display_start_t *)data)->y = 0;
		break;

	case FBIO_GETLINEWIDTH:	/* get scan line width in bytes */
		*(u_int *)data = info->fix.line_length;;
		break;

	case FBIO_BLANK:	/* blank display */
		if (info->fbops->fb_blank != NULL)
			error = info->fbops->fb_blank((long)data, info);
		break;

	default:
		error = ENOIOCTL;
		break;
	}
	return (error);
}

static int
fb_read(struct cdev *dev, struct uio *uio, int ioflag)
{

	return (0); /* XXX nothing to read, yet */
}

static int
fb_write(struct cdev *dev, struct uio *uio, int ioflag)
{

	return (0); /* XXX nothing written */
}

static int
fb_mmap(struct cdev *dev, vm_ooffset_t offset, vm_paddr_t *paddr, int nprot,
    vm_memattr_t *memattr)
{
	struct linux_fb_info *info;

	info = dev->si_drv1;
#ifdef notyet
	if (info->fb_flags & FB_FLAG_NOMMAP)
		return (ENODEV);
#endif
	if (offset >= 0 && offset < info->screen_size) {
		if (info->fix.smem_start == 0)
			*paddr = vtophys((uint8_t *)info->screen_base + offset);
		else
			*paddr = info->fix.smem_start + offset;
		return (0);
	}
	return (EINVAL);
}



static int
fbd_init(struct linux_fb_info *fb_info, int unit)
{
	fb_info->fb_cdev = make_dev(&fb_cdevsw, unit, UID_ROOT, GID_WHEEL, 0600, "fb%d", unit);
	fb_info->fb_cdev->si_drv1 = fb_info;

	return (0);
}

static int
fbd_destroy(struct linux_fb_info *fb_info)
{
	destroy_dev(fb_info->fb_cdev);

	return (0);
}


static int
fb_init(void)
{
	fb_class = class_create(THIS_MODULE, "graphics");

	return (fb_class == NULL ? ENOMEM : 0);
}
SYSINIT(fb_init, SI_SUB_KLD, SI_ORDER_MIDDLE, fb_init, NULL);

static void
fb_exit(void)
{
	if (fb_class) {
		class_destroy(fb_class);
		fb_class = NULL;
	}
}
SYSUNINIT(fb_exit, SI_SUB_KLD, SI_ORDER_MIDDLE, fb_exit, NULL);

CTASSERT((sizeof(struct linux_fb_info) % sizeof(long)) == 0);

struct linux_fb_info *
framebuffer_alloc(size_t size, struct device *dev)
{
	struct linux_fb_info *info;
	struct vt_kms_softc *sc;

	info = malloc(sizeof(*info) + size, DRM_MEM_KMS, M_WAITOK | M_ZERO);
	sc = malloc(sizeof(*sc), DRM_MEM_KMS, M_WAITOK | M_ZERO);
	TASK_INIT(&sc->fb_mode_task, 0, vt_restore_fbdev_mode, sc);

	info->fbio.fb_priv = sc;
	info->fbio.enter = &vt_kms_postswitch;

	if (size)
		info->par = info + 1;

	info->device = dev;

	return info;
}

void
framebuffer_release(struct linux_fb_info *info)
{
	struct vt_kms_softc *sc;

	if (info == NULL)
		return;
	if (info->fbio.fb_priv) {
		sc = info->fbio.fb_priv;
		if (sc->fb_helper != NULL)
			sc->fb_helper->fbdev = NULL;
	}
	kfree(info->apertures);
	free(info->fbio.fb_priv, DRM_MEM_KMS);
	free(info, DRM_MEM_KMS);
}

static void
put_fb_info(struct linux_fb_info *fb_info)
{
	if (!atomic_dec_and_test(&fb_info->count))
		return;

	if (fb_info->fbops->fb_destroy)
		fb_info->fbops->fb_destroy(fb_info);
}

static bool
apertures_overlap(struct aperture *gen, struct aperture *hw)
{
	/* is the generic aperture base the same as the HW one */
	if (gen->base == hw->base)
		return (true);
	/* is the generic aperture base inside the hw base->hw base+size */
	if (gen->base > hw->base && gen->base < hw->base + hw->size)
		return (true);
	return (false);
}

static bool
check_overlap(struct apertures_struct *a, struct apertures_struct *b)
{
	int i, j;

	if (a == NULL || b == NULL)
		return (false);

	for (i = 0; i < b->count; ++i)
		for (j = 0; j < a->count; ++j) 
			if (apertures_overlap(&a->ranges[j], &b->ranges[i]))
				return (true);
	return (false);
}



#define VGA_FB_PHYS 0xA0000
static void
__remove_conflicting(struct apertures_struct *a, const char *name, bool primary)
{
	struct apertures_struct *gen_aper;
	int i;

	for (i = 0 ; i < FB_MAX; i++) {

		if (registered_fb[i] == NULL)
			continue;

		if ((registered_fb[i]->flags & FBINFO_MISC_FIRMWARE) == 0)
			continue;

		gen_aper = registered_fb[i]->apertures;
		if (check_overlap(gen_aper, a) ||
			(primary && gen_aper && gen_aper->count &&
			 gen_aper->ranges[0].base == VGA_FB_PHYS)) {
			__unregister_framebuffer(registered_fb[i]);
		}
	}

#ifdef __FreeBSD__
	// Native drivers are not registered in LinuxKPI so they
	// would never be removed by the code above!
	if (main_vd && main_vd->vd_driver && main_vd->vd_softc &&
	    /* For these, we know the softc (or its first field) is of type fb_info */
	    (strcmp(main_vd->vd_driver->vd_name, "efifb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "vbefb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "ofwfb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "fb") == 0)) {
		struct fb_info *fb = main_vd->vd_softc;
		struct apertures_struct *fb_ap = alloc_apertures(1);
		if (!fb_ap) {
			DRM_ERROR("Could not allocate an apertures_struct");
			return;
		}
		fb_ap->ranges[0].base = fb->fb_pbase;
		fb_ap->ranges[0].size = fb->fb_size;
		if (check_overlap(fb_ap, a))
			vt_allocate(&vt_dummy_driver, &vt_dummy_driver /* any non-NULL softc */);
		kfree(fb_ap);
	}
#endif
}

int
remove_conflicting_framebuffers(struct apertures_struct *a,
				const char *name, bool primary)
{
	sx_xlock(&linux_fb_mtx);
	__remove_conflicting(a, name, primary);
	sx_xunlock(&linux_fb_mtx);
	return (0);
}


#define	PCI_STD_NUM_BARS	6
int
remove_conflicting_pci_framebuffers(struct pci_dev *pdev, const char *name)
{
	struct apertures_struct *ap;
	bool primary = false;
	int err, idx, bar;

	for (idx = 0, bar = 0; bar < PCI_STD_NUM_BARS; bar++) {
		if (!(pci_resource_flags(pdev, bar) & IORESOURCE_MEM))
			continue;
		idx++;
	}

	ap = alloc_apertures(idx);
	if (!ap)
		return -ENOMEM;

	for (idx = 0, bar = 0; bar < PCI_STD_NUM_BARS; bar++) {
		if (!(pci_resource_flags(pdev, bar) & IORESOURCE_MEM))
			continue;
		ap->ranges[idx].base = pci_resource_start(pdev, bar);
		ap->ranges[idx].size = pci_resource_len(pdev, bar);
		idx++;
	}

#ifdef CONFIG_X86
#ifndef __linux__
	/* BSDFIXME: Check primary! */
	primary = false;
#else
	primary = pdev->resource[PCI_ROM_RESOURCE].flags &
					IORESOURCE_ROM_SHADOW;
#endif
#endif
	sx_xlock(&linux_fb_mtx);
	__remove_conflicting(ap, name, primary);
	sx_xunlock(&linux_fb_mtx);
	kfree(ap);
	return (0);
}

static int
is_primary(struct linux_fb_info *info)
{
	struct device *device = info->device;
	struct pci_dev *pci_dev = NULL;
#ifdef __linux__
	struct linux_resource *res = NULL;
#endif

	if (device &&  (pci_dev = to_pci_dev(device)) == NULL)
		return (0);

#ifdef __linux__
	res = &pci_dev->resource[PCI_ROM_RESOURCE];
	if (res && res->flags & IORESOURCE_ROM_SHADOW)
		return (1);
#endif

	return (0);
}


static int
fb_mode_is_equal(const struct fb_videomode *mode1,
		     const struct fb_videomode *mode2)
{
	return (mode1->xres         == mode2->xres &&
		mode1->yres         == mode2->yres &&
		mode1->pixclock     == mode2->pixclock &&
		mode1->hsync_len    == mode2->hsync_len &&
		mode1->vsync_len    == mode2->vsync_len &&
		mode1->left_margin  == mode2->left_margin &&
		mode1->right_margin == mode2->right_margin &&
		mode1->upper_margin == mode2->upper_margin &&
		mode1->lower_margin == mode2->lower_margin &&
		mode1->sync         == mode2->sync &&
		mode1->vmode        == mode2->vmode);
}
static void
fb_destroy_modes(struct list_head *head)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, head) {
		list_del(pos);
		kfree(pos);
	}
}


static int
fb_add_videomode(const struct fb_videomode *mode, struct list_head *head)
{
	struct list_head *pos;
	struct fb_modelist *modelist;
	struct fb_videomode *m;
	int found = 0;

	list_for_each(pos, head) {
		modelist = list_entry(pos, struct fb_modelist, list);
		m = &modelist->mode;
		if (fb_mode_is_equal(m, mode)) {
			found = 1;
			break;
		}
	}
	if (!found) {
		modelist = kmalloc(sizeof(struct fb_modelist),
						  GFP_KERNEL);

		if (!modelist)
			return -ENOMEM;
		modelist->mode = *mode;
		list_add(&modelist->list, head);
	}
	return 0;
}

static void
fb_var_to_videomode(struct fb_videomode *mode,
			 const struct fb_var_screeninfo *var)
{
	u32 pixclock, hfreq, htotal, vtotal;

	mode->name = NULL;
	mode->xres = var->xres;
	mode->yres = var->yres;
	mode->pixclock = var->pixclock;
	mode->hsync_len = var->hsync_len;
	mode->vsync_len = var->vsync_len;
	mode->left_margin = var->left_margin;
	mode->right_margin = var->right_margin;
	mode->upper_margin = var->upper_margin;
	mode->lower_margin = var->lower_margin;
	mode->sync = var->sync;
	mode->vmode = var->vmode & FB_VMODE_MASK;
	mode->flag = FB_MODE_IS_FROM_VAR;
	mode->refresh = 0;

	if (!var->pixclock)
		return;

	pixclock = PICOS2KHZ(var->pixclock) * 1000;

	htotal = var->xres + var->right_margin + var->hsync_len +
		var->left_margin;
	vtotal = var->yres + var->lower_margin + var->vsync_len +
		var->upper_margin;

	if (var->vmode & FB_VMODE_INTERLACED)
		vtotal /= 2;
	if (var->vmode & FB_VMODE_DOUBLE)
		vtotal *= 2;

	hfreq = pixclock/htotal;
	mode->refresh = hfreq/vtotal;
}

void
drm_legacy_fb_init(struct linux_fb_info *info)
{
	struct fb_info *t;

	t = &info->fbio;
	t->fb_type = FBTYPE_PCIMISC;
	t->fb_height = info->var.yres;
	t->fb_width = info->var.xres;
	t->fb_depth = info->var.bits_per_pixel;
	t->fb_cmsize = info->cmap.len;
	t->fb_stride = info->fix.line_length;
	t->fb_pbase = info->fix.smem_start;
	t->fb_size = info->fix.smem_len;
	t->fb_vbase = (uintptr_t)info->screen_base;
}

static int
__register_framebuffer(struct linux_fb_info *fb_info)
{
	int i, err;
	static int unit_no;
	struct fb_event event;
	struct fb_videomode mode;

	__remove_conflicting(fb_info->apertures, fb_info->fix.id, is_primary(fb_info));

	if (num_registered_fb == FB_MAX)
		return -ENXIO;

	num_registered_fb++;
	for (i = 0 ; i < FB_MAX; i++)
		if (!registered_fb[i])
			break;
	fb_info->node = i;
	atomic_set(&fb_info->count, 1);
	mutex_init(&fb_info->lock);
	mutex_init(&fb_info->mm_lock);

	MPASS(fb_info->apertures->ranges[0].base);
	MPASS(fb_info->apertures->ranges[0].size);
	vm_phys_fictitious_reg_range(fb_info->apertures->ranges[0].base,
				     fb_info->apertures->ranges[0].base +
				     fb_info->apertures->ranges[0].size,
#ifdef VM_MEMATTR_WRITE_COMBINING
				     VM_MEMATTR_WRITE_COMBINING);
#else
				     VM_MEMATTR_UNCACHEABLE);
#endif
	fb_info->dev = device_create(fb_class, fb_info->device,
				     MKDEV(FB_MAJOR, i), NULL, "fb%d", i);
	if (IS_ERR(fb_info->dev)) {
		/* Not fatal */
		printk(KERN_WARNING "Unable to create device for framebuffer %d; errno = %ld\n", i, PTR_ERR(fb_info->dev));
		fb_info->dev = NULL;
	} else
		dev_set_drvdata(fb_info->dev, fb_info);

	if (fb_info->pixmap.addr == NULL) {
		fb_info->pixmap.addr = kmalloc(FBPIXMAPSIZE, GFP_KERNEL);
		if (fb_info->pixmap.addr) {
			fb_info->pixmap.size = FBPIXMAPSIZE;
			fb_info->pixmap.buf_align = 1;
			fb_info->pixmap.scan_align = 1;
			fb_info->pixmap.access_align = 32;
			fb_info->pixmap.flags = FB_PIXMAP_DEFAULT;
		}
	}
	fb_info->pixmap.offset = 0;

	if (!fb_info->pixmap.blit_x)
		fb_info->pixmap.blit_x = ~(u32)0;

	if (!fb_info->pixmap.blit_y)
		fb_info->pixmap.blit_y = ~(u32)0;

	if (!fb_info->modelist.prev || !fb_info->modelist.next)
		INIT_LIST_HEAD(&fb_info->modelist);

	fb_var_to_videomode(&mode, &fb_info->var);
	fb_add_videomode(&mode, &fb_info->modelist);
	registered_fb[i] = fb_info;

	event.info = fb_info;
	drm_legacy_fb_init(fb_info);

	fb_info->fbio.fb_fbd_dev = device_add_child(fb_info->fb_bsddev, "fbd",
				device_get_unit(fb_info->fb_bsddev));

	/*
	 * XXX we're deliberately not attaching because we already
	 * do the equivalent albeit with less fanfare in fbd_init
	 */
	fbd_init(fb_info, unit_no++);
	if (num_registered_fb == 1) {
		/* tell vt_fb to initialize color map */
		fb_info->fbio.fb_cmsize = 0;
		if (fb_info->fbio.fb_bpp == 0) {
			device_printf(fb_info->fbio.fb_fbd_dev,
				      "fb_bpp not set, setting to 8");
			fb_info->fbio.fb_bpp = 32;
		}
		if ((err = vt_fb_attach(&fb_info->fbio)) != 0)
			return (err);
	}
	fb_info_print(&fb_info->fbio);

#if 0
	if (!lock_fb_info(fb_info))
		return -ENODEV;
	console_lock();
	fb_notifier_call_chain(FB_EVENT_FB_REGISTERED, &event);
	console_unlock();
	unlock_fb_info(fb_info);
#endif
	return 0;
}

int
linux_register_framebuffer(struct linux_fb_info *fb_info)
{
	int rc;

	sx_xlock(&linux_fb_mtx);
	rc = __register_framebuffer(fb_info);
	sx_xunlock(&linux_fb_mtx);
	return (rc);
}

int
unlink_framebuffer(struct linux_fb_info *fb_info)
{
	int i;

	i = fb_info->node;
	if (i < 0 || i >= FB_MAX || registered_fb[i] != fb_info)
		return -EINVAL;

	if (fb_info->dev) {
		device_destroy(fb_class, MKDEV(FB_MAJOR, i));
		fb_info->dev = NULL;
	}
	return 0;
}

static int
__unregister_framebuffer(struct linux_fb_info *fb_info)
{
	struct fb_event event;
	int i, ret = 0;

	i = fb_info->node;
	if (i < 0 || i >= FB_MAX || registered_fb[i] != fb_info)
		return -EINVAL;
	vm_phys_fictitious_unreg_range(fb_info->apertures->ranges[0].base,
				     fb_info->apertures->ranges[0].base +
				     fb_info->apertures->ranges[0].size);
	if (fb_info->fbio.fb_fbd_dev) {
		mtx_lock(&Giant);
		device_delete_child(fb_info->fb_bsddev, fb_info->fbio.fb_fbd_dev);
		mtx_unlock(&Giant);
		fb_info->fbio.fb_fbd_dev = NULL;
	}
	if (num_registered_fb == 1)
		vt_fb_detach(&fb_info->fbio);
	fbd_destroy(fb_info);


#if 0
	if (!lock_fb_info(fb_info))
		return -ENODEV;
	console_lock();
	event.info = fb_info;
	ret = fb_notifier_call_chain(FB_EVENT_FB_UNBIND, &event);
	console_unlock();
	unlock_fb_info(fb_info);

	if (ret)
		return -EINVAL;
#endif

	unlink_framebuffer(fb_info);
	if (fb_info->pixmap.addr &&
	    (fb_info->pixmap.flags & FB_PIXMAP_DEFAULT))
		kfree(fb_info->pixmap.addr);
	fb_destroy_modes(&fb_info->modelist);
	registered_fb[i] = NULL;
	num_registered_fb--;
	event.info = fb_info;

#if 0
	console_lock();
	fb_notifier_call_chain(FB_EVENT_FB_UNREGISTERED, &event);
	console_unlock();
#endif
	put_fb_info(fb_info);
	return 0;
}

int
linux_unregister_framebuffer(struct linux_fb_info *fb_info)
{
	int rc;

	sx_xlock(&linux_fb_mtx);
	rc = __unregister_framebuffer(fb_info);
	sx_xunlock(&linux_fb_mtx);
	return (rc);
}

void
fb_set_suspend(struct linux_fb_info *info, int state)
{
#if 0
	struct fb_event event;

	event.info = info;
	if (state) {
		fb_notifier_call_chain(FB_EVENT_SUSPEND, &event);
		info->state = FBINFO_STATE_SUSPENDED;
	} else {
		info->state = FBINFO_STATE_RUNNING;
		fb_notifier_call_chain(FB_EVENT_RESUME, &event);
	}
#endif
}


void
cfb_fillrect(struct linux_fb_info *p, const struct fb_fillrect *rect)
{
	tainted_cfb_fillrect(p, rect);
}

void
cfb_copyarea(struct linux_fb_info *p, const struct fb_copyarea *area)
{

	tainted_cfb_copyarea(p, area);
}

void
cfb_imageblit(struct linux_fb_info *p, const struct fb_image *image)
{
	tainted_cfb_imageblit(p, image);
}

void
sys_fillrect(struct linux_fb_info *p, const struct fb_fillrect *rect)
{
	tainted_cfb_fillrect(p, rect);
}

void
sys_copyarea(struct linux_fb_info *p, const struct fb_copyarea *area)
{

	tainted_cfb_copyarea(p, area);
}

void
sys_imageblit(struct linux_fb_info *p, const struct fb_image *image)
{
	tainted_cfb_imageblit(p, image);
}

static int
fb_copy_cmap(const struct fb_cmap *from, struct fb_cmap *to)
{
	int tooff = 0, fromoff = 0;
	int size;

	if (to->start > from->start)
		fromoff = to->start - from->start;
	else
		tooff = from->start - to->start;
	size = to->len - tooff;
	if (size > (int) (from->len - fromoff))
		size = from->len - fromoff;
	if (size <= 0)
		return -EINVAL;
	size *= sizeof(u16);

	memcpy(to->red+tooff, from->red+fromoff, size);
	memcpy(to->green+tooff, from->green+fromoff, size);
	memcpy(to->blue+tooff, from->blue+fromoff, size);
	if (from->transp && to->transp)
		memcpy(to->transp+tooff, from->transp+fromoff, size);
	return 0;
}

static int
fb_alloc_cmap_gfp(struct fb_cmap *cmap, int len, int transp, gfp_t flags)
{
	int size = len * sizeof(u16);
	int ret = -ENOMEM;

	if (cmap->len != len) {
		fb_dealloc_cmap(cmap);
		if (!len)
			return 0;

		cmap->red = kmalloc(size, flags);
		if (!cmap->red)
			goto fail;
		cmap->green = kmalloc(size, flags);
		if (!cmap->green)
			goto fail;
		cmap->blue = kmalloc(size, flags);
		if (!cmap->blue)
			goto fail;
		if (transp) {
			cmap->transp = kmalloc(size, flags);
			if (!cmap->transp)
				goto fail;
		} else {
			cmap->transp = NULL;
		}
	}
	cmap->start = 0;
	cmap->len = len;
	ret = fb_copy_cmap(tainted_fb_default_cmap(len), cmap);
	if (ret)
		goto fail;
	return 0;

fail:
	fb_dealloc_cmap(cmap);
	return ret;
}

int
fb_alloc_cmap(struct fb_cmap *cmap, int len, int transp)
{
	return fb_alloc_cmap_gfp(cmap, len, transp, GFP_ATOMIC);
}

void
fb_dealloc_cmap(struct fb_cmap *cmap)
{
	kfree(cmap->red);
	kfree(cmap->green);
	kfree(cmap->blue);
	kfree(cmap->transp);

	cmap->red = cmap->green = cmap->blue = cmap->transp = NULL;
	cmap->len = 0;
}


ssize_t
fb_sys_read(struct linux_fb_info *info, char *ubuf, size_t count,
		    loff_t *ppos)
{
	unsigned long p = *ppos;
	void *src;
	int err = 0;
	unsigned long total_size;

	if (info->state != FBINFO_STATE_RUNNING)
		return -EPERM;

	total_size = info->screen_size ? info->screen_size : info->fix.smem_len;

	if (p >= total_size)
		return 0;

	if (count + p > total_size)
		count = total_size - p;

	src = (void *)(info->screen_base + p);

	if (info->fbops->fb_sync)
		info->fbops->fb_sync(info);

	if (copyout(src, ubuf, count))
		err = -EFAULT;

	if  (!err)
		*ppos += count;

	return (err) ? err : count;
}

ssize_t
fb_sys_write(struct linux_fb_info *info, const char *kbuf,
		     size_t count, loff_t *ppos)
{
	unsigned long p = *ppos;
	void *dst;
	int err = 0;
	unsigned long total_size;

	if (info->state != FBINFO_STATE_RUNNING)
		return -EPERM;

	total_size = info->screen_size ? info->screen_size : info->fix.smem_len;

	if (p > total_size)
		return -EFBIG;

	if (count + p > total_size)
		count = total_size - p;

	dst = (void *) (info->screen_base + p);

	if (info->fbops->fb_sync)
		info->fbops->fb_sync(info);

	if (copyin(kbuf, dst, count))
		err = -EFAULT;

	if  (!err)
		*ppos += count;

	return (err) ? err : count;
}
