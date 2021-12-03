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

static struct sx linux_fb_mtx;
SX_SYSINIT(linux_fb_mtx, &linux_fb_mtx, "linux fb");

static int __unregister_framebuffer(struct linux_fb_info *fb_info);

void
fb_info_print(struct fb_info *t)
{
	printf("start FB_INFO:\n");
	printf("type=%d height=%d width=%d depth=%d\n",
	       t->fb_type, t->fb_height, t->fb_width, t->fb_depth);
	printf("pbase=0x%lx vbase=0x%lx\n",
	       t->fb_pbase, t->fb_vbase);
	printf("name=%s flags=0x%x stride=%d bpp=%d\n",
	       t->fb_name, t->fb_flags, t->fb_stride, t->fb_bpp);
	printf("end FB_INFO\n");
}

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
	vt_freeze_main_vd(a);
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
	vt_freeze_main_vd(ap);
	sx_xunlock(&linux_fb_mtx);
	kfree(ap);
	return (0);
}

static int
__register_framebuffer(struct linux_fb_info *fb_info)
{
	int i, err;

	vt_freeze_main_vd(fb_info->apertures);

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

	fb_info->fbio.fb_type = FBTYPE_PCIMISC;
	fb_info->fbio.fb_height = fb_info->var.yres;
	fb_info->fbio.fb_width = fb_info->var.xres;
	fb_info->fbio.fb_depth = fb_info->var.bits_per_pixel;
	fb_info->fbio.fb_cmsize = 0;
	fb_info->fbio.fb_stride = fb_info->fix.line_length;
	fb_info->fbio.fb_pbase = fb_info->fix.smem_start;
	fb_info->fbio.fb_size = fb_info->fix.smem_len;
	fb_info->fbio.fb_vbase = (uintptr_t)fb_info->screen_base;

	fb_info->fbio.fb_fbd_dev = device_add_child(fb_info->fb_bsddev, "fbd",
				device_get_unit(fb_info->fb_bsddev));

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
