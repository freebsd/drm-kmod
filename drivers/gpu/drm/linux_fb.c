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

#include <dev/vt/vt.h>
#include "vt_drmfb.h"

#include <drm/drm_fb_helper.h>
#include <linux/fb.h>
#include <video/cmdline.h>
#undef fb_info
#include <drm/drm_os_freebsd.h>

MALLOC_DEFINE(LKPI_FB_MEM, "fb_kms", "FB KMS Data Structures");

static struct sx linux_fb_mtx;
SX_SYSINIT(linux_fb_mtx, &linux_fb_mtx, "linux fb");

extern struct vt_device *main_vd;

static int __unregister_framebuffer(struct linux_fb_info *fb_info);

void
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
	    || strcmp(main_vd->vd_driver->vd_name, "fb") == 0
	    || strcmp(main_vd->vd_driver->vd_name, "drmfb") == 0)) {
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

/* Call restore out of vt(9) locks. */
static void
vt_restore_fbdev_mode(void *arg, int pending)
{
	struct linux_fb_info *info;

	info = (struct linux_fb_info *)arg;
	linux_set_current(curthread);
	info->fbops->fb_set_par(info);
}

void
fb_info_print(struct linux_fb_info *info)
{
	printf("start FB_INFO:\n");
	printf("height=%d width=%d depth=%d\n",
	       info->var.yres, info->var.xres, info->var.bits_per_pixel);
	printf("pbase=0x%lx vbase=0x%lx\n",
	       info->fix.smem_start, info->screen_base);
	printf("name=%s id=%s flags=0x%x stride=%d\n",
	       info->fbio.fb_name, info->fix.id, info->fbio.fb_flags,
	       info->fix.line_length);
	printf("end FB_INFO\n");
}

CTASSERT((sizeof(struct linux_fb_info) % sizeof(long)) == 0);

struct linux_fb_info *
framebuffer_alloc(size_t size, struct device *dev)
{
	struct linux_fb_info *info;

	info = malloc(sizeof(*info) + size, LKPI_FB_MEM, M_WAITOK | M_ZERO);
	TASK_INIT(&info->fb_mode_task, 0, vt_restore_fbdev_mode, info);

	if (size)
		info->par = info + 1;

	info->device = dev;

	return info;
}

void
framebuffer_release(struct linux_fb_info *info)
{
	if (info == NULL)
		return;
	kfree(info->apertures);
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
	struct drm_fb_helper *fb_helper;

	fb_helper = (struct drm_fb_helper *)fb_info->fbio.fb_priv;
	fb_info->fb_bsddev = fb_helper->dev->dev->bsddev;
	fb_info->fbio.fb_video_dev = device_get_parent(fb_info->fb_bsddev);
	fb_info->fbio.fb_name = device_get_nameunit(fb_info->fb_bsddev);

	fb_info->fbio.fb_type = FBTYPE_PCIMISC;
	fb_info->fbio.fb_height = fb_info->var.yres;
	fb_info->fbio.fb_width = fb_info->var.xres;
	fb_info->fbio.fb_bpp = fb_info->var.bits_per_pixel;
	fb_info->fbio.fb_depth = fb_info->var.bits_per_pixel;
	fb_info->fbio.fb_cmsize = 0;
	fb_info->fbio.fb_stride = fb_info->fix.line_length;
	fb_info->fbio.fb_pbase = fb_info->fix.smem_start;
	fb_info->fbio.fb_size = fb_info->fix.smem_len;
	fb_info->fbio.fb_vbase = (uintptr_t)fb_info->screen_base;

	fb_info->fbio.fb_fbd_dev = device_add_child(fb_info->fb_bsddev, "fbd",
				device_get_unit(fb_info->fb_bsddev));

	/* tell vt_drmfb to initialize color map */
	fb_info->fbio.fb_cmsize = 0;
	if (fb_info->fbio.fb_bpp == 0) {
		device_printf(fb_info->fbio.fb_fbd_dev,
		    "fb_bpp not set, setting to 8\n");
		fb_info->fbio.fb_bpp = 32;
	}
	if ((err = vt_drmfb_attach(&fb_info->fbio)) != 0) {
		switch (err) {
		case EEXIST:
			device_printf(fb_info->fbio.fb_fbd_dev,
			    "not attached to vt(4) console; "
			    "another device has precedence (err=%d)\n",
			    err);
			err = 0;
			break;
		default:
			device_printf(fb_info->fbio.fb_fbd_dev,
			    "failed to attach to vt(4) console (err=%d)\n",
			    err);
		}
		return (-err);
	}
	fb_info_print(fb_info);
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

	vt_drmfb_detach(&fb_info->fbio);

	if (fb_info->fbio.fb_fbd_dev) {
		mtx_lock(&Giant);
		device_delete_child(fb_info->fb_bsddev, fb_info->fbio.fb_fbd_dev);
		mtx_unlock(&Giant);
		fb_info->fbio.fb_fbd_dev = NULL;
	}

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
	*option = __DECONST(char *, video_get_options(connector_name));
	return (*option != NULL ? 0 : -ENOENT);
}

/*
 * Routines to write to the framebuffer. They are used to implement Linux'
 * fbdev equivalent functions below.
 *
 * Copied from `sys/dev/vt/hw/fb/vt_fb.c`.
 */

static void
fb_mem_wr1(struct linux_fb_info *info, uint32_t offset, uint8_t value)
{
	KASSERT(
	    (offset < info->screen_size),
	    ("Offset %#08x out of framebuffer size", offset));
	*(uint8_t *)(info->screen_base + offset) = value;
}

static void
fb_mem_wr2(struct linux_fb_info *info, uint32_t offset, uint16_t value)
{
	KASSERT(
	    (offset < info->screen_size),
	    ("Offset %#08x out of framebuffer size", offset));
	*(uint16_t *)(info->screen_base + offset) = value;
}

static void
fb_mem_wr4(struct linux_fb_info *info, uint32_t offset, uint32_t value)
{
	KASSERT(
	    (offset < info->screen_size),
	    ("Offset %#08x out of framebuffer size", offset));
	*(uint32_t *)(info->screen_base + offset) = value;
}

static void
fb_setpixel(struct linux_fb_info *info, uint32_t x, uint32_t y,
    uint32_t color)
{
	uint32_t bytes_per_pixel;
	unsigned int offset;

	bytes_per_pixel = info->var.bits_per_pixel / 8;
	offset = info->fix.line_length * y + x * bytes_per_pixel;

	KASSERT((info->screen_base != 0), ("Unmapped framebuffer"));

	switch (bytes_per_pixel) {
	case 1:
		fb_mem_wr1(info, offset, color);
		break;
	case 2:
		fb_mem_wr2(info, offset, color);
		break;
	case 3:
		fb_mem_wr1(info, offset, (color >> 16) & 0xff);
		fb_mem_wr1(info, offset + 1, (color >> 8) & 0xff);
		fb_mem_wr1(info, offset + 2, color & 0xff);
		break;
	case 4:
		fb_mem_wr4(info, offset, color);
		break;
	default:
		/* panic? */
		return;
	}
}

void
cfb_fillrect(struct linux_fb_info *info, const struct fb_fillrect *rect)
{
	uint32_t x, y;

	if (info->fbio.fb_flags & FB_FLAG_NOWRITE)
		return;

	KASSERT(
	    (rect->rop == ROP_COPY),
	    ("`rect->rop=%u` is unsupported in cfb_fillrect()", rect->rop));

	for (y = rect->dy; y < rect->dy + rect->height; ++y) {
		for (x = rect->dx; x < rect->dx + rect->width; ++x) {
			fb_setpixel(info, x, y, rect->color);
		}
	}
}

void
cfb_copyarea(struct linux_fb_info *info, const struct fb_copyarea *area)
{
	panic("cfb_copyarea() not implemented");
}

void
cfb_imageblit(struct linux_fb_info *info, const struct fb_image *image)
{
	uint32_t x, y, width, height, xi, yi;
	uint32_t bytes_per_img_line, bit, byte, color;

	if (info->fbio.fb_flags & FB_FLAG_NOWRITE)
		return;

	KASSERT(
	    (image->depth == 1),
	    ("`image->depth=%u` is unsupported in cfb_imageblit()",
	     image->depth));

	bytes_per_img_line = (image->width + 7) / 8;

	x = image->dx;
	y = image->dy;
	width = image->width;
	height = image->height;

	if (x + width > info->var.xres) {
		if (x >= info->var.xres)
			return;
		width = info->var.xres - x;
	}
	if (y + height > info->var.yres) {
		if (y >= info->var.yres)
			return;
		height = info->var.yres - y;
	}

	if (image->mask == NULL) {
		for (yi = 0; yi < height; ++yi) {
			for (xi = 0; xi < width; ++xi) {
				byte = yi * bytes_per_img_line + xi / 8;
				bit = 0x80 >> (xi % 8);
				color = image->data[byte] & bit ?
				    image->fg_color : image->bg_color;

				fb_setpixel(info, x + xi, y + yi, color);
			}
		}
	} else {
		for (yi = 0; yi < height; ++yi) {
			for (xi = 0; xi < width; ++xi) {
				byte = yi * bytes_per_img_line + xi / 8;
				bit = 0x80 >> (xi % 8);
				if (image->mask[byte] & bit) {
					color = image->fg_color;

					fb_setpixel(info, x + xi, y + yi, color);
				}
			}
		}
	}
}

void
sys_fillrect(struct linux_fb_info *info, const struct fb_fillrect *rect)
{
	cfb_fillrect(info, rect);
}

void
sys_copyarea(struct linux_fb_info *info, const struct fb_copyarea *area)
{
	cfb_copyarea(info, area);
}

void
sys_imageblit(struct linux_fb_info *info, const struct fb_image *image)
{
	cfb_imageblit(info, image);
}

ssize_t
fb_sys_read(struct linux_fb_info *info, char __user *buf,
    size_t count, loff_t *ppos)
{
	panic("fb_sys_read() not implemented");
	return (0);
}

ssize_t
fb_sys_write(struct linux_fb_info *info, const char __user *buf,
    size_t count, loff_t *ppos)
{
	panic("fb_sys_write() not implemented");
	return (0);
}

ssize_t
fb_io_read(struct linux_fb_info *info, char __user *buf,
    size_t count, loff_t *ppos)
{
	panic("fb_io_read() not implemented");
	return (0);
}

ssize_t
fb_io_write(struct linux_fb_info *info, const char __user *buf,
    size_t count, loff_t *ppos)
{
	panic("fb_io_write() not implemented");
	return (0);
}

int
fb_deferred_io_mmap(struct linux_fb_info *info, struct vm_area_struct *vma)
{
	panic("fb_deferred_io_mmap() not implemented");
	return (0);
}
