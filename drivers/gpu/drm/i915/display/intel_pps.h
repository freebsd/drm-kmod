/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2020 Intel Corporation
 */

#ifndef __INTEL_PPS_H__
#define __INTEL_PPS_H__

#include <linux/types.h>

#include "intel_wakeref.h"

struct drm_i915_private;
struct intel_connector;
struct intel_crtc_state;
struct intel_dp;
struct intel_encoder;

intel_wakeref_t pps_lock(struct intel_dp *intel_dp);
intel_wakeref_t pps_unlock(struct intel_dp *intel_dp, intel_wakeref_t wakeref);

#define with_pps_lock(dp, wf)						\
	for ((wf) = pps_lock(dp); (wf); (wf) = pps_unlock((dp), (wf)))

void intel_dp_check_edp(struct intel_dp *intel_dp);
void _intel_edp_backlight_on(struct intel_dp *intel_dp);
void _intel_edp_backlight_off(struct intel_dp *intel_dp);
void intel_edp_backlight_power(struct intel_connector *connector, bool enable);

bool edp_panel_vdd_on(struct intel_dp *intel_dp);
void edp_panel_vdd_off(struct intel_dp *intel_dp, bool sync);
void edp_panel_vdd_off_sync(struct intel_dp *intel_dp);
void edp_panel_on(struct intel_dp *intel_dp);
void edp_panel_off(struct intel_dp *intel_dp);
void edp_panel_vdd_work(struct work_struct *__work);

void intel_edp_panel_vdd_on(struct intel_dp *intel_dp);
void intel_edp_panel_on(struct intel_dp *intel_dp);
void intel_edp_panel_off(struct intel_dp *intel_dp);
bool intel_edp_have_power(struct intel_dp *intel_dp);

void intel_edp_panel_vdd_sanitize(struct intel_dp *intel_dp);

void wait_panel_power_cycle(struct intel_dp *intel_dp);

void intel_dp_pps_init(struct intel_dp *intel_dp);
void intel_power_sequencer_reset(struct drm_i915_private *i915);
void intel_dp_init_panel_power_timestamps(struct intel_dp *intel_dp);

void vlv_init_panel_power_sequencer(struct intel_encoder *encoder,
				    const struct intel_crtc_state *crtc_state);

#endif /* __INTEL_PPS_H__ */
