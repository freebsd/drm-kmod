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

struct fb_fix_screeninfo {
	char id[16];
#ifdef __linux__
	unsigned long smem_start;
#elif defined(__FreeBSD__)
	vm_paddr_t smem_start;
#endif

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

#endif
