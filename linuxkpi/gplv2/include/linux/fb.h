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

#define	FBINFO_DEFAULT		0
#define	FBINFO_VIRTFB		1
#define	FBINFO_READS_FAST	2

struct linux_fb_info;
struct videomode;
struct vm_area_struct;

struct fb_blit_caps {
	u32 x;
	u32 y;
	u32 len;
	u32 flags;
};

struct fb_ops {
	/* open/release and usage marking */
	struct module *owner;
	int (*fb_open)(struct linux_fb_info *info, int user);
	int (*fb_release)(struct linux_fb_info *info, int user);

	/* For framebuffers with strange non linear layouts or that do not
	 * work with normal memory mapped access
	 */
	ssize_t (*fb_read)(struct linux_fb_info *info, char __user *buf,
			   size_t count, loff_t *ppos);
	ssize_t (*fb_write)(struct linux_fb_info *info, const char __user *buf,
			    size_t count, loff_t *ppos);

	/* checks var and eventually tweaks it to something supported,
	 * DO NOT MODIFY PAR */
	int (*fb_check_var)(struct fb_var_screeninfo *var, struct linux_fb_info *info);

	/* set the video mode according to info->var */
	int (*fb_set_par)(struct linux_fb_info *info);

	/* set color register */
	int (*fb_setcolreg)(unsigned regno, unsigned red, unsigned green,
			    unsigned blue, unsigned transp, struct linux_fb_info *info);

	/* set color registers in batch */
	int (*fb_setcmap)(struct fb_cmap *cmap, struct linux_fb_info *info);

	/* blank display */
	int (*fb_blank)(int blank, struct linux_fb_info *info);

	/* pan display */
	int (*fb_pan_display)(struct fb_var_screeninfo *var, struct linux_fb_info *info);

	/* Draws a rectangle */
	void (*fb_fillrect) (struct linux_fb_info *info, const struct fb_fillrect *rect);
	/* Copy data from area to another */
	void (*fb_copyarea) (struct linux_fb_info *info, const struct fb_copyarea *region);
	/* Draws a image to the display */
	void (*fb_imageblit) (struct linux_fb_info *info, const struct fb_image *image);

	/* Draws cursor */
	int (*fb_cursor) (struct linux_fb_info *info, struct fb_cursor *cursor);

	/* wait for blit idle, optional */
	int (*fb_sync)(struct linux_fb_info *info);

	/* perform fb specific ioctl (optional) */
	int (*fb_ioctl)(struct linux_fb_info *info, unsigned int cmd,
			unsigned long arg);

	/* Handle 32bit compat ioctl (optional) */
	int (*fb_compat_ioctl)(struct linux_fb_info *info, unsigned cmd,
			unsigned long arg);

	/* perform fb specific mmap */
	int (*fb_mmap)(struct linux_fb_info *info, struct vm_area_struct *vma);

	/* get capability given var */
	void (*fb_get_caps)(struct linux_fb_info *info, struct fb_blit_caps *caps,
			    struct fb_var_screeninfo *var);

	/* teardown any resources to do with this framebuffer */
	void (*fb_destroy)(struct linux_fb_info *info);

	/* called at KDB enter and leave time to prepare the console */
	int (*fb_debug_enter)(struct linux_fb_info *info);
	int (*fb_debug_leave)(struct linux_fb_info *info);
};

/*
 * Hide smem_start in the FBIOGET_FSCREENINFO IOCTL. This is used by modern DRM
 * drivers to stop userspace from trying to share buffers behind the kernel's
 * back. Instead dma-buf based buffer sharing should be used.
 */
#define FBINFO_HIDE_SMEM_START  0x200000


struct linux_fb_info {
	int flags;
	/*
	 * -1 by default, set to a FB_ROTATE_* value by the driver, if it knows
	 * a lcd is not mounted upright and fbcon should rotate to compensate.
	 */
	int fbcon_rotate_hint;

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

	bool skip_vt_switch; /* no VT switch on suspend/resume required */

#ifdef __FreeBSD__
	struct fb_info fbio;
	device_t fb_bsddev;
	struct task fb_mode_task;
#endif
} __aligned(sizeof(long));

static inline struct apertures_struct *alloc_apertures(unsigned int max_num) {
	struct apertures_struct *a = kzalloc(sizeof(struct apertures_struct)
			+ max_num * sizeof(struct aperture), GFP_KERNEL);
	if (!a)
		return NULL;
	a->count = max_num;
	return a;
}

    /*
     *  `Generic' versions of the frame buffer device operations
     */

extern void cfb_fillrect(struct linux_fb_info *info, const struct fb_fillrect *rect);
extern void cfb_copyarea(struct linux_fb_info *info, const struct fb_copyarea *area);
extern void cfb_imageblit(struct linux_fb_info *info, const struct fb_image *image);
extern ssize_t fb_io_read(struct linux_fb_info *info, char __user *buf,
    size_t count, loff_t *ppos);
extern ssize_t fb_io_write(struct linux_fb_info *info, const char __user *buf,
    size_t count, loff_t *ppos);

/*
 * Initializes struct fb_ops for framebuffers in I/O memory.
 */

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

/*
 * Drawing operations where framebuffer is in system RAM
 */
extern void sys_fillrect(struct linux_fb_info *info, const struct fb_fillrect *rect);
extern void sys_copyarea(struct linux_fb_info *info, const struct fb_copyarea *area);
extern void sys_imageblit(struct linux_fb_info *info, const struct fb_image *image);
extern ssize_t fb_sys_read(struct linux_fb_info *info, char __user *buf,
			   size_t count, loff_t *ppos);
extern ssize_t fb_sys_write(struct linux_fb_info *info, const char __user *buf,
			    size_t count, loff_t *ppos);
extern int fb_deferred_io_mmap(struct linux_fb_info *info, struct vm_area_struct *vma);

/*
 * Generate callbacks for deferred I/O
 */

#define __FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, __mode) \
	static ssize_t __prefix ## _defio_read(struct fb_info *info, char __user *buf, \
					       size_t count, loff_t *ppos) \
	{ \
		return fb_ ## __mode ## _read(info, buf, count, ppos); \
	} \
	static ssize_t __prefix ## _defio_write(struct fb_info *info, const char __user *buf, \
						size_t count, loff_t *ppos) \
	{ \
		unsigned long offset = *ppos; \
		ssize_t ret = fb_ ## __mode ## _write(info, buf, count, ppos); \
		if (ret > 0) \
			__damage_range(info, offset, ret); \
		return ret; \
	}

#define __FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, __mode) \
	static void __prefix ## _defio_fillrect(struct fb_info *info, \
						const struct fb_fillrect *rect) \
	{ \
		__mode ## _fillrect(info, rect); \
		__damage_area(info, rect->dx, rect->dy, rect->width, rect->height); \
	} \
	static void __prefix ## _defio_copyarea(struct fb_info *info, \
						const struct fb_copyarea *area) \
	{ \
		__mode ## _copyarea(info, area); \
		__damage_area(info, area->dx, area->dy, area->width, area->height); \
	} \
	static void __prefix ## _defio_imageblit(struct fb_info *info, \
						 const struct fb_image *image) \
	{ \
		__mode ## _imageblit(info, image); \
		__damage_area(info, image->dx, image->dy, image->width, image->height); \
	}

#define FB_GEN_DEFAULT_DEFERRED_IOMEM_OPS(__prefix, __damage_range, __damage_area) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, io) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, cfb)

#define FB_GEN_DEFAULT_DEFERRED_SYSMEM_OPS(__prefix, __damage_range, __damage_area) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, sys) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, sys)

/*
 * Initializes struct fb_ops for deferred I/O.
 */

#define __FB_DEFAULT_DEFERRED_OPS_RDWR(__prefix) \
	.fb_read	= __prefix ## _defio_read, \
	.fb_write	= __prefix ## _defio_write

#define __FB_DEFAULT_DEFERRED_OPS_DRAW(__prefix) \
	.fb_fillrect	= __prefix ## _defio_fillrect, \
	.fb_copyarea	= __prefix ## _defio_copyarea, \
	.fb_imageblit	= __prefix ## _defio_imageblit

#define __FB_DEFAULT_DEFERRED_OPS_MMAP(__prefix) \
	.fb_mmap	= fb_deferred_io_mmap

#define FB_DEFAULT_DEFERRED_OPS(__prefix) \
        __FB_DEFAULT_DEFERRED_OPS_RDWR(__prefix), \
        __FB_DEFAULT_DEFERRED_OPS_DRAW(__prefix), \
        __FB_DEFAULT_DEFERRED_OPS_MMAP(__prefix)


int linux_register_framebuffer(struct linux_fb_info *fb_info);
int linux_unregister_framebuffer(struct linux_fb_info *fb_info);
int remove_conflicting_framebuffers(struct apertures_struct *a,
	const char *name, bool primary);
int remove_conflicting_pci_framebuffers(struct pci_dev *pdev, const char *name);
struct linux_fb_info *framebuffer_alloc(size_t size, struct device *dev);
void framebuffer_release(struct linux_fb_info *info);
#define	fb_set_suspend(x, y)	0

static inline bool
is_firmware_framebuffer(struct apertures_struct *a __unused)
{
	return false;
}

/* updated FreeBSD fb_info */
int linux_fb_get_options(const char *name, char **option);
#define	fb_get_options	linux_fb_get_options

#endif /* __LINUX_FB_H_ */
