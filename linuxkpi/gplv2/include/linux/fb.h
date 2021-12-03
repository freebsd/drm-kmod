#ifndef __LINUX_FB_H_
#define __LINUX_FB_H_
#include <sys/fbio.h>
#include <uapi/linux/fb.h>

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/backlight.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <asm/io.h>
#include <linux/notifier.h>

struct linux_fb_info;
struct videomode;
struct vm_area_struct;

#define FB_TYPE_PACKED_PIXELS		0

enum {
	FB_VISUAL_TRUECOLOR = 0,
	FB_VISUAL_PSEUDOCOLOR,
};

#define FB_ACCEL_NONE		0

#define FB_ACTIVATE_NOW		0

#define FB_ACCELF_TEXT		1

#define KHZ2PICOS(a) (1000000000UL/(a))

struct fb_fix_screeninfo {
	char id[16];			/* identification string eg "TT Builtin" */
	unsigned long smem_start;	/* Start of frame buffer mem */
					/* (physical address) */
	__u32 smem_len;			/* Length of frame buffer mem */
	__u32 type;			/* see FB_TYPE_*		*/
	__u32 type_aux;			/* Interleave for interleaved Planes */
	__u32 visual;			/* see FB_VISUAL_*		*/ 
	__u16 xpanstep;			/* zero if no hardware panning  */
	__u16 ypanstep;			/* zero if no hardware panning  */
	__u16 ywrapstep;		/* zero if no hardware ywrap    */
	__u32 line_length;		/* length of a line in bytes    */
	unsigned long mmio_start;	/* Start of Memory Mapped I/O   */
					/* (physical address) */
	__u32 mmio_len;			/* Length of Memory Mapped I/O  */
	__u32 accel;			/* Indicate to driver which	*/
					/*  specific chip/card we have	*/
	__u16 capabilities;		/* see FB_CAP_*			*/
	__u16 reserved[2];		/* Reserved for future compatibility */
};

struct fb_var_screeninfo {
	__u32 xres;			/* visible resolution		*/
	__u32 yres;
	__u32 xres_virtual;		/* virtual resolution		*/
	__u32 yres_virtual;
	__u32 xoffset;			/* offset from virtual to visible */
	__u32 yoffset;			/* resolution			*/

	__u32 bits_per_pixel;		/* guess what			*/
	__u32 grayscale;		/* 0 = color, 1 = grayscale,	*/
					/* >1 = FOURCC			*/

	__u32 nonstd;			/* != 0 Non standard pixel format */

	__u32 activate;			/* see FB_ACTIVATE_*		*/

	__u32 height;			/* height of picture in mm    */
	__u32 width;			/* width of picture in mm     */

	__u32 accel_flags;		/* (OBSOLETE) see fb_info.flags */

	/* Timing: All values in pixclocks, except pixclock (of course) */
	__u32 pixclock;			/* pixel clock in ps (pico seconds) */
	__u32 left_margin;		/* time from sync to picture	*/
	__u32 right_margin;		/* time from picture to sync	*/
	__u32 upper_margin;		/* time from sync to picture	*/
	__u32 lower_margin;
	__u32 hsync_len;		/* length of horizontal sync	*/
	__u32 vsync_len;		/* length of vertical sync	*/
	__u32 sync;			/* see FB_SYNC_*		*/
	__u32 vmode;			/* see FB_VMODE_*		*/
	__u32 rotate;			/* angle we rotate counter clockwise */
	__u32 colorspace;		/* colorspace for FOURCC-based modes */
	__u32 reserved[4];		/* Reserved for future compatibility */
};

struct fb_cmap {
	__u32 start;			/* First entry	*/
	__u32 len;			/* Number of entries */
	__u16 *red;			/* Red values	*/
	__u16 *green;
	__u16 *blue;
	__u16 *transp;			/* transparency, can be NULL */
};

enum {
	FB_BLANK_UNBLANK       = 0,
	FB_BLANK_NORMAL,
	FB_BLANK_VSYNC_SUSPEND,
	FB_BLANK_HSYNC_SUSPEND,
	FB_BLANK_POWERDOWN,
};

struct fb_ops {
	/* open/release and usage marking */
	struct module *owner;

	/* set the video mode according to info->var */
	int (*fb_set_par)(struct linux_fb_info *info);

	/* blank display */
	int (*fb_blank)(int blank, struct linux_fb_info *info);

	/* teardown any resources to do with this framebuffer */
	void (*fb_destroy)(struct linux_fb_info *info);
};

struct linux_fb_info {
	struct fb_var_screeninfo var;	/* Current var */
	struct fb_fix_screeninfo fix;	/* Current fix */
	struct fb_cmap cmap;		/* Current cmap */

	const struct fb_ops *fbops;
	struct device *device;		/* This is the parent */
	struct device *dev;		/* This is this fb device */
	union {
		char __iomem *screen_base;	/* Virtual address */
		char *screen_buffer;
	};
	unsigned long screen_size;	/* Amount of ioremapped VRAM or 0 */ 
	void *pseudo_palette;		/* Fake palette of 16 colors */ 
#define FBINFO_STATE_RUNNING	0
#define FBINFO_STATE_SUSPENDED	1
	u32 state;			/* Hardware state i.e suspend */
	/* From here on everything is device dependent */
	void *par;
	/* we need the PCI or similar aperture base/size not
	   smem_start/size as smem_start may just be an object
	   allocated inside the aperture so may not actually overlap */
	struct apertures_struct {
		unsigned int count;
		struct aperture {
			resource_size_t base;
			resource_size_t size;
		} ranges[0];
	} *apertures;

	struct fb_info fbio;
	device_t fb_bsddev;
} __aligned(sizeof(long));

static inline struct apertures_struct *alloc_apertures(unsigned int max_num) {
	struct apertures_struct *a = kzalloc(sizeof(struct apertures_struct)
			+ max_num * sizeof(struct aperture), GFP_KERNEL);
	if (!a)
		return NULL;
	a->count = max_num;
	return a;
}

#define	cfb_fillrect(x, y)	0
#define	cfb_copyarea(x, y)	0
#define	cfb_imageblit(x, y)	0

extern int linux_register_framebuffer(struct linux_fb_info *fb_info);
extern int linux_unregister_framebuffer(struct linux_fb_info *fb_info);
extern int remove_conflicting_framebuffers(struct apertures_struct *a,
				const char *name, bool primary);
extern int remove_conflicting_pci_framebuffers(struct pci_dev *pdev, const char *name);
struct linux_fb_info *framebuffer_alloc(size_t size, struct device *dev);
extern void framebuffer_release(struct linux_fb_info *info);
#define	fb_set_suspend(x, y)	0

static inline int
fb_alloc_cmap(struct fb_cmap *cmap, int len, int transp)
{

	return (0);
}
#define	fb_dealloc_cmap(x)	0

/* updated FreeBSD fb_info */
extern int fb_get_options(const char *name, char **option);

void vt_dummy_switchto(struct apertures_struct *a, const char *name);


#endif /* __LINUX_FB_H_ */
