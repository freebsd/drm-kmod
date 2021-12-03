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

#define KHZ2PICOS(a) (1000000000UL/(a))

struct fb_fix_screeninfo {
	vm_paddr_t	smem_start;
	uint32_t	smem_len;
	uint32_t	line_length;
};

struct fb_var_screeninfo {
	int	xres;
	int	yres;
	int	bits_per_pixel;
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

/* updated FreeBSD fb_info */
extern int fb_get_options(const char *name, char **option);

void vt_dummy_switchto(struct apertures_struct *a, const char *name);


#endif /* __LINUX_FB_H_ */
