#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <linux/device.h>
#include <linux/acpi.h>
#include <drm/drmP.h>
#include <drm/i915_drm.h>
#include "i915_trace.h"
#include <linux/mm.h>
#include <linux/io-mapping.h>

#include <asm/pgtable.h>

#include "i915_drv.h"
#include <linux/console.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/vgaarb.h>
#include <linux/vga_switcheroo.h>
#include <drm/drm_crtc_helper.h>

void *intel_gtt_get_registers(void);
void _intel_gtt_get(size_t *gtt_total, size_t *stolen_size, unsigned long *mappable_end);
void _intel_gtt_install_pte(unsigned int index, vm_paddr_t addr, unsigned int flags);
uint32_t intel_gtt_read_pte(unsigned int entry);

#define AGP_I810_PGTBL_CTL	0x2020
#define	AGP_I810_PGTBL_ENABLED	0x00000001
#define INTEL_GTT_GEN	intel_private.gen
#define GFX_FLSH_CNTL_BSD	0x2170 /* 915+ only */
#define HAS_PGTBL_EN		1
#define I915_GMADR_BAR	2

#define WARN_UN() log(LOG_WARNING, "%s unimplemented", __FUNCTION__)
static struct _intel_private {
	struct pci_dev *bridge_dev;
	u8 __iomem *registers;
	u32 PGTBL_save;
	int gen;
	phys_addr_t gma_bus_addr;
} intel_private;

#ifdef __notyet__
static struct _intel_private {
	const struct intel_gtt_driver *driver;
	struct pci_dev *pcidev;	/* device one */
	struct pci_dev *bridge_dev;
	u8 __iomem *registers;
	phys_addr_t gtt_bus_addr;
	u32 PGETBL_save;
	u32 __iomem *gtt;		/* I915G */
	bool clear_fake_agp; /* on first access via agp, fill with scratch */
	int num_dcache_entries;
	void __iomem *i9xx_flush_page;
	char *i81x_gtt_table;
	struct resource ifp_resource;
	int resource_valid;
	struct page *scratch_page;
	phys_addr_t scratch_page_dma;
	int refcount;
	/* Whether i915 needs to use the dmar apis or not. */
	unsigned int needs_dmar : 1;
	phys_addr_t gma_bus_addr;
	/*  Size of memory reserved for graphics by the BIOS */
	unsigned int stolen_size;
	/* Total number of gtt entries. */
	unsigned int gtt_total_entries;
	/* Part of the gtt that is mappable by the cpu, for those chips where
	 * this is not the full gtt. */
	unsigned int gtt_mappable_entries;
} intel_private;
#endif

void
intel_gtt_get(size_t *gtt_total, size_t *stolen_size,
		  bus_addr_t *mappable_base, unsigned long *mappable_end)

{
	_intel_gtt_get(gtt_total, stolen_size, mappable_end);	
	*mappable_base = intel_private.gma_bus_addr;
	DRM_DEBUG("gtt_total %lx, stolen_size %lx, mappable_end %lx gma_bus_addr %lx\n",
		  *gtt_total, *stolen_size, *mappable_end, *mappable_base);
}

bool
intel_enable_gtt(void)
{
	u8 __iomem *reg;


	DRM_DEBUG("entering %s\n", __func__);
#ifdef __notyet__
	/* too old */
	if (INTEL_GTT_GEN == 2) {
		u16 gmch_ctrl;

		pci_read_config_word(intel_private.bridge_dev,
				     I830_GMCH_CTRL, &gmch_ctrl);
		gmch_ctrl |= I830_GMCH_ENABLED;
		pci_write_config_word(intel_private.bridge_dev,
				      I830_GMCH_CTRL, gmch_ctrl);

		pci_read_config_word(intel_private.bridge_dev,
				     I830_GMCH_CTRL, &gmch_ctrl);
		if ((gmch_ctrl & I830_GMCH_ENABLED) == 0) {
			dev_err(&intel_private.pcidev->dev,
				"failed to enable the GTT: GMCH_CTRL=%x\n",
				gmch_ctrl);
			return false;
		}
	}
#endif	

	/* On the resume path we may be adjusting the PGTBL value, so
	 * be paranoid and flush all chipset write buffers...
	 */
	if (INTEL_GTT_GEN >= 3)
		writel(0, intel_private.registers+ GFX_FLSH_CNTL_BSD);

	reg = intel_private.registers+AGP_I810_PGTBL_CTL;
	writel(intel_private.PGTBL_save, reg);
#ifdef __notyet	
	if (HAS_PGTBL_EN && (readl(reg) & AGP_I810_PGTBL_ENABLED) == 0) {
		dev_err(&intel_private.pcidev->dev,
			"failed to enable the GTT: PGTBL=%x [expected %x]\n",
			readl(reg), intel_private.PGTBL_save);
		return false;
	}
#endif	

	if (INTEL_GTT_GEN >= 3)
		writel(0, intel_private.registers+ GFX_FLSH_CNTL_BSD);
	DRM_DEBUG("exiting %s\n", __func__);
	return (1);
}

int
intel_gmch_probe(struct pci_dev *bridge_pdev, struct pci_dev *gpu_pdev,
		 struct agp_bridge_data *bridge)
{
	DRM_DEBUG("entering %s\n", __func__);
	intel_private.registers = intel_gtt_get_registers();
	intel_private.gma_bus_addr = pci_bus_address(gpu_pdev, I915_GMADR_BAR);
	DRM_DEBUG("bus_addr %lx\n", intel_private.gma_bus_addr);
	INTEL_GTT_GEN = 4;
	/* save the PGTBL reg for resume */
	intel_private.PGTBL_save =
		readl(intel_private.registers+AGP_I810_PGTBL_CTL)
			& ~AGP_I810_PGTBL_ENABLED;
	/* we only ever restore the register when enabling the PGTBL... */
	if (HAS_PGTBL_EN)
		intel_private.PGTBL_save |= AGP_I810_PGTBL_ENABLED;

	DRM_DEBUG("exiting %s\n", __func__);
	return (1);
}

void
intel_gmch_remove(void)
{

}

void
intel_gtt_insert_page(dma_addr_t addr, unsigned int pg, unsigned int flags)
{

	_intel_gtt_install_pte(pg, addr, flags);
	intel_gtt_chipset_flush();
}

void
intel_gtt_insert_sg_entries(struct sg_table *st, unsigned int pg_start,
    unsigned int flags)
{
	struct sg_page_iter sg_iter;
	unsigned int i;
	vm_paddr_t addr;

	i = 0;
	for_each_sg_page(st->sgl, &sg_iter, st->nents, 0) {
		addr = sg_page_iter_dma_address(&sg_iter);
		_intel_gtt_install_pte(pg_start + i, addr, flags);
		++i;
	}

	intel_gtt_read_pte(pg_start + i - 1);
}

void
i915_locks_destroy(struct drm_i915_private *dev_priv)
{
	spin_lock_destroy(&dev_priv->irq_lock);
	spin_lock_destroy(&dev_priv->gpu_error.lock);
	mutex_destroy(&dev_priv->backlight_lock);
	spin_lock_destroy(&dev_priv->uncore.lock);
	spin_lock_destroy(&dev_priv->mm.object_stat_lock);
	spin_lock_destroy(&dev_priv->mmio_flip_lock);
	mutex_destroy(&dev_priv->sb_lock);
	mutex_destroy(&dev_priv->modeset_restore_lock);
	mutex_destroy(&dev_priv->av_mutex);
}


/**
 * remap_io_mapping - remap an IO mapping to userspace
 * @vma: user vma to map to
 * @addr: target user address to start at
 * @pfn: physical address of kernel memory
 * @size: size of map area
 * @iomap: the source io_mapping
 *
 *  Note: this is only safe if the mm semaphore is held when called.
 */
int
remap_io_mapping(struct vm_area_struct *vma, unsigned long addr,
    unsigned long pfn, unsigned long size, struct io_mapping *iomap)
{
	vm_page_t m;
	vm_object_t vm_obj;
	vm_memattr_t attr;
	vm_paddr_t pa;
	vm_pindex_t pidx, pidx_start;
	int count, rc;

	attr = pgprot2cachemode(iomap->prot);
	count = size >> PAGE_SHIFT;
	pa = pfn << PAGE_SHIFT;
	pidx_start = OFF_TO_IDX(addr);
	rc = 0;
	vm_obj = vma->vm_obj;

	vma->vm_pfn_first = pidx_start;

	VM_OBJECT_WLOCK(vm_obj);
	for (pidx = pidx_start; pidx < pidx_start + count;
	    pidx++, pa += PAGE_SIZE) {
retry:
		m = vm_page_lookup(vm_obj, pidx);
		if (m != NULL) {
			/* XXX need to unpin object first */
			if (vm_page_sleep_if_busy(m, "i915flt"))
				goto retry;
		} else {
			m = PHYS_TO_VM_PAGE(pa);
			if (vm_page_busied(m)) {
				vm_page_lock(m);
				VM_OBJECT_WUNLOCK(vm_obj);
				vm_page_busy_sleep(m, "i915flt", false);
				VM_OBJECT_WLOCK(vm_obj);
				goto retry;
			}
			if (vm_page_insert(m, vm_obj, pidx)) {
				VM_OBJECT_WUNLOCK(vm_obj);
				VM_WAIT;
				VM_OBJECT_WLOCK(vm_obj);
				goto retry;
			}
			m->valid = VM_PAGE_BITS_ALL;
		}
		vm_page_xbusy(m);
		pmap_page_set_memattr(m, attr);
		vma->vm_pfn_count++;
	}

	/*
	 * In order to adhere to the semantics expected by the latest i915_gem_fault
	 * we make this an all or nothing. The implicit assumption here is that overlaps
	 * in page faults will not be sufficiently common to impair performance.
	 */
	if (__predict_false(rc != 0)) {
		count = pidx - pidx_start;
		pa = pfn << PAGE_SHIFT;
		for (pidx = pidx_start; pidx < pidx_start + count; pidx++, pa += PAGE_SIZE) {
			m = PHYS_TO_VM_PAGE(pa);
			if (vm_page_busied(m))
				vm_page_xunbusy(m);
			vm_page_lock(m);
			vm_page_remove(m);
			vm_page_unlock(m);
		}
		vma->vm_pfn_count = 0;
	}
	VM_OBJECT_WUNLOCK(vm_obj);
	return (rc);
}

MODULE_DEPEND(i915kms, drmn, 1, 1, 1);
MODULE_DEPEND(i915kms, agp, 1, 1, 1);
MODULE_DEPEND(i915kms, linuxkpi, 1, 1, 1);
MODULE_DEPEND(i915kms, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(i915kms, firmware, 1, 1, 1);
MODULE_DEPEND(i915kms, debugfs, 1, 1, 1);
