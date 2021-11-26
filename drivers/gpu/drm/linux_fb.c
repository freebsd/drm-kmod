#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <drm/drm_fb_helper.h>
#include <drm/drm_print.h>

#undef fb_info

#include <sys/param.h>
#include <sys/systm.h>
#include <vm/vm.h>
#include <vm/vm_page.h>
#include <vm/vm_phys.h>
#include <dev/vt/vt.h>
#include <dev/vt/hw/fb/vt_fb.h>

#define FB_MAJOR		29   /* /dev/fb* framebuffers */

static struct sx linux_fb_mtx;
SX_SYSINIT(linux_fb_mtx, &linux_fb_mtx, "linux fb");

static struct class *fb_class;

static int __unregister_framebuffer(struct linux_fb_info *fb_info);

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

int
remove_conflicting_framebuffers(struct apertures_struct *a,
				const char *name, bool primary)
{

	sx_xlock(&linux_fb_mtx);
	vt_freeze_main_vd(a, name);
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
	sx_xlock(&linux_fb_mtx);
	vt_freeze_main_vd(ap, name);
	sx_xunlock(&linux_fb_mtx);
	kfree(ap);
	return (0);
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

	vt_freeze_main_vd(fb_info->apertures, fb_info->fix.id);

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

	drm_legacy_fb_init(fb_info);

	fb_info->fbio.fb_fbd_dev = device_add_child(fb_info->fb_bsddev, "fbd",
				device_get_unit(fb_info->fb_bsddev));

	/*
	 * XXX we're deliberately not attaching because we already
	 * do the equivalent albeit with less fanfare in fbd_init
	 */
	fbd_init(fb_info, unit_no++);
	/* tell vt_fb to initialize color map */
	fb_info->fbio.fb_cmsize = 0;
	if (fb_info->fbio.fb_bpp == 0) {
		device_printf(fb_info->fbio.fb_fbd_dev,
		    "fb_bpp not set, setting to 8");
		fb_info->fbio.fb_bpp = 32;
	}
	if ((err = vt_fb_attach(&fb_info->fbio)) != 0)
		return (err);
	fb_info_print(&fb_info->fbio);
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

	if (fb_info->dev) {
		device_destroy(fb_class, MKDEV(FB_MAJOR, 0));
		fb_info->dev = NULL;
	}
	return 0;
}

static int
__unregister_framebuffer(struct linux_fb_info *fb_info)
{
	int ret = 0;

	vm_phys_fictitious_unreg_range(fb_info->apertures->ranges[0].base,
				     fb_info->apertures->ranges[0].base +
				     fb_info->apertures->ranges[0].size);
	if (fb_info->fbio.fb_fbd_dev) {
		mtx_lock(&Giant);
		device_delete_child(fb_info->fb_bsddev, fb_info->fbio.fb_fbd_dev);
		mtx_unlock(&Giant);
		fb_info->fbio.fb_fbd_dev = NULL;
	}
	vt_fb_detach(&fb_info->fbio);
	fbd_destroy(fb_info);

	unlink_framebuffer(fb_info);
	if (fb_info->fbops->fb_destroy)
		fb_info->fbops->fb_destroy(fb_info);
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
