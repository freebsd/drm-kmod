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

#include "drm_os_config.h"

#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/pci.h>

#define DRM_DEV_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)
#define DRM_DEV_UID	UID_ROOT
#define DRM_DEV_GID	GID_VIDEO

extern const char *fb_mode_option;
extern int drm_always_interruptible;

struct vt_kms_softc {
	struct drm_fb_helper    *fb_helper;
	struct task              fb_mode_task;
};

#define	DRM_IRQ_ARGS		int irq, void *arg

#define	KHZ2PICOS(a)	(1000000000UL/(a))

#define	DRM_HZ			hz
#define DRM_CURPROC		curthread
#define	DRM_SUSER(p)		(priv_check(p, PRIV_DRIVER) == 0)
#define	DRM_UDELAY(n)		udelay(n);

#define DRM_WAIT_ON( ret, queue, timeout, condition )		\
do {								\
	DECLARE_WAITQUEUE(entry, current);			\
	unsigned long end = ticks + (timeout);		\
	add_wait_queue(&(queue), &entry);			\
								\
	for (;;) {						\
		__set_current_state(TASK_INTERRUPTIBLE);	\
		if (condition)					\
			break;					\
		if (time_after_eq(ticks, end)) {		\
			ret = -EBUSY;				\
			break;					\
		}						\
		schedule_timeout((HZ/100 > 1) ? HZ/100 : 1);	\
		if (signal_pending(current)) {			\
			ret = -EINTR;				\
			break;					\
		}						\
	}							\
	__set_current_state(TASK_RUNNING);			\
	remove_wait_queue(&(queue), &entry);			\
} while (0)



#define	DRM_WRITE8(map, offset, val)					\
	*(volatile u_int8_t *)(((vm_offset_t)(map)->handle) +		\
	    (vm_offset_t)(offset)) = val
#define	DRM_WRITE16(map, offset, val)					\
	*(volatile u_int16_t *)(((vm_offset_t)(map)->handle) +		\
	    (vm_offset_t)(offset)) = htole16(val)
#define	DRM_WRITE32(map, offset, val)					\
	*(volatile u_int32_t *)(((vm_offset_t)(map)->handle) +		\
	    (vm_offset_t)(offset)) = htole32(val)
#define	DRM_WRITE64(map, offset, val)					\
	*(volatile u_int64_t *)(((vm_offset_t)(map)->handle) +		\
	    (vm_offset_t)(offset)) = htole64(val)

/* DRM_READMEMORYBARRIER() prevents reordering of reads.
 * DRM_WRITEMEMORYBARRIER() prevents reordering of writes.
 * DRM_MEMORYBARRIER() prevents reordering of reads and writes.
 */
#define	DRM_READMEMORYBARRIER()		rmb()
#define	DRM_WRITEMEMORYBARRIER()	wmb()
#define	DRM_MEMORYBARRIER()		mb()
#define	smp_mb__before_atomic_inc()	mb()
#define	smp_mb__after_atomic_inc()	mb()

#define	lower_32_bits(n)	((u32)(n))
#define	upper_32_bits(n)	((u32)(((n) >> 16) >> 16))

/* XXXKIB what is the right code for the FreeBSD ? */
/* kib@ used ENXIO here -- dumbbell@ */
#define	EREMOTEIO	EIO

#define	KTR_DRM		KTR_DEV
#define	KTR_DRM_REG	KTR_SPARE3

#define	DRM_AGP_KERN	struct agp_info
#define	DRM_AGP_MEM	void

#define	IS_ALIGNED(x, y)	(((x) & ((y) - 1)) == 0)
#define	get_unaligned(ptr)                                              \
	({ __typeof__(*(ptr)) __tmp;                                    \
	  memcpy(&__tmp, (ptr), sizeof(*(ptr))); __tmp; })

#if _BYTE_ORDER == _LITTLE_ENDIAN
/* Taken from linux/include/linux/unaligned/le_struct.h. */
struct __una_u32 { u32 x; } __packed;

static inline u32
__get_unaligned_cpu32(const void *p)
{
	const struct __una_u32 *ptr = (const struct __una_u32 *)p;

	return (ptr->x);
}

static inline u32
get_unaligned_le32(const void *p)
{

	return (__get_unaligned_cpu32((const u8 *)p));
}
#else
/* Taken from linux/include/linux/unaligned/le_byteshift.h. */
static inline u32
__get_unaligned_le32(const u8 *p)
{

	return (p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24);
}

static inline u32
get_unaligned_le32(const void *p)
{

	return (__get_unaligned_le32((const u8 *)p));
}
#endif

#define	page_to_phys(x) VM_PAGE_TO_PHYS(x)

#define	drm_get_device_from_kdev(_kdev)	(((struct drm_minor *)(_kdev)->si_drv1)->dev)

#define DRM_IOC_VOID		IOC_VOID
#define DRM_IOC_READ		IOC_OUT
#define DRM_IOC_WRITE		IOC_IN
#define DRM_IOC_READWRITE	IOC_INOUT
#define DRM_IOC(dir, group, nr, size) _IOC(dir, group, nr, size)

static inline int
__copy_to_user_inatomic(void __user *to, const void *from, unsigned n)
{

	return (copyout_nofault(from, to, n) != 0 ? n : 0);
}
#define	__copy_to_user_inatomic_nocache(to, from, n) \
    __copy_to_user_inatomic((to), (from), (n))

static inline unsigned long
__copy_from_user_inatomic(void *to, const void __user *from,
    unsigned long n)
{

	/*
	 * XXXKIB.  Equivalent Linux function is implemented using
	 * MOVNTI for aligned moves.  For unaligned head and tail,
	 * normal move is performed.  As such, it is not incorrect, if
	 * only somewhat slower, to use normal copyin.  All uses
	 * except shmem_pwrite_fast() have the destination mapped WC.
	 */
	return ((copyin_nofault(__DECONST(void *, from), to, n) != 0 ? n : 0));
}
#define	__copy_from_user_inatomic_nocache(to, from, n) \
    __copy_from_user_inatomic((to), (from), (n))


#define	sigemptyset(set)	SIGEMPTYSET(set)
#define	sigaddset(set, sig)	SIGADDSET(set, sig)

MALLOC_DECLARE(DRM_MEM_DMA);
MALLOC_DECLARE(DRM_MEM_DRIVER);
MALLOC_DECLARE(DRM_MEM_KMS);

extern devclass_t drm_devclass;

typedef struct drm_pci_id_list
{
	int vendor;
	int device;
	long driver_private;
	char *name;
} drm_pci_id_list_t;

struct drm_minor;
int drm_dev_alias(struct device *dev, struct drm_minor *minor, const char *minor_str);
void cancel_reset_debug_log(void);

#define	console_lock()
#define	console_unlock()
#define	console_trylock()	true

#define	PM_EVENT_SUSPEND	0x0002
#define	PM_EVENT_QUIESCE	0x0008
#define	PM_EVENT_PRETHAW	PM_EVENT_QUIESCE


#define drm_proc_cleanup(a, b)


/* Legacy VGA regions */
#define VGA_RSRC_NONE	       0x00
#define VGA_RSRC_LEGACY_IO     0x01
#define VGA_RSRC_LEGACY_MEM    0x02
#define VGA_RSRC_LEGACY_MASK   (VGA_RSRC_LEGACY_IO | VGA_RSRC_LEGACY_MEM)
/* Non-legacy access */
#define VGA_RSRC_NORMAL_IO     0x04
#define VGA_RSRC_NORMAL_MEM    0x08



struct linux_fb_info;
#if 0
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

#define pm_qos_add_request(a, b, c)
#define pm_qos_update_request(a, b)
#define pm_qos_remove_request(a)

#define pci_get_power_state pci_get_powerstate
/* XXX ?? */
#define pci_disable_device(dev)

#define in_dbg_master() (kdb_active)
#define do_gettimeofday microtime
#define acpi_video_register()
#define acpi_video_unregister()

#define CONFIG_X86_PAT
extern	u_int	cpu_clflush_line_size;


#endif /* _DRM_OS_FREEBSD_H_ */
