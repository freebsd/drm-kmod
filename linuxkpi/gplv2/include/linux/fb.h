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

#define FB_TYPE_PACKED_PIXELS		0	/* Packed Pixels	*/

#define FB_VISUAL_MONO01		0	/* Monochr. 1=Black 0=White */
#define FB_VISUAL_MONO10		1	/* Monochr. 1=White 0=Black */
#define FB_VISUAL_TRUECOLOR		2	/* True color	*/
#define FB_VISUAL_PSEUDOCOLOR		3	/* Pseudo color (like atari) */
#define FB_VISUAL_DIRECTCOLOR		4	/* Direct color */
#define FB_VISUAL_STATIC_PSEUDOCOLOR	5	/* Pseudo color readonly */
#define FB_VISUAL_FOURCC		6	/* Visual identified by a V4L2 FOURCC */

#define FB_ACCEL_NONE		0	/* no hardware accelerator	*/

#define FB_ACTIVATE_NOW		0	/* set values immediately (or vbl)*/

#define FB_ACCELF_TEXT		1	/* (OBSOLETE) see fb_info.flags and vc_mode */

struct fb_videomode {
	const char *name;	/* optional */
	u32 refresh;		/* optional */
	u32 xres;
	u32 yres;
	u32 pixclock;
	u32 left_margin;
	u32 right_margin;
	u32 upper_margin;
	u32 lower_margin;
	u32 hsync_len;
	u32 vsync_len;
	u32 sync;
	u32 vmode;
	u32 flag;
};

extern const char *fb_mode_option;

struct fb_chroma {
	__u32 redx;	/* in fraction of 1024 */
	__u32 greenx;
	__u32 bluex;
	__u32 whitex;
	__u32 redy;
	__u32 greeny;
	__u32 bluey;
	__u32 whitey;
};

struct fb_monspecs {
	struct fb_chroma chroma;
	struct fb_videomode *modedb;	/* mode database */
	__u8  manufacturer[4];		/* Manufacturer */
	__u8  monitor[14];		/* Monitor String */
	__u8  serial_no[14];		/* Serial Number */
	__u8  ascii[14];		/* ? */
	__u32 modedb_len;		/* mode database length */
	__u32 model;			/* Monitor Model */
	__u32 serial;			/* Serial Number - Integer */
	__u32 year;			/* Year manufactured */
	__u32 week;			/* Week Manufactured */
	__u32 hfmin;			/* hfreq lower limit (Hz) */
	__u32 hfmax;			/* hfreq upper limit (Hz) */
	__u32 dclkmin;			/* pixelclock lower limit (Hz) */
	__u32 dclkmax;			/* pixelclock upper limit (Hz) */
	__u16 input;			/* display type - see FB_DISP_* */
	__u16 dpms;			/* DPMS support - see FB_DPMS_ */
	__u16 signal;			/* Signal Type - see FB_SIGNAL_* */
	__u16 vfmin;			/* vfreq lower limit (Hz) */
	__u16 vfmax;			/* vfreq upper limit (Hz) */
	__u16 gamma;			/* Gamma - in fractions of 100 */
	__u16 gtf	: 1;		/* supports GTF */
	__u16 misc;			/* Misc flags - see FB_MISC_* */
	__u8  version;			/* EDID version... */
	__u8  revision;			/* ...and revision */
	__u8  max_x;			/* Maximum horizontal size (cm) */
	__u8  max_y;			/* Maximum vertical size (cm) */
};

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

struct fb_bitfield {
	__u32 offset;			/* beginning of bitfield	*/
	__u32 length;			/* length of bitfield		*/
	__u32 msb_right;		/* != 0 : Most significant bit is */ 
					/* right */ 
};

#define FB_ACTIVATE_NOW		0	/* set values immediately (or vbl)*/

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
	struct fb_bitfield red;		/* bitfield in fb mem if true color, */
	struct fb_bitfield green;	/* else only length is significant */
	struct fb_bitfield blue;
	struct fb_bitfield transp;	/* transparency			*/	

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


/* VESA Blanking Levels */
#define VESA_NO_BLANKING        0
#define VESA_VSYNC_SUSPEND      1
#define VESA_HSYNC_SUSPEND      2
#define VESA_POWERDOWN          3

enum {
	/* screen: unblanked, hsync: on,  vsync: on */
	FB_BLANK_UNBLANK       = VESA_NO_BLANKING,

	/* screen: blanked,   hsync: on,  vsync: on */
	FB_BLANK_NORMAL        = VESA_NO_BLANKING + 1,

	/* screen: blanked,   hsync: on,  vsync: off */
	FB_BLANK_VSYNC_SUSPEND = VESA_VSYNC_SUSPEND + 1,

	/* screen: blanked,   hsync: off, vsync: on */
	FB_BLANK_HSYNC_SUSPEND = VESA_HSYNC_SUSPEND + 1,

	/* screen: blanked,   hsync: off, vsync: off */
	FB_BLANK_POWERDOWN     = VESA_POWERDOWN + 1
};

struct fb_vblank {
	__u32 flags;			/* FB_VBLANK flags */
	__u32 count;			/* counter of retraces since boot */
	__u32 vcount;			/* current scanline position */
	__u32 hcount;			/* current scandot position */
	__u32 reserved[4];		/* reserved for future compatibility */
};

struct fb_event {
	struct linux_fb_info *info;
	void *data;
};

struct fb_blit_caps {
	u32 x;
	u32 y;
	u32 len;
	u32 flags;
};

/*
 * Pixmap structure definition
 *
 * The purpose of this structure is to translate data
 * from the hardware independent format of fbdev to what
 * format the hardware needs.
 */

#define FB_PIXMAP_DEFAULT 1     /* used internally by fbcon */
#define FB_PIXMAP_SYSTEM  2     /* memory is in system RAM  */
#define FB_PIXMAP_IO      4     /* memory is iomapped       */
#define FB_PIXMAP_SYNC    256   /* set if GPU can DMA       */

struct fb_pixmap {
	u8  *addr;		/* pointer to memory			*/
	u32 size;		/* size of buffer in bytes		*/
	u32 offset;		/* current offset to buffer		*/
	u32 buf_align;		/* byte alignment of each bitmap	*/
	u32 scan_align;		/* alignment per scanline		*/
	u32 access_align;	/* alignment per read/write (bits)	*/
	u32 flags;		/* see FB_PIXMAP_*			*/
	u32 blit_x;             /* supported bit block dimensions (1-32)*/
	u32 blit_y;             /* Format: blit_x = 1 << (width - 1)    */
	                        /*         blit_y = 1 << (height - 1)   */
	                        /* if 0, will be set to 0xffffffff (all)*/
	/* access methods */
	void (*writeio)(struct linux_fb_info *info, void __iomem *dst, void *src, unsigned int size);
	void (*readio) (struct linux_fb_info *info, void *dst, void __iomem *src, unsigned int size);
};

struct fb_copyarea {
	__u32 dx;
	__u32 dy;
	__u32 width;
	__u32 height;
	__u32 sx;
	__u32 sy;
};

struct fb_fillrect {
	__u32 dx;	/* screen-relative */
	__u32 dy;
	__u32 width;
	__u32 height;
	__u32 color;
	__u32 rop;
};

struct fb_image {
	__u32 dx;		/* Where to place image */
	__u32 dy;
	__u32 width;		/* Size of image */
	__u32 height;
	__u32 fg_color;		/* Only used when a mono bitmap */
	__u32 bg_color;
	__u8  depth;		/* Depth of the image */
	const char *data;	/* Pointer to image data */
	struct fb_cmap cmap;	/* color map info */
};

struct lfbcurpos {
	__u16 x, y;
};

struct fb_cursor {
	__u16 set;		/* what to set */
	__u16 enable;		/* cursor on/off */
	__u16 rop;		/* bitop operation */
	const char *mask;	/* cursor mask bits */
	struct lfbcurpos hot;	/* cursor hot spot */
	struct fb_image	image;	/* Cursor image */
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

	/* Rotates the display */
	void (*fb_rotate)(struct linux_fb_info *info, int angle);

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


/* report to the VT layer that this fb driver can accept forced console
   output like oopses */
#define	FBINFO_HIDE_SMEM_START  0x200000

struct linux_fb_info {
	atomic_t count;
	int node;
	int flags;
	/*
	 * -1 by default, set to a FB_ROTATE_* value by the driver, if it knows
	 * a lcd is not mounted upright and fbcon should rotate to compensate.
	 */
	int fbcon_rotate_hint;
	struct mutex lock;		/* Lock for open/release/ioctl funcs */
	struct mutex mm_lock;		/* Lock for fb_mmap and smem_* fields */
	struct fb_var_screeninfo var;	/* Current var */
	struct fb_fix_screeninfo fix;	/* Current fix */
	struct fb_monspecs monspecs;	/* Current Monitor specs */
	struct work_struct queue;	/* Framebuffer event queue */
	struct fb_pixmap pixmap;	/* Image hardware mapper */
	struct fb_pixmap sprite;	/* Cursor hardware mapper */
	struct fb_cmap cmap;		/* Current cmap */
	struct list_head modelist;      /* mode list */
	struct fb_videomode *mode;	/* current mode */

#ifdef CONFIG_FB_BACKLIGHT
	/* assigned backlight device */
	/* set before framebuffer registration, 
	   remove after unregister */
	struct backlight_device *bl_dev;

	/* Backlight level curve */
	struct mutex bl_curve_mutex;	
	u8 bl_curve[FB_BACKLIGHT_LEVELS];
#endif

#ifdef __linux__
	/* Not used on FreeBSD */
	struct delayed_work deferred_work;
	struct fb_deferred_io *fbdefio;
#endif

	const struct fb_ops *fbops;
	struct device *device;		/* This is the parent */
	struct device *dev;		/* This is this fb device */
	int class_flag;                    /* private sysfs flags */
#ifdef CONFIG_FB_TILEBLITTING
	struct fb_tile_ops *tileops;    /* Tile Blitting */
#endif
	union {
		char __iomem *screen_base;	/* Virtual address */
		char *screen_buffer;
	};
	unsigned long screen_size;	/* Amount of ioremapped VRAM or 0 */ 
	void *pseudo_palette;		/* Fake palette of 16 colors */ 
#define FBINFO_STATE_RUNNING	0
#define FBINFO_STATE_SUSPENDED	1
	u32 state;			/* Hardware state i.e suspend */
	void *fbcon_par;                /* fbcon use-only private area */
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

	struct fb_info fbio;
	struct cdev *fb_cdev;
	device_t fb_bsddev;
#ifdef __linux__
};
#else
} __aligned(sizeof(long));
#endif

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
extern int unlink_framebuffer(struct linux_fb_info *fb_info);
#define	fb_set_suspend(x, y)	0

static inline int
fb_alloc_cmap(struct fb_cmap *cmap, int len, int transp)
{

	return (0);
}
#define	fb_dealloc_cmap(x)	0

/* updated FreeBSD fb_info */
extern int fb_get_options(const char *name, char **option);

#endif /* __LINUX_FB_H_ */
