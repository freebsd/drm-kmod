// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2013-2017 Oracle Corporation
 * This file is based on ast_main.c
 * Copyright 2012 Red Hat Inc.
 * Authors: Dave Airlie <airlied@redhat.com>,
 *          Michael Thayer <michael.thayer@oracle.com,
 *          Hans de Goede <hdegoede@redhat.com>
 */

#include <linux/vbox_err.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_damage_helper.h>

#include "vbox_drv.h"
#include "vboxvideo_guest.h"
#include "vboxvideo_vbe.h"

void vbox_report_caps(struct vbox_private *vbox)
{
	u32 caps = VBVACAPS_DISABLE_CURSOR_INTEGRATION |
		   VBVACAPS_IRQ | VBVACAPS_USE_VBVA_ONLY;

	/* The host only accepts VIDEO_MODE_HINTS if it is send separately. */
	hgsmi_send_caps_info(vbox->guest_pool, caps);
	caps |= VBVACAPS_VIDEO_MODE_HINTS;
	hgsmi_send_caps_info(vbox->guest_pool, caps);
}

static int vbox_accel_init(struct vbox_private *vbox)
{
	struct vbva_buffer *vbva;
	unsigned int i;

	vbox->vbva_info = devm_kcalloc(vbox->ddev.dev, vbox->num_crtcs,
				       sizeof(*vbox->vbva_info), GFP_KERNEL);
	if (!vbox->vbva_info)
		return -ENOMEM;

	/* Take a command buffer for each screen from the end of usable VRAM. */
	vbox->available_vram_size -= vbox->num_crtcs * VBVA_MIN_BUFFER_SIZE;

#ifdef __linux__
	vbox->vbva_buffers = pci_iomap_range(vbox->ddev.pdev, 0,
					     vbox->available_vram_size,
					     vbox->num_crtcs *
					     VBVA_MIN_BUFFER_SIZE);
#else
	struct resource *res;
	int rid, type;

	rid = PCIR_BAR(0);
	type = pci_resource_type(vbox->ddev.pdev, 0);
	res = bus_alloc_resource_any(vbox->ddev.pdev->dev.bsddev, type, &rid, RF_ACTIVE);
	if (!res) {
		DRM_ERROR("Could not allocate resource for vbva buffers\n");
		return -ENOMEM;
	}
	vbox->vbva_buffers_res = res;
	vbox->vbva_buffers_rid = rid;
	vbox->vbva_buffers_restype = type;
	/* BSDFIXME: Map with offset in a less dodgy way */
	vbox->vbva_buffers = (void *)(rman_get_bushandle(res) + vbox->available_vram_size);
#endif
	if (!vbox->vbva_buffers)
		return -ENOMEM;

	for (i = 0; i < vbox->num_crtcs; ++i) {
		vbva_setup_buffer_context(&vbox->vbva_info[i],
					  vbox->available_vram_size +
					  i * VBVA_MIN_BUFFER_SIZE,
					  VBVA_MIN_BUFFER_SIZE);
		vbva = (void __force *)vbox->vbva_buffers +
			i * VBVA_MIN_BUFFER_SIZE;
		if (!vbva_enable(&vbox->vbva_info[i],
				 vbox->guest_pool, vbva, i)) {
			/* very old host or driver error. */
			DRM_ERROR("vboxvideo: vbva_enable failed\n");
		}
	}

	return 0;
}

static void vbox_accel_fini(struct vbox_private *vbox)
{
	unsigned int i;

	for (i = 0; i < vbox->num_crtcs; ++i)
		vbva_disable(&vbox->vbva_info[i], vbox->guest_pool, i);

	pci_iounmap(vbox->ddev.pdev, vbox->vbva_buffers);
}

/* Do we support the 4.3 plus mode hint reporting interface? */
static bool have_hgsmi_mode_hints(struct vbox_private *vbox)
{
	u32 have_hints, have_cursor;
	int ret;

	ret = hgsmi_query_conf(vbox->guest_pool,
			       VBOX_VBVA_CONF32_MODE_HINT_REPORTING,
			       &have_hints);
	if (ret)
		return false;

	ret = hgsmi_query_conf(vbox->guest_pool,
			       VBOX_VBVA_CONF32_GUEST_CURSOR_REPORTING,
			       &have_cursor);
	if (ret)
		return false;

	return have_hints == VINF_SUCCESS && have_cursor == VINF_SUCCESS;
}

bool vbox_check_supported(u16 id)
{
	u16 dispi_id;

	vbox_write_ioport(VBE_DISPI_INDEX_ID, id);
	dispi_id = inw(VBE_DISPI_IOPORT_DATA);

	return dispi_id == id;
}

int vbox_hw_init(struct vbox_private *vbox)
{
	int ret = -ENOMEM;

	vbox->full_vram_size = inl(VBE_DISPI_IOPORT_DATA);
	vbox->any_pitch = vbox_check_supported(VBE_DISPI_ID_ANYX);

	DRM_INFO("VRAM %08x\n", vbox->full_vram_size);

	/* Map guest-heap at end of vram */
#ifdef __linux__
	vbox->guest_heap =
	    pci_iomap_range(vbox->ddev.pdev, 0, GUEST_HEAP_OFFSET(vbox),
			    GUEST_HEAP_SIZE);
#else
	struct resource *res;
	int rid, type;

	rid = PCIR_BAR(0);
	type = pci_resource_type(vbox->ddev.pdev, 0);
	res = bus_alloc_resource_any(vbox->ddev.pdev->dev.bsddev, type, &rid, RF_ACTIVE);
	if (!res) {
		DRM_ERROR("Could not allocate resource for guest heap\n");
		return -ENOMEM;
	}
	vbox->guest_heap_res = res;
	vbox->guest_heap_rid = rid;
	vbox->guest_heap_restype = type;
	/* BSDFIXME: Map with offset in a less dodgy way */
	vbox->guest_heap = (void *)rman_get_bushandle(res) + GUEST_HEAP_OFFSET(vbox);
#endif
	if (!vbox->guest_heap)
		return -ENOMEM;

	/* Create guest-heap mem-pool use 2^4 = 16 byte chunks */
	vbox->guest_pool = gen_pool_create(4, -1);
	if (!vbox->guest_pool)
		goto err_unmap_guest_heap;

	ret = gen_pool_add_virt(vbox->guest_pool,
				(unsigned long)vbox->guest_heap,
				GUEST_HEAP_OFFSET(vbox),
				GUEST_HEAP_USABLE_SIZE, -1);
	if (ret)
		goto err_destroy_guest_pool;

	ret = hgsmi_test_query_conf(vbox->guest_pool);
	if (ret) {
		DRM_ERROR("vboxvideo: hgsmi_test_query_conf failed\n");
		goto err_destroy_guest_pool;
	}

	/* Reduce available VRAM size to reflect the guest heap. */
	vbox->available_vram_size = GUEST_HEAP_OFFSET(vbox);
	/* Linux drm represents monitors as a 32-bit array. */
	hgsmi_query_conf(vbox->guest_pool, VBOX_VBVA_CONF32_MONITOR_COUNT,
			 &vbox->num_crtcs);
	vbox->num_crtcs = clamp_t(u32, vbox->num_crtcs, 1, VBOX_MAX_SCREENS);

	if (!have_hgsmi_mode_hints(vbox)) {
		ret = -ENOTSUPP;
		goto err_destroy_guest_pool;
	}

	vbox->last_mode_hints = devm_kcalloc(vbox->ddev.dev, vbox->num_crtcs,
					     sizeof(struct vbva_modehint),
					     GFP_KERNEL);
	if (!vbox->last_mode_hints) {
		ret = -ENOMEM;
		goto err_destroy_guest_pool;
	}

	ret = vbox_accel_init(vbox);
	if (ret)
		goto err_destroy_guest_pool;

	return 0;

err_destroy_guest_pool:
	gen_pool_destroy(vbox->guest_pool);
err_unmap_guest_heap:
	pci_iounmap(vbox->ddev.pdev, vbox->guest_heap);
	return ret;
}

void vbox_hw_fini(struct vbox_private *vbox)
{
	vbox_accel_fini(vbox);
	gen_pool_destroy(vbox->guest_pool);
	pci_iounmap(vbox->ddev.pdev, vbox->guest_heap);
}
