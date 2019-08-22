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
#define FB_TYPE_PLANES			1	/* Non interleaved planes */
#define FB_TYPE_INTERLEAVED_PLANES	2	/* Interleaved planes	*/
#define FB_TYPE_TEXT			3	/* Text/attributes	*/
#define FB_TYPE_VGA_PLANES		4	/* EGA/VGA planes	*/
#define FB_TYPE_FOURCC			5	/* Type identified by a V4L2 FOURCC */

#define FB_AUX_TEXT_MDA		0	/* Monochrome text */
#define FB_AUX_TEXT_CGA		1	/* CGA/EGA/VGA Color text */
#define FB_AUX_TEXT_S3_MMIO	2	/* S3 MMIO fasttext */
#define FB_AUX_TEXT_MGA_STEP16	3	/* MGA Millenium I: text, attr, 14 reserved bytes */
#define FB_AUX_TEXT_MGA_STEP8	4	/* other MGAs:      text, attr,  6 reserved bytes */
#define FB_AUX_TEXT_SVGA_GROUP	8	/* 8-15: SVGA tileblit compatible modes */
#define FB_AUX_TEXT_SVGA_MASK	7	/* lower three bits says step */
#define FB_AUX_TEXT_SVGA_STEP2	8	/* SVGA text mode:  text, attr */
#define FB_AUX_TEXT_SVGA_STEP4	9	/* SVGA text mode:  text, attr,  2 reserved bytes */
#define FB_AUX_TEXT_SVGA_STEP8	10	/* SVGA text mode:  text, attr,  6 reserved bytes */
#define FB_AUX_TEXT_SVGA_STEP16	11	/* SVGA text mode:  text, attr, 14 reserved bytes */
#define FB_AUX_TEXT_SVGA_LAST	15	/* reserved up to 15 */

#define FB_AUX_VGA_PLANES_VGA4		0	/* 16 color planes (EGA/VGA) */
#define FB_AUX_VGA_PLANES_CFB4		1	/* CFB4 in planes (VGA) */
#define FB_AUX_VGA_PLANES_CFB8		2	/* CFB8 in planes (VGA) */

#define FB_VISUAL_MONO01		0	/* Monochr. 1=Black 0=White */
#define FB_VISUAL_MONO10		1	/* Monochr. 1=White 0=Black */
#define FB_VISUAL_TRUECOLOR		2	/* True color	*/
#define FB_VISUAL_PSEUDOCOLOR		3	/* Pseudo color (like atari) */
#define FB_VISUAL_DIRECTCOLOR		4	/* Direct color */
#define FB_VISUAL_STATIC_PSEUDOCOLOR	5	/* Pseudo color readonly */
#define FB_VISUAL_FOURCC		6	/* Visual identified by a V4L2 FOURCC */


#define FB_ACCEL_NONE		0	/* no hardware accelerator	*/

#define FB_ACTIVATE_NOW		0	/* set values immediately (or vbl)*/
#define FB_ACTIVATE_NXTOPEN	1	/* activate on next open	*/
#define FB_ACTIVATE_TEST	2	/* don't set, round up impossible */
#define FB_ACTIVATE_MASK       15
					/* values			*/
#define FB_ACTIVATE_VBL	       16	/* activate values on next vbl  */
#define FB_CHANGE_CMAP_VBL     32	/* change colormap on vbl	*/
#define FB_ACTIVATE_ALL	       64	/* change all VCs on this fb	*/
#define FB_ACTIVATE_FORCE     128	/* force apply even when no change*/
#define FB_ACTIVATE_INV_MODE  256       /* invalidate videomode */

#define FB_ACCELF_TEXT		1	/* (OBSOLETE) see fb_info.flags and vc_mode */

#define FB_MODE_IS_UNKNOWN	0
#define FB_MODE_IS_DETAILED	1
#define FB_MODE_IS_STANDARD	2
#define FB_MODE_IS_VESA		4
#define FB_MODE_IS_CALCULATED	8
#define FB_MODE_IS_FIRST	16
#define FB_MODE_IS_FROM_VAR     32

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
extern const struct fb_videomode vesa_modes[];
extern const struct fb_videomode cea_modes[64];

struct fb_modelist {
	struct list_head list;
	struct fb_videomode mode;
};

/* Definitions below are used in the parsed monitor specs */
#define FB_DPMS_ACTIVE_OFF	1
#define FB_DPMS_SUSPEND		2
#define FB_DPMS_STANDBY		4

#define FB_DISP_DDI		1
#define FB_DISP_ANA_700_300	2
#define FB_DISP_ANA_714_286	4
#define FB_DISP_ANA_1000_400	8
#define FB_DISP_ANA_700_000	16

#define FB_DISP_MONO		32
#define FB_DISP_RGB		64
#define FB_DISP_MULTI		128
#define FB_DISP_UNKNOWN		256

#define FB_SIGNAL_NONE		0
#define FB_SIGNAL_BLANK_BLANK	1
#define FB_SIGNAL_SEPARATE	2
#define FB_SIGNAL_COMPOSITE	4
#define FB_SIGNAL_SYNC_ON_GREEN	8
#define FB_SIGNAL_SERRATION_ON	16

#define FB_MISC_PRIM_COLOR	1
#define FB_MISC_1ST_DETAIL	2	/* First Detailed Timing is preferred */
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


#define FB_VMODE_NONINTERLACED  0	/* non interlaced */
#define FB_VMODE_INTERLACED	1	/* interlaced	*/
#define FB_VMODE_DOUBLE		2	/* double scan */
#define FB_VMODE_ODD_FLD_FIRST	4	/* interlaced: top line first */
#define FB_VMODE_MASK		255

#define FB_VMODE_YWRAP		256	/* ywrap instead of panning     */
#define FB_VMODE_SMOOTH_XPAN	512	/* smooth xpan possible (internally used) */
#define FB_VMODE_CONUPDATE	512	/* don't update x/yoffset	*/

#define PICOS2KHZ(a) (1000000000UL/(a))
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


#define FB_NONSTD_HAM		1	/* Hold-And-Modify (HAM)        */
#define FB_NONSTD_REV_PIX_IN_B	2	/* order of pixels in each byte is reversed */

#define FB_ACTIVATE_NOW		0	/* set values immediately (or vbl)*/
#define FB_ACTIVATE_NXTOPEN	1	/* activate on next open	*/
#define FB_ACTIVATE_TEST	2	/* don't set, round up impossible */
#define FB_ACTIVATE_MASK       15
					/* values			*/
#define FB_ACTIVATE_VBL	       16	/* activate values on next vbl  */
#define FB_CHANGE_CMAP_VBL     32	/* change colormap on vbl	*/
#define FB_ACTIVATE_ALL	       64	/* change all VCs on this fb	*/
#define FB_ACTIVATE_FORCE     128	/* force apply even when no change*/
#define FB_ACTIVATE_INV_MODE  256       /* invalidate videomode */


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

#define FB_VBLANK_VBLANKING	0x001	/* currently in a vertical blank */
#define FB_VBLANK_HBLANKING	0x002	/* currently in a horizontal blank */
#define FB_VBLANK_HAVE_VBLANK	0x004	/* vertical blanks can be detected */
#define FB_VBLANK_HAVE_HBLANK	0x008	/* horizontal blanks can be detected */
#define FB_VBLANK_HAVE_COUNT	0x010	/* global retrace counter is available */
#define FB_VBLANK_HAVE_VCOUNT	0x020	/* the vcount field is valid */
#define FB_VBLANK_HAVE_HCOUNT	0x040	/* the hcount field is valid */
#define FB_VBLANK_VSYNCING	0x080	/* currently in a vsync */
#define FB_VBLANK_HAVE_VSYNC	0x100	/* verical syncs can be detected */

struct fb_vblank {
	__u32 flags;			/* FB_VBLANK flags */
	__u32 count;			/* counter of retraces since boot */
	__u32 vcount;			/* current scanline position */
	__u32 hcount;			/* current scandot position */
	__u32 reserved[4];		/* reserved for future compatibility */
};

/*
 * Register/unregister for framebuffer events
 */

/*	The resolution of the passed in fb_info about to change */ 
#define FB_EVENT_MODE_CHANGE		0x01
/*	The display on this fb_info is beeing suspended, no access to the
 *	framebuffer is allowed any more after that call returns
 */
#define FB_EVENT_SUSPEND		0x02
/*	The display on this fb_info was resumed, you can restore the display
 *	if you own it
 */
#define FB_EVENT_RESUME			0x03
/*      An entry from the modelist was removed */
#define FB_EVENT_MODE_DELETE            0x04
/*      A driver registered itself */
#define FB_EVENT_FB_REGISTERED          0x05
/*      A driver unregistered itself */
#define FB_EVENT_FB_UNREGISTERED        0x06
/*      CONSOLE-SPECIFIC: get console to framebuffer mapping */
#define FB_EVENT_GET_CONSOLE_MAP        0x07
/*      CONSOLE-SPECIFIC: set console to framebuffer mapping */
#define FB_EVENT_SET_CONSOLE_MAP        0x08
/*      A hardware display blank change occurred */
#define FB_EVENT_BLANK                  0x09
/*      Private modelist is to be replaced */
#define FB_EVENT_NEW_MODELIST           0x0A
/*	The resolution of the passed in fb_info about to change and
        all vc's should be changed         */
#define FB_EVENT_MODE_CHANGE_ALL	0x0B
/*	A software display blank change occurred */
#define FB_EVENT_CONBLANK               0x0C
/*      Get drawing requirements        */
#define FB_EVENT_GET_REQ                0x0D
/*      Unbind from the console if possible */
#define FB_EVENT_FB_UNBIND              0x0E
/*      CONSOLE-SPECIFIC: remap all consoles to new fb - for vga_switcheroo */
#define FB_EVENT_REMAP_ALL_CONSOLE      0x0F
/*      A hardware display blank early change occured */
#define FB_EARLY_EVENT_BLANK		0x10
/*      A hardware display blank revert early change occured */
#define FB_R_EARLY_EVENT_BLANK		0x11

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

#ifdef CONFIG_FB_NOTIFY
extern int fb_register_client(struct notifier_block *nb);
extern int fb_unregister_client(struct notifier_block *nb);
extern int fb_notifier_call_chain(unsigned long val, void *v);
#else
static inline int fb_register_client(struct notifier_block *nb)
{
	return 0;
};

static inline int fb_unregister_client(struct notifier_block *nb)
{
	return 0;
};

static inline int fb_notifier_call_chain(unsigned long val, void *v)
{
	return 0;
};
#endif


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

/* Internal HW accel */
#define ROP_COPY 0
#define ROP_XOR  1

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



/* FBINFO_* = fb_info.flags bit flags */
#define FBINFO_MODULE		0x0001	/* Low-level driver is a module */
#define FBINFO_HWACCEL_DISABLED	0x0002
	/* When FBINFO_HWACCEL_DISABLED is set:
	 *  Hardware acceleration is turned off.  Software implementations
	 *  of required functions (copyarea(), fillrect(), and imageblit())
	 *  takes over; acceleration engine should be in a quiescent state */

/* hints */
#define FBINFO_VIRTFB		0x0004 /* FB is System RAM, not device. */
#define FBINFO_PARTIAL_PAN_OK	0x0040 /* otw use pan only for double-buffering */
#define FBINFO_READS_FAST	0x0080 /* soft-copy faster than rendering */

/* hardware supported ops */
/*  semantics: when a bit is set, it indicates that the operation is
 *   accelerated by hardware.
 *  required functions will still work even if the bit is not set.
 *  optional functions may not even exist if the flag bit is not set.
 */
#define FBINFO_HWACCEL_NONE		0x0000
#define FBINFO_HWACCEL_COPYAREA		0x0100 /* required */
#define FBINFO_HWACCEL_FILLRECT		0x0200 /* required */
#define FBINFO_HWACCEL_IMAGEBLIT	0x0400 /* required */
#define FBINFO_HWACCEL_ROTATE		0x0800 /* optional */
#define FBINFO_HWACCEL_XPAN		0x1000 /* optional */
#define FBINFO_HWACCEL_YPAN		0x2000 /* optional */
#define FBINFO_HWACCEL_YWRAP		0x4000 /* optional */

#define FBINFO_MISC_USEREVENT          0x10000 /* event request
						  from userspace */
#define FBINFO_MISC_TILEBLITTING       0x20000 /* use tile blitting */

/* A driver may set this flag to indicate that it does want a set_par to be
 * called every time when fbcon_switch is executed. The advantage is that with
 * this flag set you can really be sure that set_par is always called before
 * any of the functions dependent on the correct hardware state or altering
 * that state, even if you are using some broken X releases. The disadvantage
 * is that it introduces unwanted delays to every console switch if set_par
 * is slow. It is a good idea to try this flag in the drivers initialization
 * code whenever there is a bug report related to switching between X and the
 * framebuffer console.
 */
#define FBINFO_MISC_ALWAYS_SETPAR   0x40000

/* where the fb is a firmware driver, and can be replaced with a proper one */
#define FBINFO_MISC_FIRMWARE        0x80000
/*
 * Host and GPU endianness differ.
 */
#define FBINFO_FOREIGN_ENDIAN	0x100000
/*
 * Big endian math. This is the same flags as above, but with different
 * meaning, it is set by the fb subsystem depending FOREIGN_ENDIAN flag
 * and host endianness. Drivers should not use this flag.
 */
#define FBINFO_BE_MATH  0x100000

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

	struct fb_ops *fbops;
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

#ifdef MODULE
#define FBINFO_DEFAULT	FBINFO_MODULE
#else
#define FBINFO_DEFAULT	0
#endif

// This will go away
#define FBINFO_FLAG_MODULE	FBINFO_MODULE
#define FBINFO_FLAG_DEFAULT	FBINFO_DEFAULT

extern int fb_set_var(struct linux_fb_info *info, struct fb_var_screeninfo *var); 
extern int fb_pan_display(struct linux_fb_info *info, struct fb_var_screeninfo *var); 
extern int fb_blank(struct linux_fb_info *info, int blank);
extern void cfb_fillrect(struct linux_fb_info *info, const struct fb_fillrect *rect); 
extern void cfb_copyarea(struct linux_fb_info *info, const struct fb_copyarea *area); 
extern void cfb_imageblit(struct linux_fb_info *info, const struct fb_image *image);
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

/* drivers/video/fbmem.c */
extern int linux_register_framebuffer(struct linux_fb_info *fb_info);
extern int linux_unregister_framebuffer(struct linux_fb_info *fb_info);
extern int remove_conflicting_framebuffers(struct apertures_struct *a,
				const char *name, bool primary);
extern int remove_conflicting_pci_framebuffers(struct pci_dev *pdev, const char *name);
struct linux_fb_info *framebuffer_alloc(size_t size, struct device *dev);
extern void framebuffer_release(struct linux_fb_info *info);


extern int unlink_framebuffer(struct linux_fb_info *fb_info);
extern int fb_prepare_logo(struct linux_fb_info *fb_info, int rotate);
extern int fb_show_logo(struct linux_fb_info *fb_info, int rotate);
extern char* fb_get_buffer_offset(struct linux_fb_info *info, struct fb_pixmap *buf, u32 size);
extern void fb_pad_unaligned_buffer(u8 *dst, u32 d_pitch, u8 *src, u32 idx,
				u32 height, u32 shift_high, u32 shift_low, u32 mod);
extern void fb_pad_aligned_buffer(u8 *dst, u32 d_pitch, u8 *src, u32 s_pitch, u32 height);
extern void fb_set_suspend(struct linux_fb_info *info, int state);
extern int fb_get_color_depth(struct fb_var_screeninfo *var,
			      struct fb_fix_screeninfo *fix);
extern int fb_new_modelist(struct linux_fb_info *info);


extern int fb_alloc_cmap(struct fb_cmap *cmap, int len, int transp);
extern void fb_dealloc_cmap(struct fb_cmap *cmap);

/* updated FreeBSD fb_info */
extern void drm_legacy_fb_init(struct linux_fb_info *fb_info);
extern int fb_get_options(const char *name, char **option);



/*
 * GPL licensed routines that need to be replaced:
 */

extern const struct fb_cmap * tainted_fb_default_cmap(int len);
extern void tainted_cfb_fillrect(struct linux_fb_info *p, const struct fb_fillrect *rect);
extern void tainted_cfb_copyarea(struct linux_fb_info *p, const struct fb_copyarea *area);
extern void tainted_cfb_imageblit(struct linux_fb_info *p, const struct fb_image *image);

#endif /* __LINUX_FB_H_ */
