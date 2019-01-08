/*
 * Internal Header for the Direct Rendering Manager
 *
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * Copyright (c) 2009-2010, Code Aurora Forum.
 * All rights reserved.
 *
 * Author: Rickard E. (Rik) Faith <faith@valinux.com>
 * Author: Gareth Hughes <gareth@valinux.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _DRM_P_H_
#define _DRM_P_H_

#if defined(_KERNEL) || defined(__KERNEL__)
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/module.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/sglist.h>
#include <sys/stat.h>
#include <sys/priv.h>
#include <sys/proc.h>
#include <sys/lock.h>
#include <sys/fcntl.h>
#include <sys/uio.h>
#include <sys/filio.h>
#include <sys/rwlock.h>
#include <sys/selinfo.h>
#include <sys/sysctl.h>
#include <sys/bus.h>
#include <sys/queue.h>
#include <sys/signalvar.h>
#include <sys/pciio.h>
#include <sys/poll.h>
#include <sys/sbuf.h>
#include <sys/taskqueue.h>
#include <sys/resourcevar.h>
#include <sys/vmmeter.h>
#include <vm/vm.h>
#include <vm/pmap.h>
#include <vm/vm_extern.h>
#include <vm/vm_kern.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vm_page.h>
#include <vm/vm_pageout.h>
#include <vm/vm_pager.h>
#include <vm/vm_param.h>
#include <vm/vm_phys.h>
#include <machine/bus.h>
#include <machine/resource.h>
#if defined(__i386__) || defined(__amd64__)
#include <machine/specialreg.h>
#endif
#include <machine/sysarch.h>
#include <sys/endian.h>
#include <sys/mman.h>
#include <sys/rman.h>
#include <sys/memrange.h>
#include <dev/agp/agpvar.h>
#include <sys/agpio.h>
#include <sys/mutex.h>
#include <dev/pci/pcivar.h>
#include <dev/pci/pcireg.h>
#include <sys/selinfo.h>
#include <sys/bus.h>

#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/idr.h>
#include <linux/string.h>
#include <linux/compat.h>
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/pm.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/poll.h>
#include <linux/ratelimit.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/ioctl.h>
#include <linux/workqueue.h>
#include <linux/math64.h>
#include <linux/dma-fence.h>
#include <linux/module.h>
#include <linux/refcount.h>
#include <linux/sizes.h>

#include <drm/drm_hashtab.h>
#ifdef __linux__
#include <asm/mman.h>
#include <asm/pgalloc.h>
#endif
#include <linux/uaccess.h>

#include <uapi/drm/drm.h>



#include <linux/device.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/pagemap.h>


#include <asm/mtrr.h>
#include <drm/drm_os_freebsd.h>
#include <drm/drm_agpsupport.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_hashtab.h>
#include <drm/drm_mm.h>
#include <drm/drm_ioctl.h>
#include <uapi/drm/drm_sarea.h>
#include <drm/drm_vma_manager.h>
#include <linux/atomic.h>
#include <asm/processor.h>

#include <drm/drm_drv.h>
#include <drm/drm_prime.h>
#include <drm/drm_print.h>
#include <drm/drm_pci.h>
#include <drm/drm_file.h>
#include <drm/drm_debugfs.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_sysfs.h>
#include <drm/drm_vblank.h>
#include <drm/drm_irq.h>
#include <drm/drm_device.h>

#include "opt_compat.h"
#include "opt_drm.h"
#include "opt_syscons.h"

#define __OS_HAS_AGP (defined(CONFIG_AGP) || (defined(CONFIG_AGP_MODULE) && defined(MODULE)))
#define __OS_HAS_MTRR (defined(CONFIG_MTRR))

#undef DRM_LINUX
#define DRM_LINUX 0

extern int drm_skipwc;

struct device_node;
struct videomode;
struct reservation_object;
struct seq_file;
struct dma_buf_attachment;

struct pci_dev;
struct pci_controller;

/* returns true if currently okay to sleep */
static inline bool drm_can_sleep(void)
{
	if (in_atomic() || in_dbg_master() || irqs_disabled() || cold)
		return false;
	return true;
}

#ifndef __linux__
/* BSDFIXME: Confirm is this for freebsd only? */
/* sysctl support (drm_sysctl.h) */
extern int		drm_sysctl_init(struct drm_device *dev);
extern int		drm_sysctl_cleanup(struct drm_device *dev);

/* helper for handling conditionals in various for_each macros */
#define for_each_if(condition) if (!(condition)) {} else

#ifdef ENABLE_DRM_ERR_RET
#define DRM_ERR_RET(V) do {						\
	printf("%s:%d ret %d\n", __FUNCTION__, __LINE__, V);		\
	return V;							\
} while (0)
#else
#define DRM_ERR_RET(V) return V
#endif

#endif
#endif				/* __KERNEL__ */

#if defined(CONFIG_DRM_DEBUG_SELFTEST_MODULE)
#define EXPORT_SYMBOL_FOR_TESTS_ONLY(x) EXPORT_SYMBOL(x)
#else
#define EXPORT_SYMBOL_FOR_TESTS_ONLY(x)
#endif

#endif
