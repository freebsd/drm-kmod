/*-
 * Copyright (c) 2023 Beckhoff Automation GmbH & Co. KG
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

#ifndef _BSD_LKPI_UAPI_LINUX_FB_H_
#define	_BSD_LKPI_UAPI_LINUX_FB_H_

#include <sys/fbio.h>

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/pci.h>

#define	FB_ROTATE_UR	0
#define	FB_ROTATE_CW	1
#define	FB_ROTATE_UD	2
#define	FB_ROTATE_CCW	3

#define	FBIO_WAITFORVSYNC	_IOW('F', 0x20, uint32_t)

#define PICOS2KHZ(a) (1000000000UL/(a))
#define KHZ2PICOS(a) (1000000000UL/(a))

#define FB_TYPE_PACKED_PIXELS		0
#define FB_TYPE_PLANES			1
#define FB_TYPE_INTERLEAVED_PLANES	2
#define FB_TYPE_TEXT			3
#define FB_TYPE_VGA_PLANES		4
#define FB_TYPE_FOURCC			5

#define FB_VISUAL_MONO01		0
#define FB_VISUAL_MONO10		1
#define FB_VISUAL_TRUECOLOR		2
#define FB_VISUAL_PSEUDOCOLOR		3
#define FB_VISUAL_DIRECTCOLOR		4
#define FB_VISUAL_STATIC_PSEUDOCOLOR	5
#define FB_VISUAL_FOURCC		6

#define FB_ACCEL_NONE		0

#define	FBINFO_DEFAULT		0
#define	FBINFO_VIRTFB		1
#define	FBINFO_READS_FAST	2

#define FBINFO_HIDE_SMEM_START  0x200000

struct linux_fb_info;
struct vm_area_struct;

#ifdef __FreeBSD__
extern int linuxkpi_skip_ddb;
#endif

struct fb_fix_screeninfo {
	char id[16];
	unsigned long smem_start;
	uint32_t smem_len;
	uint32_t type;
	uint32_t type_aux;
	uint32_t visual;
	uint16_t xpanstep;
	uint16_t ypanstep;
	uint16_t ywrapstep;
	uint32_t line_length;
	unsigned long mmio_start;
	uint32_t mmio_len;
	uint32_t accel;
	uint16_t capabilities;
	uint16_t reserved[2];
};

struct fb_bitfield {
	uint32_t offset;
	uint32_t length;
	uint32_t msb_right;
};

#define FB_ACTIVATE_NOW		0

#define FB_ACCELF_TEXT		1

struct fb_var_screeninfo {
	uint32_t xres;
	uint32_t yres;
	uint32_t xres_virtual;
	uint32_t yres_virtual;
	uint32_t xoffset;
	uint32_t yoffset;

	uint32_t bits_per_pixel;
	uint32_t grayscale;

	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;

	uint32_t nonstd;

	uint32_t activate;

	uint32_t height;
	uint32_t width;

	uint32_t accel_flags;

	uint32_t pixclock;
	uint32_t left_margin;
	uint32_t right_margin;
	uint32_t upper_margin;
	uint32_t lower_margin;
	uint32_t hsync_len;
	uint32_t vsync_len;
	uint32_t sync;
	uint32_t vmode;
	uint32_t rotate;
	uint32_t colorspace;
	uint32_t reserved[4];
};

struct fb_cmap {
	uint32_t start;
	uint32_t len;
	uint16_t *red;
	uint16_t *green;
	uint16_t *blue;
	uint16_t *transp;
};

/* VESA Blanking Levels */
#define VESA_NO_BLANKING        0
#define VESA_VSYNC_SUSPEND      1
#define VESA_HSYNC_SUSPEND      2
#define VESA_POWERDOWN          3

enum {
	FB_BLANK_UNBLANK       = VESA_NO_BLANKING,
	FB_BLANK_NORMAL        = VESA_NO_BLANKING + 1,
	FB_BLANK_VSYNC_SUSPEND = VESA_VSYNC_SUSPEND + 1,
	FB_BLANK_HSYNC_SUSPEND = VESA_HSYNC_SUSPEND + 1,
	FB_BLANK_POWERDOWN     = VESA_POWERDOWN + 1
};

struct fb_vblank {
	uint32_t flags;
	uint32_t count;
	uint32_t vcount;
	uint32_t hcount;
	uint32_t reserved[4];
};

#define ROP_COPY 0
#define ROP_XOR  1

struct fb_copyarea {
	uint32_t dx;
	uint32_t dy;
	uint32_t width;
	uint32_t height;
	uint32_t sx;
	uint32_t sy;
};

struct fb_fillrect {
	uint32_t dx;
	uint32_t dy;
	uint32_t width;
	uint32_t height;
	uint32_t color;
	uint32_t rop;
};

struct fb_image {
	uint32_t dx;
	uint32_t dy;
	uint32_t width;
	uint32_t height;
	uint32_t fg_color;
	uint32_t bg_color;
	uint8_t  depth;
	const char *data;
	struct fb_cmap cmap;
#ifdef __FreeBSD__
	const char *mask;
	uint32_t vt_width;
#endif
};

struct fbcurpos {
	uint16_t x, y;
};

struct fb_cursor {
	uint16_t set;
	uint16_t enable;
	uint16_t rop;
	const char *mask;
	struct fbcurpos hot;
	struct fb_image	image;
};

struct fb_blit_caps {
	uint32_t x;
	uint32_t y;
	uint32_t len;
	uint32_t flags;
};

struct fb_ops {
	struct module *owner;
	int (*fb_open)(struct linux_fb_info *, int);
	int (*fb_release)(struct linux_fb_info *, int);
	ssize_t (*fb_read)(struct linux_fb_info *, char *, size_t, loff_t *);
	ssize_t (*fb_write)(struct linux_fb_info *, const char *, size_t, loff_t *);
	int (*fb_check_var)(struct fb_var_screeninfo *, struct linux_fb_info *);
	int (*fb_set_par)(struct linux_fb_info *);
	int (*fb_setcolreg)(unsigned, unsigned, unsigned, unsigned, unsigned,
	    struct linux_fb_info *);
	int (*fb_setcmap)(struct fb_cmap *, struct linux_fb_info *);
	int (*fb_blank)(int, struct linux_fb_info *);
	int (*fb_pan_display)(struct fb_var_screeninfo *, struct linux_fb_info *);
	void (*fb_fillrect) (struct linux_fb_info *, const struct fb_fillrect *);
	void (*fb_copyarea) (struct linux_fb_info *, const struct fb_copyarea *);
	void (*fb_imageblit) (struct linux_fb_info *, const struct fb_image *);
	int (*fb_cursor) (struct linux_fb_info *, struct fb_cursor *);
	int (*fb_sync)(struct linux_fb_info *);
	int (*fb_ioctl)(struct linux_fb_info *, unsigned int, unsigned long);
	int (*fb_compat_ioctl)(struct linux_fb_info *, unsigned, unsigned long);
	int (*fb_mmap)(struct linux_fb_info *, struct vm_area_struct *);
	void (*fb_get_caps)(struct linux_fb_info *, struct fb_blit_caps *,
	    struct fb_var_screeninfo *);
	void (*fb_destroy)(struct linux_fb_info *);
	int (*fb_debug_enter)(struct linux_fb_info *);
	int (*fb_debug_leave)(struct linux_fb_info *);
};

struct linux_fb_info {
	int flags;
	int fbcon_rotate_hint;

	struct fb_var_screeninfo var;
	struct fb_fix_screeninfo fix;

	const struct fb_ops *fbops;
	struct device *device;
	struct device *dev;
	union {
		char __iomem *screen_base;
		char *screen_buffer;
	};
	unsigned long screen_size;
	void *pseudo_palette;
#define FBINFO_STATE_RUNNING	0
#define FBINFO_STATE_SUSPENDED	1
	uint32_t state;
	void *par;
	bool skip_vt_switch;

#ifdef __FreeBSD__
	struct fb_info fbio;
	device_t fb_bsddev;
	struct task fb_mode_task;

	/* i915 fictitious pages area */
	resource_size_t aperture_base;
	resource_size_t aperture_size;
#endif
} __aligned(sizeof(long));

#define __FB_DEFAULT_IOMEM_OPS_RDWR \
	.fb_read	= fb_io_read, \
	.fb_write	= fb_io_write

#define __FB_DEFAULT_IOMEM_OPS_DRAW \
	.fb_fillrect	= cfb_fillrect, \
	.fb_copyarea	= cfb_copyarea, \
	.fb_imageblit	= cfb_imageblit

#define __FB_DEFAULT_IOMEM_OPS_MMAP \
	.fb_mmap	= NULL /* default implementation */

#define FB_DEFAULT_IOMEM_OPS \
	__FB_DEFAULT_IOMEM_OPS_RDWR, \
	__FB_DEFAULT_IOMEM_OPS_DRAW, \
	__FB_DEFAULT_IOMEM_OPS_MMAP

#define FB_GEN_DEFAULT_DEFERRED_IOMEM_OPS(_pfx, _range, _area)		\
	static void							\
	_pfx ## _fillrect(struct linux_fb_info *fi, const struct fb_fillrect *fr) \
	{								\
		cfb_fillrect(fi, fr);					\
		_area(fi, fr->dx, fr->dy, fr->width, fr->height);	\
	}								\
	static void							\
	_pfx ## _copyarea(struct linux_fb_info *fi, const struct fb_copyarea *fca) \
	{								\
		cfb_copyarea(fi, fca);					\
		_area(fi, fca->dx, fca->dy, fca->width, fca->height);	\
	}								\
	static void							\
	_pfx ## _imageblit(struct linux_fb_info *fi, const struct fb_image *img) \
	{								\
		cfb_imageblit(fi, img);					\
		_area(fi, img->dx, img->dy, img->width, img->height);	\
	}

#define FB_GEN_DEFAULT_DEFERRED_SYSMEM_OPS(...) \
	FB_GEN_DEFAULT_DEFERRED_IOMEM_OPS(__VA_ARGS__)

#define __FB_DEFAULT_DEFERRED_OPS_RDWR(...)	\
	.fb_read	= fb_io_read,		\
	.fb_write	= fb_io_write

#define __FB_DEFAULT_DEFERRED_OPS_DRAW(_pfx)	\
	.fb_fillrect	= _pfx ## _fillrect,	\
	.fb_copyarea	= _pfx ## _copyarea,	\
	.fb_imageblit	= _pfx ## _imageblit

#define __FB_DEFAULT_DEFERRED_OPS_MMAP(...)	\
	.fb_mmap	= linuxkpi_fb_deferred_io_mmap

#define FB_DEFAULT_DEFERRED_OPS(_pfx)		\
	__FB_DEFAULT_DEFERRED_OPS_RDWR(),	\
	__FB_DEFAULT_DEFERRED_OPS_DRAW(_pfx),	\
	__FB_DEFAULT_DEFERRED_OPS_MMAP()

void linuxkpi_cfb_fillrect(struct linux_fb_info *info,
    const struct fb_fillrect *rect);
#define	cfb_fillrect	linuxkpi_cfb_fillrect
void linuxkpi_cfb_copyarea(struct linux_fb_info *info,
    const struct fb_copyarea *area);
#define	cfb_copyarea	linuxkpi_cfb_copyarea
void linuxkpi_cfb_imageblit(struct linux_fb_info *info,
     const struct fb_image *image);
#define	cfb_imageblit	linuxkpi_cfb_imageblit
ssize_t linuxkpi_fb_io_read(struct linux_fb_info *info, char __user *buf,
    size_t count, loff_t *ppos);
#define	fb_io_read	linuxkpi_fb_io_read
ssize_t linuxkpi_fb_io_write(struct linux_fb_info *info, const char __user *buf,
    size_t count, loff_t *ppos);
#define	fb_io_write	linuxkpi_fb_io_write
int linuxkpi_fb_deferred_io_mmap(struct linux_fb_info *info,
    struct vm_area_struct *vma);
#define	fb_deferred_io_mmap	linuxkpi_fb_deferred_io_mmap

static inline void
sys_fillrect(struct linux_fb_info *info, const struct fb_fillrect *rect)
{
	cfb_fillrect(info, rect);
}

static inline void
sys_copyarea(struct linux_fb_info *info, const struct fb_copyarea *area)
{
	cfb_copyarea(info, area);
}

static inline void
sys_imageblit(struct linux_fb_info *info, const struct fb_image *image)
{
	cfb_imageblit(info, image);
}

static inline ssize_t
fb_sys_read(struct linux_fb_info *info, char __user *buf, size_t count,
    loff_t *ppos)
{
	return (fb_io_read(info, buf, count, ppos));
}

static inline ssize_t
fb_sys_write(struct linux_fb_info *info, const char __user *buf, size_t count,
    loff_t *ppos)
{
	return (fb_io_write(info, buf, count, ppos));
}

int linuxkpi_register_framebuffer(struct linux_fb_info *fb_info);
#define	register_framebuffer(...)	\
    linuxkpi_register_framebuffer(__VA_ARGS__)
int linuxkpi_unregister_framebuffer(struct linux_fb_info *fb_info);
#define	unregister_framebuffer(...)	\
    linuxkpi_unregister_framebuffer(__VA_ARGS__)
struct linux_fb_info *linuxkpi_framebuffer_alloc(size_t size, struct device *dev);
#define	framebuffer_alloc(...)	linuxkpi_framebuffer_alloc(__VA_ARGS__)
void linuxkpi_framebuffer_release(struct linux_fb_info *info);
#define	framebuffer_release(...)	\
    linuxkpi_framebuffer_release(__VA_ARGS__)
#define	fb_set_suspend(x, y)	0

/* updated FreeBSD fb_info */
int linuxkpi_fb_get_options(const char *name, char **option);
#define	fb_get_options(...)	linuxkpi_fb_get_options(__VA_ARGS__)

#endif
