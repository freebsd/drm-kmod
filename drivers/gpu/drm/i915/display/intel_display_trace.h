/* Public domain. */

#ifndef __INTEL_DISPLAY_TRACE_H__
#define __INTEL_DISPLAY_TRACE_H__

#include <sys/param.h>
#include <sys/ktr.h>

#include "i915_drv.h"
#include "intel_crtc.h"
#include "intel_display_limits.h"
#include "intel_display_types.h"
#include "intel_vblank.h"

#define __dev_name_display(display) dev_name((display)->drm->dev)
#define __dev_name_kms(obj) dev_name((obj)->base.dev->dev)

/*
 * Using identifiers from enum pipe in TP_printk() will confuse tools that
 * parse /sys/kernel/debug/tracing/{xe,i915}/<event>/format. So we use CPP
 * macros instead.
 */
#define _TRACE_PIPE_A	0
#define _TRACE_PIPE_B	1
#define _TRACE_PIPE_C	2
#define _TRACE_PIPE_D	3

/*
 * FIXME: Several TP_printk() calls below display frame and scanline numbers for
 * all possible pipes (regardless of whether they are available) and that is
 * done with a constant format string. A better approach would be to generate
 * that info dynamically based on available pipes, but, while we do not have
 * that implemented yet, let's assert that the constant format string indeed
 * covers all possible pipes.
 */
static_assert(I915_MAX_PIPES - 1 == _TRACE_PIPE_D);

#define _PIPES_FRAME_AND_SCANLINE_FMT_AB	\
	"pipe A: frame=%u, scanline=%u"		\
	", pipe B: frame=%u, scanline=%u"
#define _PIPES_FRAME_AND_SCANLINE_FMT_CD	\
	"pipe C: frame=%u, scanline=%u"		\
	", pipe D: frame=%u, scanline=%u"

/*
 * Paranoid sanity check that at least the enumeration starts at the
 * same value as _TRACE_PIPE_A.
 */
static_assert(PIPE_A == _TRACE_PIPE_A);

static inline void
trace_intel_pipe_enable(struct intel_crtc *crtc)
{
#ifdef KTR
	struct intel_display *display = to_intel_display(crtc);
	struct intel_crtc *it__;
	uint32_t frame[I915_MAX_PIPES], scanline[I915_MAX_PIPES];

	memset(frame, 0, sizeof(frame[0]) * I915_MAX_PIPES);
	memset(scanline, 0, sizeof(scanline[0]) * I915_MAX_PIPES);
	for_each_intel_crtc(display->drm, it__) {
		frame[it__->pipe] = intel_crtc_get_vblank_counter(it__);
		scanline[it__->pipe] = intel_get_crtc_scanline(it__);
	}
#endif

	CTR2(KTR_DRM,
	    "intel_pipe_enable[1/3]: dev %s, pipe %c enable",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe));
	CTR4(KTR_DRM,
	    "intel_pipe_enable[2/3]: " _PIPES_FRAME_AND_SCANLINE_FMT_AB,
	    frame[_TRACE_PIPE_A], scanline[_TRACE_PIPE_A],
	    frame[_TRACE_PIPE_B], scanline[_TRACE_PIPE_B]);
	CTR4(KTR_DRM,
	    "intel_pipe_enable[3/3]: " _PIPES_FRAME_AND_SCANLINE_FMT_CD,
	    frame[_TRACE_PIPE_C], scanline[_TRACE_PIPE_C],
	    frame[_TRACE_PIPE_D], scanline[_TRACE_PIPE_D]);
}

static inline void
trace_intel_pipe_disable(struct intel_crtc *crtc)
{
#ifdef KTR
	struct intel_display *display = to_intel_display(crtc);
	struct intel_crtc *it__;
	uint32_t frame[I915_MAX_PIPES], scanline[I915_MAX_PIPES];

	memset(frame, 0, sizeof(frame[0]) * I915_MAX_PIPES);
	memset(scanline, 0, sizeof(scanline[0]) * I915_MAX_PIPES);
	for_each_intel_crtc(display->drm, it__) {
		frame[it__->pipe] = intel_crtc_get_vblank_counter(it__);
		scanline[it__->pipe] = intel_get_crtc_scanline(it__);
	}
#endif

	CTR2(KTR_DRM,
	    "intel_pipe_disable[1/3]: dev %s, pipe %c enable",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe));
	CTR4(KTR_DRM,
	    "intel_pipe_disable[2/3]: " _PIPES_FRAME_AND_SCANLINE_FMT_AB,
	    frame[_TRACE_PIPE_A], scanline[_TRACE_PIPE_A],
	    frame[_TRACE_PIPE_B], scanline[_TRACE_PIPE_B]);
	CTR4(KTR_DRM,
	    "intel_pipe_disable[3/3]: " _PIPES_FRAME_AND_SCANLINE_FMT_CD,
	    frame[_TRACE_PIPE_C], scanline[_TRACE_PIPE_C],
	    frame[_TRACE_PIPE_D], scanline[_TRACE_PIPE_D]);
}

static inline void
trace_intel_crtc_flip_done(struct intel_crtc *crtc)
{
	CTR4(KTR_DRM,
	    "intel_crtc_flip_done: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(crtc->pipe),
	    intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc));
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
trace_intel_cpu_fifo_underrun(struct intel_display *display, enum pipe pipe)
{
#ifdef KTR
	struct intel_crtc *crtc = intel_crtc_for_pipe(display, pipe);
#endif

	CTR4(KTR_DRM,
	    "intel_cpu_fifo_underrun: dev %s, pipe %c, frame=%u, scanline=%u",
	    __dev_name_kms(crtc), pipe_name(pipe),
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_pch_fifo_underrun(struct intel_display *display, enum pipe pch_transcoder)
{
#ifdef KTR
	enum pipe pipe = pch_transcoder;
	struct intel_crtc *crtc = intel_crtc_for_pipe(display, pipe);
#endif

	CTR4(KTR_DRM,
	    "intel_pch_fifo_underrun: dev %s, pch transcoder %c, frame=%u, scanline=%u",
	    __dev_name_display(display), pipe_name(pipe),
	    intel_crtc_get_vblank_counter(crtc), intel_get_crtc_scanline(crtc));
}

static inline void
trace_intel_memory_cxsr(struct intel_display *display, bool old, bool new)
{
#ifdef KTR
	struct intel_crtc *crtc;
	uint32_t frame[I915_MAX_PIPES], scanline[I915_MAX_PIPES];

	memset(frame, 0, sizeof(frame[0]) * I915_MAX_PIPES);
	memset(scanline, 0, sizeof(scanline[0]) * I915_MAX_PIPES);
	for_each_intel_crtc(display->drm, crtc) {
		frame[crtc->pipe] = intel_crtc_get_vblank_counter(crtc);
		scanline[crtc->pipe] = intel_get_crtc_scanline(crtc);
	}
#endif

	CTR3(KTR_DRM,
	    "intel_memory_cxsr[1/3]: dev %s, cxsr %s->%s",
	    __dev_name_display(display), str_on_off(old), str_on_off(new));
	CTR4(KTR_DRM,
	    "intel_memory_cxsr[2/3]: " _PIPES_FRAME_AND_SCANLINE_FMT_AB,
	    frame[_TRACE_PIPE_A], scanline[_TRACE_PIPE_A],
	    frame[_TRACE_PIPE_B], scanline[_TRACE_PIPE_B]);
	CTR4(KTR_DRM,
	    "intel_memory_cxsr[3/3]: " _PIPES_FRAME_AND_SCANLINE_FMT_CD,
	    frame[_TRACE_PIPE_C], scanline[_TRACE_PIPE_C],
	    frame[_TRACE_PIPE_D], scanline[_TRACE_PIPE_D]);
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
trace_intel_plane_async_flip(struct intel_plane *plane, struct intel_crtc *crtc, bool async_flip)
{
	CTR6(KTR_DRM,
	    "intel_plane_async_flip: dev %s, pipe %c, plane %s, frame=%u, scanline=%u, async_flip=%s",
	    __dev_name_kms(plane), pipe_name(crtc->pipe), plane->base.name,
	    intel_crtc_get_vblank_counter(crtc),
	    intel_get_crtc_scanline(crtc),
	    str_yes_no(async_flip));
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
	struct intel_display *display = to_intel_display(plane->base.dev);
	struct intel_crtc *crtc = intel_crtc_for_pipe(display,
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
	struct intel_display *display = to_intel_display(plane->base.dev);
	struct intel_crtc *crtc = intel_crtc_for_pipe(display,
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
	struct intel_display *display = to_intel_display(plane->base.dev);
	struct intel_crtc *crtc = intel_crtc_for_pipe(display,
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
trace_intel_frontbuffer_invalidate(struct intel_display *display,
    unsigned int frontbuffer_bits, unsigned int origin)
{
	CTR3(KTR_DRM,
	    "intel_frontbuffer_invalidate: dev %s, frontbuffer_bits=0x%08x, origin=%u",
	    __dev_name_display(display), frontbuffer_bits, origin);
}

static inline void
trace_intel_frontbuffer_flush(struct intel_display *display,
    unsigned int frontbuffer_bits, unsigned int origin)
{
	CTR3(KTR_DRM,
	    "intel_frontbuffer_flush: dev %s, frontbuffer_bits=0x%08x, origin=%u",
	    __dev_name_display(display), frontbuffer_bits, origin);
}

#endif /* __INTEL_DISPLAY_TRACE_H__ */
