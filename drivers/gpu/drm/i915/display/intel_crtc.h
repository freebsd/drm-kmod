/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2020 Intel Corporation
 */

#ifndef _INTEL_CRTC_H_
#define _INTEL_CRTC_H_

#include <linux/types.h>

enum pipe;
struct drm_display_mode;
struct drm_i915_private;
struct intel_atomic_state;
struct intel_crtc;
struct intel_crtc_state;

int intel_usecs_to_scanlines(const struct drm_display_mode *adjusted_mode,
			     int usecs);
u32 intel_crtc_max_vblank_count(const struct intel_crtc_state *crtc_state);
int intel_crtc_init(struct drm_i915_private *dev_priv, enum pipe pipe);
struct intel_crtc_state *intel_crtc_state_alloc(struct intel_crtc *crtc);
void intel_crtc_state_reset(struct intel_crtc_state *crtc_state,
			    struct intel_crtc *crtc);
u32 intel_crtc_get_vblank_counter(struct intel_crtc *crtc);
void intel_crtc_vblank_on(const struct intel_crtc_state *crtc_state);
void intel_crtc_vblank_off(const struct intel_crtc_state *crtc_state);
void intel_pipe_update_start(struct intel_crtc_state *new_crtc_state);
void intel_pipe_update_end(struct intel_crtc_state *new_crtc_state);
void intel_wait_for_vblank_workers(struct intel_atomic_state *state);

#endif
