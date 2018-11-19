/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2014 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Aleksandr Rybalko under sponsorship from the
 * FreeBSD Foundation.
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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/fbio.h>
#include <sys/linker.h>
#include <sys/kdb.h>

#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>

#include "opt_platform.h"

#include <machine/metadata.h>
#include <machine/vmparam.h>
#include <vm/vm.h>
#include <vm/pmap.h>

#include <linux/fb.h>

#undef fb_info
#include <dev/vt/vt.h>
#include <dev/vt/hw/fb/vt_fb.h>
#include <dev/vt/colors/vt_termcolors.h>

#include "vmwgfx_fb_freebsd.h"
#include "vmwgfx_drv.h"

/* #define FB_MAJOR		29   /\* /dev/fb* framebuffers *\/ */
#define FBPIXMAPSIZE	(1024 * 8)

extern void fb_info_print(struct fb_info *t);

static struct sx vt_vmwfb_mtx;
SX_SYSINIT(vt_vmwfb_mtx, &vt_vmwfb_mtx, "vmwfb");

static struct linux_fb_info *registered_fb[FB_MAX];
static int num_registered_fb;
/* static struct class *vmw_fb_class; */

static int __vmw_unregister_framebuffer(struct linux_fb_info *fb_info);

/* #include <sys/reboot.h> */

/* int skip_ddb; */

static struct vt_driver vt_vmwfb_driver = {
	.vd_name = "vmwfb",
	.vd_init = vt_vmwfb_init,
	.vd_fini = vt_vmwfb_fini,
	.vd_blank = vt_vmwfb_blank,
	.vd_bitblt_text = vt_vmwfb_bitblt_text,
	.vd_invalidate_text = vt_vmwfb_invalidate_text,
	.vd_bitblt_bmp = vt_vmwfb_bitblt_bitmap,
	.vd_drawrect = vt_vmwfb_drawrect,
	.vd_setpixel = vt_vmwfb_setpixel,
	.vd_postswitch = vt_vmwfb_postswitch,
	.vd_priority = VD_PRIORITY_SPECIFIC,
	.vd_fb_ioctl = vt_vmwfb_ioctl,
	.vd_fb_mmap = vt_vmwfb_mmap,
	.vd_suspend = vt_vmwfb_suspend,
	.vd_resume = vt_vmwfb_resume,
};

VT_DRIVER_DECLARE(vt_vmwfb, vt_vmwfb_driver);


static void
vt_vmwfb_mem_wr1(struct fb_info *sc, uint32_t o, uint8_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	*(uint8_t *)(sc->fb_vbase + o) = v;
}

static void
vt_vmwfb_mem_wr2(struct fb_info *sc, uint32_t o, uint16_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	*(uint16_t *)(sc->fb_vbase + o) = v;
}

static void
vt_vmwfb_mem_wr4(struct fb_info *sc, uint32_t o, uint32_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	*(uint32_t *)(sc->fb_vbase + o) = v;
}

int
vt_vmwfb_ioctl(struct vt_device *vd, u_long cmd, caddr_t data, struct thread *td)
{
	struct fb_info *info;
	int error = 0;

	info = vd->vd_softc;

	switch (cmd) {
	case FBIOGTYPE:
		bcopy(info, (struct fbtype *)data, sizeof(struct fbtype));
		break;

	case FBIO_GETWINORG:	/* get frame buffer window origin */
		*(u_int *)data = 0;
		break;

	case FBIO_GETDISPSTART:	/* get display start address */
		((video_display_start_t *)data)->x = 0;
		((video_display_start_t *)data)->y = 0;
		break;

	case FBIO_GETLINEWIDTH:	/* get scan line width in bytes */
		*(u_int *)data = info->fb_stride;
		break;

	case FBIO_BLANK:	/* blank display */
		if (vd->vd_driver->vd_blank == NULL)
			return (ENODEV);
		vd->vd_driver->vd_blank(vd, TC_BLACK);
		break;

	default:
		error = ENOIOCTL;
		break;
	}

	return (error);
}

int
vt_vmwfb_mmap(struct vt_device *vd, vm_ooffset_t offset, vm_paddr_t *paddr,
    int prot, vm_memattr_t *memattr)
{
	struct fb_info *info;

	info = vd->vd_softc;

	if (info->fb_flags & FB_FLAG_NOMMAP)
		return (ENODEV);

	if (offset >= 0 && offset < info->fb_size) {
		if (info->fb_pbase == 0) {
			*paddr = vtophys((uint8_t *)info->fb_vbase + offset);
		} else {
			*paddr = info->fb_pbase + offset;
			if (info->fb_flags & FB_FLAG_MEMATTR)
				*memattr = info->fb_memattr;
#ifdef VM_MEMATTR_WRITE_COMBINING
			else
				*memattr = VM_MEMATTR_WRITE_COMBINING;
#endif
		}
		return (0);
	}

	return (EINVAL);
}

void
vt_vmwfb_setpixel(struct vt_device *vd, int x, int y, term_color_t color)
{
	struct fb_info *info;
	uint32_t c;
	u_int o;

	info = vd->vd_softc;
	c = info->fb_cmap[color];
	o = info->fb_stride * y + x * FBTYPE_GET_BYTESPP(info);

	if (info->fb_flags & FB_FLAG_NOWRITE)
		return;

	KASSERT((info->fb_vbase != 0), ("Unmapped framebuffer"));

	switch (FBTYPE_GET_BYTESPP(info)) {
	case 1:
		vt_vmwfb_mem_wr1(info, o, c);
		break;
	case 2:
		vt_vmwfb_mem_wr2(info, o, c);
		break;
	case 3:
		vt_vmwfb_mem_wr1(info, o, (c >> 16) & 0xff);
		vt_vmwfb_mem_wr1(info, o + 1, (c >> 8) & 0xff);
		vt_vmwfb_mem_wr1(info, o + 2, c & 0xff);
		break;
	case 4:
		vt_vmwfb_mem_wr4(info, o, c);
		break;
	default:
		/* panic? */
		return;
	}
}

void
vt_vmwfb_drawrect(struct vt_device *vd, int x1, int y1, int x2, int y2, int fill,
    term_color_t color)
{
	int x, y;

	for (y = y1; y <= y2; y++) {
		if (fill || (y == y1) || (y == y2)) {
			for (x = x1; x <= x2; x++)
				vt_vmwfb_setpixel(vd, x, y, color);
		} else {
			vt_vmwfb_setpixel(vd, x1, y, color);
			vt_vmwfb_setpixel(vd, x2, y, color);
		}
	}
}

void
vt_vmwfb_blank(struct vt_device *vd, term_color_t color)
{
	struct fb_info *info;
	uint32_t c;
	u_int o, h;

	info = vd->vd_softc;
	c = info->fb_cmap[color];

	if (info->fb_flags & FB_FLAG_NOWRITE)
		return;

	KASSERT((info->fb_vbase != 0), ("Unmapped framebuffer"));

	switch (FBTYPE_GET_BYTESPP(info)) {
	case 1:
		for (h = 0; h < info->fb_height; h++)
			for (o = 0; o < info->fb_stride; o++)
				vt_vmwfb_mem_wr1(info, h*info->fb_stride + o, c);
		break;
	case 2:
		for (h = 0; h < info->fb_height; h++)
			for (o = 0; o < info->fb_stride; o += 2)
				vt_vmwfb_mem_wr2(info, h*info->fb_stride + o, c);
		break;
	case 3:
		for (h = 0; h < info->fb_height; h++)
			for (o = 0; o < info->fb_stride; o += 3) {
				vt_vmwfb_mem_wr1(info, h*info->fb_stride + o,
				    (c >> 16) & 0xff);
				vt_vmwfb_mem_wr1(info, h*info->fb_stride + o + 1,
				    (c >> 8) & 0xff);
				vt_vmwfb_mem_wr1(info, h*info->fb_stride + o + 2,
				    c & 0xff);
			}
		break;
	case 4:
		for (h = 0; h < info->fb_height; h++)
			for (o = 0; o < info->fb_stride; o += 4)
				vt_vmwfb_mem_wr4(info, h*info->fb_stride + o, c);
		break;
	default:
		/* panic? */
		return;
	}
}

void
vt_vmwfb_bitblt_bitmap(struct vt_device *vd, const struct vt_window *vw,
    const uint8_t *pattern, const uint8_t *mask,
    unsigned int width, unsigned int height,
    unsigned int x, unsigned int y, term_color_t fg, term_color_t bg)
{
	struct fb_info *info;
	struct linux_fb_info *linux_info;
	uint32_t fgc, bgc, cc, o;
	int bpp, bpl, xi, yi;
	int bit, byte;

	info = vd->vd_softc;
	linux_info = container_of(info, struct linux_fb_info, fbio);
	bpp = FBTYPE_GET_BYTESPP(info);
	fgc = info->fb_cmap[fg];
	bgc = info->fb_cmap[bg];
	bpl = (width + 7) / 8; /* Bytes per source line. */

	if (info->fb_flags & FB_FLAG_NOWRITE)
		return;

	KASSERT((info->fb_vbase != 0), ("Unmapped framebuffer"));

	/* Bound by right and bottom edges. */
	if (y + height > vw->vw_draw_area.tr_end.tp_row) {
		if (y >= vw->vw_draw_area.tr_end.tp_row)
			return;
		height = vw->vw_draw_area.tr_end.tp_row - y;
	}
	if (x + width > vw->vw_draw_area.tr_end.tp_col) {
		if (x >= vw->vw_draw_area.tr_end.tp_col)
			return;
		width = vw->vw_draw_area.tr_end.tp_col - x;
	}
	for (yi = 0; yi < height; yi++) {
		for (xi = 0; xi < width; xi++) {
			byte = yi * bpl + xi / 8;
			bit = 0x80 >> (xi % 8);
			/* Skip pixel write, if mask bit not set. */
			if (mask != NULL && (mask[byte] & bit) == 0)
				continue;
			o = (y + yi) * info->fb_stride + (x + xi) * bpp;
			o += vd->vd_transpose;
			cc = pattern[byte] & bit ? fgc : bgc;

			switch(bpp) {
			case 1:
				vt_vmwfb_mem_wr1(info, o, cc);
				break;
			case 2:
				vt_vmwfb_mem_wr2(info, o, cc);
				break;
			case 3:
				/* Packed mode, so unaligned. Byte access. */
				vt_vmwfb_mem_wr1(info, o, (cc >> 16) & 0xff);
				vt_vmwfb_mem_wr1(info, o + 1, (cc >> 8) & 0xff);
				vt_vmwfb_mem_wr1(info, o + 2, cc & 0xff);
				break;
			case 4:
				vt_vmwfb_mem_wr4(info, o, cc);
				break;
			default:
				/* panic? */
				break;
			}
		}
	}
	if (linux_info && linux_info->par)
		vmw_fb_dirty_mark(linux_info->par, x, y, width, height);
}

void
vt_vmwfb_bitblt_text(struct vt_device *vd, const struct vt_window *vw,
    const term_rect_t *area)
{
	unsigned int col, row, x, y;
	unsigned int min_x, min_y, max_x, max_y;
	struct vt_font *vf;
	struct fb_info *info;
	term_char_t c;
	term_color_t fg, bg;
	const uint8_t *pattern;
	size_t z;

	vf = vw->vw_font;
	info = vd->vd_softc;
	min_x = UINT_MAX;
	min_y = UINT_MAX;
	max_x = 0;
	max_y = 0;

	for (row = area->tr_begin.tp_row; row < area->tr_end.tp_row; ++row) {
		for (col = area->tr_begin.tp_col; col < area->tr_end.tp_col;
		    ++col) {
			x = col * vf->vf_width +
			    vw->vw_draw_area.tr_begin.tp_col;
			y = row * vf->vf_height +
			    vw->vw_draw_area.tr_begin.tp_row;

			c = VTBUF_GET_FIELD(&vw->vw_buf, row, col);
			pattern = vtfont_lookup(vf, c);
			vt_determine_colors(c,
			    VTBUF_ISCURSOR(&vw->vw_buf, row, col), &fg, &bg);

			z = row * PIXEL_WIDTH(VT_FB_MAX_WIDTH) + col;
			if (vd->vd_drawn && (vd->vd_drawn[z] == c) &&
			    vd->vd_drawnfg && (vd->vd_drawnfg[z] == fg) &&
			    vd->vd_drawnbg && (vd->vd_drawnbg[z] == bg))
				continue;

			vt_vmwfb_bitblt_bitmap(vd, vw,
			    pattern, NULL, vf->vf_width, vf->vf_height,
			    x, y, fg, bg);

			if (vd->vd_drawn)
				vd->vd_drawn[z] = c;
			if (vd->vd_drawnfg)
				vd->vd_drawnfg[z] = fg;
			if (vd->vd_drawnbg)
				vd->vd_drawnbg[z] = bg;

			if (x < min_x)
				min_x = x;
			if (y < min_y)
				min_y = y;
			if (x + vf->vf_width > max_x)
				max_x = x + vf->vf_width;
			if (y + vf->vf_height > max_y)
				max_y = y + vf->vf_height;
		}
	}

#ifndef SC_NO_CUTPASTE
	if (!vd->vd_mshown)
		return;

	term_rect_t drawn_area;

	drawn_area.tr_begin.tp_col = area->tr_begin.tp_col * vf->vf_width;
	drawn_area.tr_begin.tp_row = area->tr_begin.tp_row * vf->vf_height;
	drawn_area.tr_end.tp_col = area->tr_end.tp_col * vf->vf_width;
	drawn_area.tr_end.tp_row = area->tr_end.tp_row * vf->vf_height;

	if (vt_is_cursor_in_area(vd, &drawn_area)) {
		vt_vmwfb_bitblt_bitmap(vd, vw,
		    vd->vd_mcursor->map, vd->vd_mcursor->mask,
		    vd->vd_mcursor->width, vd->vd_mcursor->height,
		    vd->vd_mx_drawn + vw->vw_draw_area.tr_begin.tp_col,
		    vd->vd_my_drawn + vw->vw_draw_area.tr_begin.tp_row,
		    vd->vd_mcursor_fg, vd->vd_mcursor_bg);
	}
#endif
}

void
vt_vmwfb_invalidate_text(struct vt_device *vd, const term_rect_t *area)
{
	unsigned int col, row;
	size_t z;

	for (row = area->tr_begin.tp_row; row < area->tr_end.tp_row; ++row) {
		for (col = area->tr_begin.tp_col; col < area->tr_end.tp_col;
		    ++col) {
			z = row * PIXEL_WIDTH(VT_FB_MAX_WIDTH) + col;
			if (vd->vd_drawn)
				vd->vd_drawn[z] = 0;
			if (vd->vd_drawnfg)
				vd->vd_drawnfg[z] = 0;
			if (vd->vd_drawnbg)
				vd->vd_drawnbg[z] = 0;
		}
	}
}

void
vt_vmwfb_postswitch(struct vt_device *vd)
{
	struct fb_info *info;

	info = vd->vd_softc;

	if (info->enter != NULL)
		info->enter(info->fb_priv);
}

static int
vt_vmwfb_init_cmap(uint32_t *cmap, int depth)
{

	switch (depth) {
	case 8:
		return (vt_generate_cons_palette(cmap, COLOR_FORMAT_RGB,
		    0x7, 5, 0x7, 2, 0x3, 0));
	case 15:
		return (vt_generate_cons_palette(cmap, COLOR_FORMAT_RGB,
		    0x1f, 10, 0x1f, 5, 0x1f, 0));
	case 16:
		return (vt_generate_cons_palette(cmap, COLOR_FORMAT_RGB,
		    0x1f, 11, 0x3f, 5, 0x1f, 0));
	case 24:
	case 32: /* Ignore alpha. */
		return (vt_generate_cons_palette(cmap, COLOR_FORMAT_RGB,
		    0xff, 16, 0xff, 8, 0xff, 0));
	default:
		return (1);
	}
}

int
vt_vmwfb_init(struct vt_device *vd)
{
	struct fb_info *info;
	u_int margin;
	int err;

	info = vd->vd_softc;
	vd->vd_height = MIN(VT_FB_MAX_HEIGHT, info->fb_height);
	margin = (info->fb_height - vd->vd_height) >> 1;
	vd->vd_transpose = margin * info->fb_stride;
	vd->vd_width = MIN(VT_FB_MAX_WIDTH, info->fb_width);
	margin = (info->fb_width - vd->vd_width) >> 1;
	vd->vd_transpose += margin * (info->fb_bpp / NBBY);
	vd->vd_video_dev = info->fb_video_dev;

	if (info->fb_size == 0)
		return (CN_DEAD);

	if (info->fb_pbase == 0 && info->fb_vbase == 0)
		info->fb_flags |= FB_FLAG_NOMMAP;

	if (info->fb_cmsize <= 0) {
		err = vt_vmwfb_init_cmap(info->fb_cmap, FBTYPE_GET_BPP(info));
		if (err)
			return (CN_DEAD);
		info->fb_cmsize = 16;
	}

	/* Clear the screen. */
	vd->vd_driver->vd_blank(vd, TC_BLACK);

	/* Wakeup screen. KMS need this. */
	vt_vmwfb_postswitch(vd);

	return (CN_INTERNAL);
}

void
vt_vmwfb_fini(struct vt_device *vd, void *softc)
{

	vd->vd_video_dev = NULL;
}

int
vt_vmwfb_attach(struct fb_info *info)
{
	vt_allocate(&vt_vmwfb_driver, info);

	return (0);
}

int
vt_vmwfb_detach(struct fb_info *info)
{

	vt_deallocate(&vt_vmwfb_driver, info);

	return (0);
}

void
vt_vmwfb_suspend(struct vt_device *vd)
{

	vt_suspend(vd);
}

void
vt_vmwfb_resume(struct vt_device *vd)
{

	vt_resume(vd);
}



struct linux_fb_info *
vmw_framebuffer_alloc(size_t size, struct device *dev)
{
#define BYTES_PER_LONG (BITS_PER_LONG/8)
#define PADDING (BYTES_PER_LONG - (sizeof(struct linux_fb_info) % BYTES_PER_LONG))
	int fb_info_size = sizeof(struct linux_fb_info);
	struct linux_fb_info *info;
	char *p;

	if (size)
		fb_info_size += PADDING;

	p = malloc(sizeof(*info), DRM_MEM_KMS, M_WAITOK | M_ZERO);
	info = (struct linux_fb_info *)p;

	if (size)
		info->par = p + fb_info_size;

	info->device = dev;

	return info;
#undef PADDING
#undef BYTES_PER_LONG
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

static void
fb_destroy_modes(struct list_head *head)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, head) {
		list_del(pos);
		kfree(pos);
	}
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

static int
__vmw_unregister_framebuffer(struct linux_fb_info *fb_info)
{
	struct fb_event event;
	int i, ret = 0;

	i = fb_info->node;
	if (i < 0 || i >= FB_MAX || registered_fb[i] != fb_info)
		return -EINVAL;

	if (num_registered_fb == 1)
		vt_vmwfb_detach(&fb_info->fbio);

	unlink_framebuffer(fb_info);
	if (fb_info->pixmap.addr &&
	    (fb_info->pixmap.flags & FB_PIXMAP_DEFAULT))
		kfree(fb_info->pixmap.addr);
	fb_destroy_modes(&fb_info->modelist);
	registered_fb[i] = NULL;
	num_registered_fb--;
	event.info = fb_info;

	put_fb_info(fb_info);
	return ret;
}

int
vmw_unregister_framebuffer(struct linux_fb_info *fb_info)
{
	int rc;

	sx_xlock(&vt_vmwfb_mtx);
	rc = __vmw_unregister_framebuffer(fb_info);
	sx_xunlock(&vt_vmwfb_mtx);
	return (rc);
}

#define VGA_FB_PHYS 0xA0000
static void
__vmw_remove_conflicting(struct apertures_struct *a, const char *name, bool primary)
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
			__vmw_unregister_framebuffer(registered_fb[i]);
		}
	}
}

static int
__vmw_register_framebuffer(struct linux_fb_info *fb_info)
{
	int i, err;
	struct fb_event event;
	struct fb_videomode mode;

	__vmw_remove_conflicting(fb_info->apertures, fb_info->fix.id, is_primary(fb_info));

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
				     VM_MEMATTR_WRITE_COMBINING);


	fb_info->dev = NULL;
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

	if (num_registered_fb == 1) {
		/* tell vt_vmwfb to initialize color map */
		fb_info->fbio.fb_cmsize = 0;
		if (fb_info->fbio.fb_bpp == 0) {
			device_printf(fb_info->fbio.fb_fbd_dev,
				      "fb_bpp not set, setting to 8");
			fb_info->fbio.fb_bpp = 32;
		}
		if ((err = vt_vmwfb_attach(&fb_info->fbio)) != 0)
			return (err);
	}
	fb_info_print(&fb_info->fbio);

	return 0;
}

int
vmw_register_framebuffer(struct linux_fb_info *fb_info)
{
	int rc;

	sx_xlock(&vt_vmwfb_mtx);
	rc = __vmw_register_framebuffer(fb_info);
	sx_xunlock(&vt_vmwfb_mtx);
	return (rc);
}
