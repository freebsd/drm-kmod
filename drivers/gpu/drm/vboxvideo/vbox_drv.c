// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2013-2017 Oracle Corporation
 * This file is based on ast_drv.c
 * Copyright 2012 Red Hat Inc.
 * Authors: Dave Airlie <airlied@redhat.com>
 *          Michael Thayer <michael.thayer@oracle.com,
 *          Hans de Goede <hdegoede@redhat.com>
 */
#include <linux/console.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/vt_kern.h>

#include <drm/drm_crtc_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>

#include "vbox_drv.h"

#ifdef __FreeBSD__
SYSCTL_NODE(_hw, OID_AUTO, vboxvideo,
    CTLFLAG_RW | CTLFLAG_MPSAFE, 0,
    DRIVER_DESC " parameters");
#endif

static int vbox_modeset = -1;

MODULE_PARM_DESC(modeset, "Disable/Enable modesetting");
module_param_named(modeset, vbox_modeset, int, 0400);

static struct drm_driver driver;

static const struct pci_device_id pciidlist[] = {
	{ PCI_DEVICE(0x80ee, 0xbeef) },
	{ }
};
MODULE_DEVICE_TABLE(pci, pciidlist);

static int vbox_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct vbox_private *vbox;
	int ret = 0;

	if (!vbox_check_supported(VBE_DISPI_ID_HGSMI))
		return -ENODEV;

	vbox = kzalloc(sizeof(*vbox), GFP_KERNEL);
	if (!vbox)
		return -ENOMEM;

	ret = drm_dev_init(&vbox->ddev, &driver, &pdev->dev);
	if (ret) {
		kfree(vbox);
		return ret;
	}

	vbox->ddev.pdev = pdev;
	vbox->ddev.dev_private = vbox;
	pci_set_drvdata(pdev, vbox);
	mutex_init(&vbox->hw_mutex);

	ret = pci_enable_device(pdev);
	if (ret)
		goto err_dev_put;

	ret = vbox_hw_init(vbox);
	if (ret)
		goto err_pci_disable;

	ret = vbox_mm_init(vbox);
	if (ret)
		goto err_hw_fini;

	ret = vbox_mode_init(vbox);
	if (ret)
		goto err_mm_fini;

	ret = vbox_irq_init(vbox);
	if (ret)
		goto err_mode_fini;

	ret = drm_fbdev_generic_setup(&vbox->ddev, 32);
	if (ret)
		goto err_irq_fini;

	ret = drm_dev_register(&vbox->ddev, 0);
	if (ret)
		goto err_irq_fini;

	return 0;

err_irq_fini:
	vbox_irq_fini(vbox);
err_mode_fini:
	vbox_mode_fini(vbox);
err_mm_fini:
	vbox_mm_fini(vbox);
err_hw_fini:
	vbox_hw_fini(vbox);
err_pci_disable:
	pci_disable_device(pdev);
err_dev_put:
	drm_dev_put(&vbox->ddev);
	return ret;
}

static void vbox_pci_remove(struct pci_dev *pdev)
{
	struct vbox_private *vbox = pci_get_drvdata(pdev);

	drm_dev_unregister(&vbox->ddev);
	vbox_irq_fini(vbox);
	vbox_mode_fini(vbox);
	vbox_mm_fini(vbox);
	vbox_hw_fini(vbox);
	drm_dev_put(&vbox->ddev);
}

#ifdef CONFIG_PM_SLEEP
static int vbox_pm_suspend(struct device *dev)
{
	struct vbox_private *vbox = dev_get_drvdata(dev);
	int error;

	error = drm_mode_config_helper_suspend(&vbox->ddev);
	if (error)
		return error;

	pci_save_state(vbox->ddev.pdev);
	pci_disable_device(vbox->ddev.pdev);
	pci_set_power_state(vbox->ddev.pdev, PCI_D3hot);

	return 0;
}

static int vbox_pm_resume(struct device *dev)
{
	struct vbox_private *vbox = dev_get_drvdata(dev);

	if (pci_enable_device(vbox->ddev.pdev))
		return -EIO;

	return drm_mode_config_helper_resume(&vbox->ddev);
}

static int vbox_pm_freeze(struct device *dev)
{
	struct vbox_private *vbox = dev_get_drvdata(dev);

	return drm_mode_config_helper_suspend(&vbox->ddev);
}

static int vbox_pm_thaw(struct device *dev)
{
	struct vbox_private *vbox = dev_get_drvdata(dev);

	return drm_mode_config_helper_resume(&vbox->ddev);
}

static int vbox_pm_poweroff(struct device *dev)
{
	struct vbox_private *vbox = dev_get_drvdata(dev);

	return drm_mode_config_helper_suspend(&vbox->ddev);
}

static const struct dev_pm_ops vbox_pm_ops = {
	.suspend = vbox_pm_suspend,
	.resume = vbox_pm_resume,
	.freeze = vbox_pm_freeze,
	.thaw = vbox_pm_thaw,
	.poweroff = vbox_pm_poweroff,
	.restore = vbox_pm_resume,
};
#endif

static struct pci_driver vbox_pci_driver = {
	.name = DRIVER_NAME,
	.id_table = pciidlist,
	.probe = vbox_pci_probe,
	.remove = vbox_pci_remove,
#ifdef CONFIG_PM_SLEEP
	.driver.pm = &vbox_pm_ops,
#endif
};

DEFINE_DRM_GEM_FOPS(vbox_fops);

static struct drm_driver driver = {
	.driver_features =
	    DRIVER_MODESET | DRIVER_GEM | DRIVER_ATOMIC,

	.lastclose = drm_fb_helper_lastclose,

	.fops = &vbox_fops,
	.irq_handler = vbox_irq_handler,
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,

	DRM_GEM_VRAM_DRIVER,
};

static int __init vbox_init(void)
{
#ifdef CONFIG_VGA_CONSOLE
	if (vgacon_text_force() && vbox_modeset == -1)
		return -EINVAL;
#endif

	if (vbox_modeset == 0)
		return -EINVAL;

#ifdef __linux__
	return pci_register_driver(&vbox_pci_driver);
#else
	int ret;
	vbox_pci_driver.bsdclass = drm_devclass;
	ret = linux_pci_register_drm_driver(&vbox_pci_driver);
	if (ret)
		DRM_ERROR("Failed initializing DRM.\n");
	return ret;
#endif
}

static void __exit vbox_exit(void)
{
#ifdef __linux__
	pci_unregister_driver(&vbox_pci_driver);
#elif defined(__FreeBSD__)
	linux_pci_unregister_drm_driver(&vbox_pci_driver);
#endif
}

#ifdef __linux__
module_init(vbox_init);
module_exit(vbox_exit);

MODULE_AUTHOR("Oracle Corporation");
MODULE_AUTHOR("Hans de Goede <hdegoede@redhat.com>");
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL and additional rights");

#else
LKPI_DRIVER_MODULE(vboxvideo, vbox_init, vbox_exit);
LKPI_PNP_INFO(pci, vboxvideo, pciidlist);
MODULE_DEPEND(vboxvideo, drmn, 2, 2, 2);
MODULE_DEPEND(vboxvideo, ttm, 1, 1, 1);
MODULE_DEPEND(vboxvideo, agp, 1, 1, 1);
MODULE_DEPEND(vboxvideo, linuxkpi, 1, 1, 1);
MODULE_DEPEND(vboxvideo, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(vboxvideo, debugfs, 1, 1, 1);
#endif
