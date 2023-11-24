#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <linux/device.h>
#include <linux/acpi.h>
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

#include <machine/md_var.h>

/*
 * intel_graphics_stolen_* are defined in sys/dev/pci/pcivar.h
 * and set at early boot from machdep.c. Copy over the values
 * here to a linux_resource struct.
 */
struct linux_resource intel_graphics_stolen_res;

void *intel_gtt_get_registers(void);
void _intel_gtt_get(size_t *gtt_total, size_t *stolen_size, unsigned long *mappable_end);
void intel_gtt_install_pte(unsigned int index, vm_paddr_t addr, unsigned int flags);
uint32_t intel_gtt_read_pte(unsigned int entry);

#define AGP_I810_PGTBL_CTL	0x2020
#define	AGP_I810_PGTBL_ENABLED	0x00000001
#define INTEL_GTT_GEN		intel_private.gen
#define GFX_FLSH_CNTL_BSD	0x2170 /* 915+ only */
#define HAS_PGTBL_EN		1
#define I915_GMADR_BAR		2

#define WARN_UN() log(LOG_WARNING, "%s unimplemented", __FUNCTION__)

#ifdef __notyet__
static struct _intel_private {
	struct pci_dev *bridge_dev;
	u8 __iomem *registers;
	u32 PGTBL_save;
	int gen;
	phys_addr_t gma_bus_addr;
} intel_private;
#endif

bool
intel_enable_gtt(void)
{
#ifdef __notyet__
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

	/*
	 * On the resume path we may be adjusting the PGTBL value, so
	 * be paranoid and flush all chipset write buffers...
	 */
	if (INTEL_GTT_GEN >= 3)
		writel(0, intel_private.registers + GFX_FLSH_CNTL_BSD);

	reg = intel_private.registers + AGP_I810_PGTBL_CTL;
	writel(intel_private.PGTBL_save, reg);
#ifdef __notyet__
	if (HAS_PGTBL_EN && (readl(reg) & AGP_I810_PGTBL_ENABLED) == 0) {
		dev_err(&intel_private.pcidev->dev,
			"failed to enable the GTT: PGTBL=%x [expected %x]\n",
			readl(reg), intel_private.PGTBL_save);
		return false;
	}
#endif

	if (INTEL_GTT_GEN >= 3)
		writel(0, intel_private.registers + GFX_FLSH_CNTL_BSD);
	DRM_DEBUG("exiting %s\n", __func__);
#endif
	return (1);
}

int
intel_gmch_probe(struct pci_dev *bridge_pdev, struct pci_dev *gpu_pdev,
		 struct agp_bridge_data *bridge)
{
#ifdef __notyet__
	DRM_DEBUG("entering %s\n", __func__);
	intel_private.registers = NULL; //intel_gtt_get_registers();
	intel_private.gma_bus_addr = pci_bus_address(gpu_pdev, I915_GMADR_BAR);
	DRM_DEBUG("bus_addr %lx\n", intel_private.gma_bus_addr);
	INTEL_GTT_GEN = 4;
	/* save the PGTBL reg for resume */
	intel_private.PGTBL_save =
	    readl(intel_private.registers + AGP_I810_PGTBL_CTL) &
	    ~AGP_I810_PGTBL_ENABLED;
	/* we only ever restore the register when enabling the PGTBL... */
	if (HAS_PGTBL_EN)
		intel_private.PGTBL_save |= AGP_I810_PGTBL_ENABLED;

	DRM_DEBUG("exiting %s\n", __func__);
#endif
	return (1);
}

void
intel_gmch_remove(void)
{
}

void
intel_gtt_insert_page(dma_addr_t addr, unsigned int pg, unsigned int flags)
{

	intel_gtt_install_pte(pg, addr, flags);
	(void)intel_gtt_chipset_flush();
}

void
linux_intel_gtt_insert_sg_entries(struct sg_table *st, unsigned int pg_start,
    unsigned int flags)
{
	struct sg_page_iter sg_iter;
	unsigned int i;
	vm_paddr_t addr;

	i = 0;
	for_each_sg_page(st->sgl, &sg_iter, st->nents, 0) {
		addr = sg_page_iter_dma_address(&sg_iter);
		intel_gtt_install_pte(pg_start + i, addr, flags);
		++i;
	}

	intel_gtt_read_pte(pg_start + i - 1);
}
