/* Public domain. */

#ifndef __INTEL_DISPLAY_TRACE_H__
#define __INTEL_DISPLAY_TRACE_H__

#include <sys/param.h>
#include <sys/ktr.h>

#include "i915_drv.h"
#include "intel_crtc.h"
#include "intel_display_types.h"
#include "intel_vblank.h"

#define __dev_name_i915(i915) dev_name((i915)->drm.dev)
#define __dev_name_kms(obj) dev_name((obj)->base.dev->dev)

static inline void
trace_intel_pipe_enable(struct intel_crtc *crtc)
{
#ifdef KTR
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc *it__;
	uint32_t frame[3], scanline[3];

	for_each_intel_crtc(&dev_priv->drm, it__) {
		frame[it__->pipe] = intel_crtc_get_vblank_counter(it__);
		scanline[it__->pipe] = intel_get_crtc_scanline(it__);
	}
#endif

	CTR2(KTR_DRM,
	    "intel_pipe_enable[1/2]: dev %s, pipe %c enable",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe));
	CTR6(KTR_DRM,
	    "intel_pipe_enable[2/2]: "
	    "pipe A: frame=%u, scanline=%u, pipe B: frame=%u, scanline=%u, pipe C: frame=%u, scanline=%u",
	    frame[PIPE_A], scanline[PIPE_A],
	    frame[PIPE_B], scanline[PIPE_B],
	    frame[PIPE_C], scanline[PIPE_C]);
}

static inline void
trace_intel_pipe_disable(struct intel_crtc *crtc)
{
#ifdef KTR
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc *it__;
	uint32_t frame[3], scanline[3];

	for_each_intel_crtc(&dev_priv->drm, it__) {
		frame[it__->pipe] = intel_crtc_get_vblank_counter(it__);
		scanline[it__->pipe] = intel_get_crtc_scanline(it__);
	}
#endif

	CTR2(KTR_DRM,
	    "intel_pipe_disable[1/2]: dev %s, pipe %c enable",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe));
	CTR6(KTR_DRM,
	    "intel_pipe_disable[2/2]: "
	    "pipe A: frame=%u, scanline=%u, pipe B: frame=%u, scanline=%u, pipe C: frame=%u, scanline=%u",
	    frame[PIPE_A], scanline[PIPE_A],
	    frame[PIPE_B], scanline[PIPE_B],
	    frame[PIPE_C], scanline[PIPE_C]);
}

static inline void
trace_intel_pipe_crc(struct intel_crtc *crtc, const u32 *crcs)
{
	CTR4(KTR_DRM,
	    "intel_pipe_crc[1/2]: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe),
	    intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc));
	CTR5(KTR_DRM,
	    "intel_pipe_crc[2/2]: crc=%08x %08x %08x %08x %08x",
	    crcs[0], crcs[1], crcs[2], crcs[3], crcs[4]);
}

static inline void
trace_intel_cpu_fifo_underrun(struct drm_i915_private *dev_priv, enum pipe pipe)
{
#ifdef KTR
	struct intel_crtc *crtc = intel_crtc_for_pipe(dev_priv, pipe);
#endif

	CTR4(KTR_DRM,
	    "intel_cpu_fifo_underrun: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(pipe),
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_pch_fifo_underrun(struct drm_i915_private *dev_priv, enum pipe pch_transcoder)
{
#ifdef KTR
	enum pipe pipe = pch_transcoder;
	struct intel_crtc *crtc = intel_crtc_for_pipe(dev_priv, pipe);
#endif

	CTR4(KTR_DRM,
	    "intel_pch_fifo_underrun: dev %s, pch transcoder %c, frame=%u, scanline=%u",
	    __dev_name_i915(dev_priv), pipe_name(pipe),
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_memory_cxsr(struct drm_i915_private *dev_priv, bool old, bool new)
{
#ifdef KTR
	struct intel_crtc *crtc;
	uint32_t frame[3], scanline[3];

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		frame[crtc->pipe] = intel_crtc_get_vblank_counter(crtc);
		scanline[crtc->pipe] = intel_get_crtc_scanline(crtc);
	}
#endif

	CTR3(KTR_DRM,
	    "intel_memory_cxsr[1/2]: dev %s, cxsr %s->%s",
	    __dev_name_i915(dev_priv), str_on_off(old), str_on_off(new));
	CTR6(KTR_DRM, 
	    "intel_memory_cxsr[2/2]: "
	    "pipe A: frame=%u, scanline=%u, pipe B: frame=%u, scanline=%u, pipe C: frame=%u, scanline=%u",
	    frame[PIPE_A], scanline[PIPE_A],
	    frame[PIPE_B], scanline[PIPE_B],
	    frame[PIPE_C], scanline[PIPE_C]);
}

static inline void
trace_g4x_wm(struct intel_crtc *crtc, const struct g4x_wm_values *wm)
{
	CTR4(KTR_DRM,
	    "g4x_wm[1/4]: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe),
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
	CTR3(KTR_DRM,
	    "g4x_wm[2/4]: wm %d/%d/%d",
	    wm->pipe[crtc->pipe].plane[PLANE_PRIMARY],
	    wm->pipe[crtc->pipe].plane[PLANE_SPRITE0],
	    wm->pipe[crtc->pipe].plane[PLANE_CURSOR]);
	CTR4(KTR_DRM,
	    "g4x_wm[3/4]: sr %s/%d/%d/%d",
	    str_yes_no(wm->cxsr), wm->sr.plane, wm->sr.cursor, wm->sr.fbc);
	CTR5(KTR_DRM,
	    "g4x_wm(4/4]: hpll %s/%d/%d/%d, fbc %s",
	    str_yes_no(wm->hpll_en), wm->hpll.plane, wm->hpll.cursor, wm->hpll.fbc,
	    str_yes_no(wm->fbc_en));
}

static inline void
trace_vlv_wm(struct intel_crtc *crtc, const struct vlv_wm_values *wm)
{
	CTR6(KTR_DRM,
	    "vlv_wm[1/2]: dev %s, pipe %c, frame=%u, scanline=%u, level=%d, cxsr=%d",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe), intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc), wm->level, wm->cxsr);
	CTR6(KTR_DRM,
	    "vlv_wm[2/2]: wm %d/%d/%d/%d, sr %d/%d",
	    wm->pipe[crtc->pipe].plane[PLANE_PRIMARY],
	    wm->pipe[crtc->pipe].plane[PLANE_SPRITE0],
	    wm->pipe[crtc->pipe].plane[PLANE_SPRITE1],
	    wm->pipe[crtc->pipe].plane[PLANE_CURSOR],
	    wm->sr.plane, wm->sr.cursor);
}

static inline void
trace_vlv_fifo_size(struct intel_crtc *crtc, u32 sprite0_start, u32 sprite1_start, u32 fifo_size)
{
	CTR4(KTR_DRM,
	    "vlv_fifo_size[1/2]: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe),
	    intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc));
	CTR3(KTR_DRM,
	    "vlv_fifo_size[2/2]: %d/%d/%d",
	    sprite0_start, sprite1_start, fifo_size);
}

static inline void
trace_intel_plane_update_noarm(struct intel_plane *plane, struct intel_crtc *crtc)
{
	CTR5(KTR_DRM,
	    "intel_plane_update_noarm[1/3]: dev %s, pipe %c, plane %s, frame=%u, scanline=%u",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
	/* FIXME FreeBSD
	CTR8(KTR_DRM,
	    "intel_plane_update_noarm[2/3]: " DRM_RECT_FP_FMT " ->",
	    DRM_RECT_FP_ARG(&plane->state->src));
	CTR4(KTR_DRM,
	    "intel_plane_update_noarm[3/3]: " DRM_RECT_FMT,
	    DRM_RECT_ARG(&plane->state->dst)); */
}

static inline void
trace_intel_plane_update_arm(struct intel_plane *plane, struct intel_crtc *crtc)
{
	CTR5(KTR_DRM,
	    "intel_plane_update_arm[1/3]: dev %s, pipe %c, plane %s, frame=%u, scanline=%u",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
	/* FIXME FreeBSD
	CTR8(KTR_DRM,
	    "intel_plane_update_arm[2/3]: " DRM_RECT_FP_FMT " ->",
	    DRM_RECT_FP_ARG(&plane->state->src));
	CTR4(KTR_DRM,
	    "intel_plane_update_arm[3/3]: " DRM_RECT_FMT,
	    DRM_RECT_ARG(&plane->state->dst)); */
}

static inline void
trace_intel_plane_disable_arm(struct intel_plane *plane, struct intel_crtc *crtc)
{
	CTR5(KTR_DRM,
	    "intel_plane_disable_arm: dev %s, pipe %c, plane %s, frame=%u, scanline=%u",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_fbc_activate(struct intel_plane *plane)
{
#ifdef KTR
	struct intel_crtc *crtc = intel_crtc_for_pipe(to_i915(plane->base.dev),
	    plane->pipe);
#endif

	CTR5(KTR_DRM,
	    "intel_fbc_activate: dev %s, pipe %c, plane %s, frame=%u, scanline=%u",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_fbc_deactivate(struct intel_plane *plane)
{
#ifdef KTR
	struct intel_crtc *crtc = intel_crtc_for_pipe(to_i915(plane->base.dev),
	    plane->pipe);
#endif

	CTR5(KTR_DRM,
	    "intel_fbc_deactivate: dev %s, pipe %c, plane %s, frame=%u, scanline=%u",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_fbc_nuke(struct intel_plane *plane)
{
#ifdef KTR
	struct intel_crtc *crtc = intel_crtc_for_pipe(to_i915(plane->base.dev),
	    plane->pipe);
#endif

	CTR5(KTR_DRM,
	    "intel_fbc_nuke: dev %s, pipe %c, plane %s, frame=%u, scanline=%u",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_crtc_vblank_work_start(struct intel_crtc *crtc)
{
	CTR4(KTR_DRM,
	    "intel_crtc_vblank_work_start: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe), intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_crtc_vblank_work_end(struct intel_crtc *crtc)
{
	CTR4(KTR_DRM,
	    "intel_crtc_vblank_work_end: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe), intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_pipe_update_start(struct intel_crtc *crtc)
{
	CTR6(KTR_DRM,
	    "dev %s, pipe %c, frame=%u, scanline=%u, min=%u, max=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe), intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc), crtc->debug.min_vbl, crtc->debug.max_vbl);
}

static inline void
trace_intel_pipe_update_vblank_evaded(struct intel_crtc *crtc)
{
	CTR6(KTR_DRM,
	    "intel_pipe_update_vblank_evaded: dev %s, pipe %c, frame=%u, scanline=%u, min=%u, max=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe), crtc->debug.start_vbl_count,
	    crtc->debug.scanline_start, crtc->debug.min_vbl, crtc->debug.max_vbl);
}

static inline void
trace_intel_pipe_update_end(struct intel_crtc *crtc, u32 frame, int scanline_end)
{
	CTR4(KTR_DRM,
	    "intel_pipe_update_end: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe), frame, scanline_end);
}

static inline void
trace_intel_frontbuffer_invalidate(struct drm_i915_private *i915,
    unsigned int frontbuffer_bits, unsigned int origin)
{
	CTR3(KTR_DRM,
	    "intel_frontbuffer_invalidate: dev %s, frontbuffer_bits=0x%08x, origin=%u",
	    __dev_name_i915(i915), frontbuffer_bits, origin);
}

static inline void
trace_intel_frontbuffer_flush(struct drm_i915_private *i915,
    unsigned int frontbuffer_bits, unsigned int origin)
{
	CTR3(KTR_DRM,
	    "intel_frontbuffer_flush: dev %s, frontbuffer_bits=0x%08x, origin=%u",
	    __dev_name_i915(i915), frontbuffer_bits, origin);
}

#endif /* __INTEL_DISPLAY_TRACE_H__ */
