/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2013 The FreeBSD Foundation
 * Copyright (c) 2023 Jean-Sébastien Pédron <dumbbell@FreeBSD.org>
 *
 * This initial software `sys/dev/vt/hw/vt_fb.c` was developed by Aleksandr
 * Rybalko under sponsorship from the FreeBSD Foundation.
 * This file is a copy of the initial file and is modified by Jean-Sébastien
 * Pédron.
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
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/fbio.h>
#include <sys/module.h>
#include <sys/reboot.h>

#include <dev/vt/vt.h>
#include <dev/vt/hw/fb/vt_fb.h>
#include <dev/vt/colors/vt_termcolors.h>

#include <linux/fb.h>

#include "fb_if.h"

/*
 * `drm_fb_helper.h` redefines `fb_info` to be `linux_fb_info` to manage the
 * name conflict between the Linux and FreeBSD structures, while avoiding a
 * extensive rewrite and use of macros in the original drm_fb_helper.[ch]
 * files.
 *
 * We need to undo this here because we use both structures.
 */
#undef	fb_info

#define	to_linux_fb_info(f)	container_of(f, struct linux_fb_info, fbio);

static vd_blank_t		vt_drmfb_blank;
static vd_bitblt_bmp_t		vt_drmfb_bitblt_bitmap;
static vd_bitblt_argb_t		vt_drmfb_bitblt_argb;
static vd_drawrect_t		vt_drmfb_drawrect;
static vd_setpixel_t		vt_drmfb_setpixel;
static vd_invalidate_text_t	vt_drmfb_invalidate_text;

static struct vt_driver vt_drmfb_driver = {
	.vd_name = "drmfb",
	.vd_blank = vt_drmfb_blank,
	/*
	 * .vd_bitblt_text is unset.
	 *
	 * We use the default implementation in vt(4) which copies the
	 * characters first and bitblt_bmp them in a second step outside of the
	 * vtbuf lock. Thus the `vd_bitblt_after_vtbuf_unlock` flag set below.
	 * We need this because vtbuf is a spin mutex and
	 * `vt_drmfb_bitblt_bitmap()` may sleep.
	 */
	.vd_bitblt_bmp = vt_drmfb_bitblt_bitmap,
	.vd_bitblt_argb = vt_drmfb_bitblt_argb,
	.vd_drawrect = vt_drmfb_drawrect,
	.vd_setpixel = vt_drmfb_setpixel,
	.vd_invalidate_text = vt_drmfb_invalidate_text,
	.vd_priority = VD_PRIORITY_GENERIC+20,

	/* Use generic implementation */
	.vd_suspend = vt_suspend,
	.vd_resume = vt_resume,

	/* Use vt_fb implementation */
	.vd_init = vt_fb_init,
	.vd_fini = vt_fb_fini,
	.vd_postswitch = vt_fb_postswitch,
	.vd_fb_ioctl = vt_fb_ioctl,
	.vd_fb_mmap = vt_fb_mmap,

	.vd_bitblt_after_vtbuf_unlock = true,
};

VT_DRIVER_DECLARE(vt_drmfb, vt_drmfb_driver);

static void
vt_drmfb_setpixel(struct vt_device *vd, int x, int y, term_color_t color)
{
	vt_drmfb_drawrect(vd, x, y, x, y, 1, color);
}

static void
vt_drmfb_drawrect(
    struct vt_device *vd,
    int x1, int y1, int x2, int y2, int fill,
    term_color_t color)
{
	struct fb_info *fbio;
	struct linux_fb_info *info;
	struct fb_fillrect rect;

	fbio = vd->vd_softc;
	info = to_linux_fb_info(fbio);
	if (info->fbops->fb_fillrect == NULL)
		return;

	KASSERT(
	    (x2 >= x1),
	    ("Invalid rectangle X coordinates passed to vd_drawrect: "
	     "x1=%d > x2=%d", x1, x2));
	KASSERT(
	    (y2 >= y1),
	    ("Invalid rectangle Y coordinates passed to vd_drawrect: "
	     "y1=%d > y2=%d", y1, y2));
	KASSERT(
	    (fill != 0),
	    ("`fill=0` argument to vd_drawrect unsupported in vt_drmfb"));

	rect.dx = x1;
	rect.dy = y1;
	rect.width = x2 - x1 + 1;
	rect.height = y2 - y1 + 1;
	rect.color = fbio->fb_cmap[color];
	rect.rop = ROP_COPY;

	info->fbops->fb_fillrect(info, &rect);
}

static void
vt_drmfb_blank(struct vt_device *vd, term_color_t color)
{
	struct fb_info *fbio;
	struct linux_fb_info *info;
	int x1, y1, x2, y2;

	fbio = vd->vd_softc;
	info = to_linux_fb_info(fbio);

	x1 = info->var.xoffset;
	y1 = info->var.yoffset;
	x2 = info->var.xres - 1;
	y2 = info->var.yres - 1;

	vt_drmfb_drawrect(vd, x1, y1, x2, y2, 1, color);
}

static void
vt_drmfb_bitblt_bitmap(struct vt_device *vd, const struct vt_window *vw,
    const uint8_t *pattern, const uint8_t *mask,
    unsigned int width, unsigned int height,
    unsigned int x, unsigned int y, term_color_t fg, term_color_t bg)
{
	struct fb_info *fbio;
	struct linux_fb_info *info;
	struct fb_image image;

	fbio = vd->vd_softc;
	info = to_linux_fb_info(fbio);
	if (info->fbops->fb_imageblit == NULL)
		return;

	/* Bound by right and bottom edges. */
	if (y >= vw->vw_draw_area.tr_end.tp_row)
		return;
	if (x >= vw->vw_draw_area.tr_end.tp_col)
		return;

	image.dx = x;
	image.dy = y;
	image.width = width;
	image.height = height;
	image.fg_color = fbio->fb_cmap[fg];
	image.bg_color = fbio->fb_cmap[bg];
	image.depth = 1;
	image.data = pattern;
	image.mask = mask; // Specific to FreeBSD to display the mouse pointer.

	if (!kdb_active && !KERNEL_PANICKED())
		linux_set_current(curthread);

	info->fbops->fb_imageblit(info, &image);
}

static int
vt_drmfb_bitblt_argb(struct vt_device *vd, const struct vt_window *vw,
    const uint8_t *argb,
    unsigned int width, unsigned int height,
    unsigned int x, unsigned int y)
{
	struct fb_info *fbio;
	struct linux_fb_info *info;
	struct fb_image image;

	fbio = vd->vd_softc;
	info = to_linux_fb_info(fbio);
	if (info->fbops->fb_imageblit == NULL)
		return (ENOTSUP);

	/* Bound by right and bottom edges. */
	if (y >= vw->vw_draw_area.tr_end.tp_row)
		return (EINVAL);
	if (x >= vw->vw_draw_area.tr_end.tp_col)
		return (EINVAL);

	image.dx = x;
	image.dy = y;
	image.width = width;
	image.height = height;
	image.depth = 32;
	image.data = argb;

	info->fbops->fb_imageblit(info, &image);

	return (0);
}

static void
vt_drmfb_invalidate_text(struct vt_device *vd, const term_rect_t *area)
{
	unsigned int col, row;
	size_t z;

	for (row = area->tr_begin.tp_row; row < area->tr_end.tp_row; ++row) {
		for (col = area->tr_begin.tp_col; col < area->tr_end.tp_col;
		    ++col) {
			z = row * PIXEL_WIDTH(VT_FB_MAX_WIDTH) + col;
			if (z >= PIXEL_HEIGHT(VT_FB_MAX_HEIGHT) *
			    PIXEL_WIDTH(VT_FB_MAX_WIDTH))
				continue;
			if (vd->vd_drawn)
				vd->vd_drawn[z] = 0;
			if (vd->vd_drawnfg)
				vd->vd_drawnfg[z] = 0;
			if (vd->vd_drawnbg)
				vd->vd_drawnbg[z] = 0;
			if (vd->vd_pos_to_flush)
				vd->vd_pos_to_flush[z] = true;
		}
	}
}

/* Newbus methods. */
static int
vt_drmfb_probe(device_t dev)
{
	char *disabled;

	disabled = kern_getenv("kern.vt.disable_drmfb");
	if (disabled != NULL && strtoul(disabled, NULL, 10) != 0)
		return (ENXIO);

	return (BUS_PROBE_GENERIC);
}

static int
vt_drmfb_attach(device_t dev)
{
	struct fb_info *fbio;

	fbio = FB_GETINFO(device_get_parent(dev));
	if (fbio == NULL)
		return (ENXIO);

	return (vt_allocate(&vt_drmfb_driver, fbio));
}

static int
vt_drmfb_detach(device_t dev)
{
	struct fb_info *fbio;

	fbio = FB_GETINFO(device_get_parent(dev));
	if (fbio == NULL)
		return (ENXIO);

	return (vt_deallocate(&vt_drmfb_driver, fbio));
}


static device_method_t vt_drmfb_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe,		vt_drmfb_probe),
	DEVMETHOD(device_attach,	vt_drmfb_attach),
	DEVMETHOD(device_detach,	vt_drmfb_detach),
	DEVMETHOD(device_shutdown,      bus_generic_shutdown),

	DEVMETHOD_END
};

driver_t vt_drmfb_bus_driver = {
	"fbd",
	vt_drmfb_methods,
	0
};

DRIVER_MODULE(vt_drmfb, drmn, vt_drmfb_bus_driver, NULL, NULL);
MODULE_VERSION(vt_drmfb, 1);
