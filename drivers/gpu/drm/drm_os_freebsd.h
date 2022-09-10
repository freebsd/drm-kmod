/**
 * \file drm_os_freebsd.h
 * OS abstraction macros.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#ifndef _DRM_OS_FREEBSD_H_
#define	_DRM_OS_FREEBSD_H_

#include <sys/fbio.h>
#include <sys/priv.h>
#include <sys/smp.h>

#include <linux/mod_devicetable.h>
#include <linux/fb.h>

#define DRM_DEV_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)
#define DRM_DEV_UID	UID_ROOT
#define DRM_DEV_GID	GID_VIDEO

struct vt_kms_softc {
	struct drm_fb_helper    *fb_helper;
	struct task              fb_mode_task;
};

/* XXXKIB what is the right code for the FreeBSD ? */
/* kib@ used ENXIO here -- dumbbell@ */
#define	EREMOTEIO	EIO

#define	KTR_DRM		KTR_DEV
#define	KTR_DRM_REG	KTR_SPARE3

#define	DRM_AGP_KERN	struct agp_info
#define	DRM_AGP_MEM	void

MALLOC_DECLARE(DRM_MEM_DMA);
MALLOC_DECLARE(DRM_MEM_DRIVER);
MALLOC_DECLARE(DRM_MEM_KMS);

extern devclass_t drm_devclass;

struct drm_minor;
int drm_dev_alias(struct device *dev, struct drm_minor *minor, const char *minor_str);
void cancel_reset_debug_log(void);

void vt_restore_fbdev_mode(void *arg, int pending);
int vt_kms_postswitch(void *arg);

#if 0
struct linux_fb_info;
static inline void vga_switcheroo_unregister_client(struct pci_dev *pdev) {}
static inline int vga_switcheroo_register_client(struct pci_dev *pdev,
		const struct vga_switcheroo_client_ops *ops) { return 0; }
static inline void vga_switcheroo_client_fb_set(struct pci_dev *pdev, struct linux_fb_info *info) {}
static inline int vga_switcheroo_register_handler(struct vga_switcheroo_handler *handler) { return 0; }
static inline int vga_switcheroo_register_audio_client(struct pci_dev *pdev,
	const struct vga_switcheroo_client_ops *ops,
	int id, bool active) { return 0; }
static inline void vga_switcheroo_unregister_handler(void) {}
static inline int vga_switcheroo_process_delayed_switch(void) { return 0; }
static inline int vga_switcheroo_get_client_state(struct pci_dev *pdev) { return VGA_SWITCHEROO_ON; }
#endif

#endif /* _DRM_OS_FREEBSD_H_ */
