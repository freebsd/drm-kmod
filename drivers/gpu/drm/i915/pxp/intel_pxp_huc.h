/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021-2022, Intel Corporation. All rights reserved.
 */

#ifndef __INTEL_PXP_HUC_H__
#define __INTEL_PXP_HUC_H__

struct intel_pxp;

#ifdef CONFIG_DRM_I915_PXP
int intel_pxp_huc_load_and_auth(struct intel_pxp *pxp);
#else
static inline int intel_pxp_huc_load_and_auth(struct intel_pxp *pxp)
{
	return -ENODEV;
}
#endif

#endif /* __INTEL_PXP_HUC_H__ */
