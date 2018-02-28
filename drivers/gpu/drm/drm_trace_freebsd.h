#ifndef _DRM_TRACE_FREEBSD_H_
#define _DRM_TRACE_FREEBSD_H_

#include <drm/drmP.h>


/* TRACE_EVENT(drm_vblank_event, */
/* TP_PROTO(int crtc, unsigned int seq), */
static inline void
trace_drm_vblank_event(int crtc, unsigned int seq)
{
	CTR2(KTR_DRM, "drm_vblank_event crtc %d, seq %u", crtc, seq);
}

/* TRACE_EVENT(drm_vblank_event_queued, */
/* TP_PROTO(struct drm_file *file, int crtc, unsigned int seq), */
static inline void
trace_drm_vblank_event_queued(struct drm_file *file, int crtc, unsigned int seq)
{
	CTR3(KTR_DRM, "drm_vblank_event_queued crtc %d, seq %u", file, crtc, seq);
}

/* TRACE_EVENT(drm_vblank_event_delivered, */
/* TP_PROTO(struct drm_file *file, int crtc, unsigned int seq), */
static inline void
trace_drm_vblank_event_delivered(struct drm_file *file, int crtc, unsigned int seq)
{
	CTR3(KTR_DRM, "drm_vblank_event_delivered drm_file %p, crtc %d, seq %u", file, crtc, seq);
}

#endif
