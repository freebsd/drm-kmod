/*-
 * Copyright (c) 2022 Beckhoff Automation GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#undef fb_info

#include <sys/param.h>
#include <sys/types.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/systm.h>
#include <sys/sx.h>
#include <sys/fbio.h>

#include <vm/vm.h>
#include <vm/vm_page.h>
#include <vm/vm_phys.h>

#include <dev/vt/vt.h>
#include <dev/vt/hw/fb/vt_fb.h>

#include <linux/fb.h>
#undef fb_info
#include <drm/drm_os_freebsd.h>

MALLOC_DEFINE(LKPI_FB_MEM, "fb_kms", "FB KMS Data Structures");

static struct sx linux_fb_mtx;
SX_SYSINIT(linux_fb_mtx, &linux_fb_mtx, "linux fb");

extern struct vt_device *main_vd;

static int __unregister_framebuffer(struct linux_fb_info *fb_info);

static void
vt_freeze_main_vd(struct apertures_struct *a)
{
	struct fb_info *fb;
	int i;
	bool overlap = false;

	if (main_vd && main_vd->vd_driver && main_vd->vd_softc &&
	    /* For these, we know the softc (or its first field) is of type fb_info */
	    (strcmp(main_vd->vd_driver->vd_name, "efifb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "vbefb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "ofwfb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "fb") == 0)) {
		fb = main_vd->vd_softc;

		for (i = 0; i < a->count; i++) {
			if (fb->fb_pbase == a->ranges[i].base) {
				overlap = true;
				break;
			}
			if ((fb->fb_pbase > a->ranges[i].base) &&
			    (fb->fb_pbase < (a->ranges[i].base + a->ranges[i].size))) {
				overlap = true;
				break;
			}
		}
		if (overlap == true)
			fb->fb_flags |= FB_FLAG_NOWRITE;
	}
}

void
vt_unfreeze_main_vd(void)
{
	struct fb_info *fb;

	fb = main_vd->vd_softc;
	fb->fb_flags &= ~FB_FLAG_NOWRITE;
}

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

	info = malloc(sizeof(*info) + size, LKPI_FB_MEM, M_WAITOK | M_ZERO);
	sc = malloc(sizeof(*sc), LKPI_FB_MEM, M_WAITOK | M_ZERO);
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
	if (info->fbio.fb_priv)
		sc = info->fbio.fb_priv;
	kfree(info->apertures);
	free(info->fbio.fb_priv, LKPI_FB_MEM);
	free(info, LKPI_FB_MEM);
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

int
linux_fb_get_options(const char *connector_name, char **option)
{
	char tunable[64];

	/*
	 * A user may use loader tunables to set a specific mode for the
	 * console. Tunables are read in the following order:
	 *     1. kern.vt.fb.modes.$connector_name
	 *     2. kern.vt.fb.default_mode
	 *
	 * Example of a mode specific to the LVDS connector:
	 *     kern.vt.fb.modes.LVDS="1024x768"
	 *
	 * Example of a mode applied to all connectors not having a
	 * connector-specific mode:
	 *     kern.vt.fb.default_mode="640x480"
	 */
	snprintf(tunable, sizeof(tunable), "kern.vt.fb.modes.%s",
	    connector_name);
	if (bootverbose) {
		printf("[drm] Connector %s: get mode from tunables:\n", connector_name);
		printf("[drm]  - %s\n", tunable);
		printf("[drm]  - kern.vt.fb.default_mode\n");
	}
	*option = kern_getenv(tunable);
	if (*option == NULL)
		*option = kern_getenv("kern.vt.fb.default_mode");

	return (*option != NULL ? 0 : -ENOENT);
}
