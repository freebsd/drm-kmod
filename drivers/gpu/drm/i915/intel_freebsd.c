#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <linux/device.h>
#include <linux/acpi.h>
#include <drm/i915_drm.h>
#include <linux/mm.h>
#include <linux/io-mapping.h>

#include <asm/pgtable.h>

#include <acpi/video.h>

#include "soc/intel_gmch.h"
#include "intel_acpi.h"
#include <linux/console.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/vgaarb.h>
#include <linux/vga_switcheroo.h>
#include <drm/drm_crtc_helper.h>
#include <drm/intel-gtt.h>

#include <machine/md_var.h>

#include <dev/agp/agp_i810.h>

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

void *
bsd_intel_pci_bus_alloc_mem(device_t dev, int *rid, uintmax_t size,
    resource_size_t *start, resource_size_t *end)
{
	struct resource *res;
	device_t vga;

	vga = device_get_parent(dev);
	res = BUS_ALLOC_RESOURCE(device_get_parent(vga), dev, SYS_RES_MEMORY,
	    rid, 0, ~0UL, size, RF_ACTIVE | RF_SHAREABLE);
	if (res != NULL) {
		*start = rman_get_start(res);
		*end = rman_get_end(res);
	}

	return (res);
}

void
bsd_intel_pci_bus_release_mem(device_t dev, int rid, void *res)
{
	device_t vga;

	vga = device_get_parent(dev);
#if __FreeBSD_version < 1500015
	BUS_DEACTIVATE_RESOURCE(device_get_parent(vga),
	    dev, SYS_RES_MEMORY, rid, res);
	BUS_RELEASE_RESOURCE(device_get_parent(vga),
	    dev, SYS_RES_MEMORY, rid, res);
#else
	BUS_DEACTIVATE_RESOURCE(device_get_parent(vga), dev, res);
	BUS_RELEASE_RESOURCE(device_get_parent(vga), dev, res);
#endif
}

bool
intel_gmch_enable_gtt(void)
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
intel_gmch_gtt_insert_page(dma_addr_t addr, unsigned int pg, unsigned int flags)
{

	intel_gtt_install_pte(pg, addr, flags);
	(void)intel_gtt_chipset_flush();
}

void
intel_gmch_gtt_insert_sg_entries(struct sg_table *st, unsigned int pg_start,
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

void
intel_gmch_gtt_get(uint64_t *gtt_total, phys_addr_t *mappable_base,
    resource_size_t *mappable_end)
{
	struct intel_gtt *gtt;

	gtt = intel_gtt_get();
	*gtt_total = gtt->gtt_total_entries << PAGE_SHIFT;
	*mappable_base = gtt->gma_bus_addr;
	*mappable_end = gtt->gtt_mappable_entries << PAGE_SHIFT;
}

void
intel_gmch_gtt_clear_range(unsigned int first_entry, unsigned int num_entries)
{
	intel_gtt_clear_range(first_entry, num_entries);
}

void
intel_gmch_gtt_flush(void)
{
	intel_gtt_chipset_flush();
}

void
intel_acpi_video_register(struct drm_i915_private *i915)
{
	acpi_video_register();
}
