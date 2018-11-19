#ifndef _VMWGFX_FB_FREEBSD_H_
#define _VMWGFX_FB_FREEBSD_H_

struct linux_fb_info *vmw_framebuffer_alloc(size_t size, struct device *dev);
int vmw_register_framebuffer(struct linux_fb_info *fb_info);
int vmw_unregister_framebuffer(struct linux_fb_info *fb_info);

int vt_vmwfb_attach(struct fb_info *info);
void vt_vmwfb_resume(struct vt_device *vd);
void vt_vmwfb_suspend(struct vt_device *vd);
int vt_vmwfb_detach(struct fb_info *info);

vd_init_t		vt_vmwfb_init;
vd_fini_t		vt_vmwfb_fini;
vd_blank_t		vt_vmwfb_blank;
vd_bitblt_text_t	vt_vmwfb_bitblt_text;
vd_invalidate_text_t	vt_vmwfb_invalidate_text;
vd_bitblt_bmp_t		vt_vmwfb_bitblt_bitmap;
vd_drawrect_t		vt_vmwfb_drawrect;
vd_setpixel_t		vt_vmwfb_setpixel;
vd_postswitch_t		vt_vmwfb_postswitch;
vd_fb_ioctl_t		vt_vmwfb_ioctl;
vd_fb_mmap_t		vt_vmwfb_mmap;


#endif
