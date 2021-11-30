/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2018 Johannes Lundberg <johalun0@gmail.com>
 * Copyright (c) 2021 Vladimir Kondratyev <wulf@FreeBSD.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _DRM_TRACE_FREEBSD_H_
#define _DRM_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>

#include <linux/ktime.h>

struct drm_file;

/* TRACE_EVENT(drm_vblank_event, */
/* TP_PROTO(int crtc, unsigned int seq, ktime_t time, bool high_prec), */
static inline void
trace_drm_vblank_event(int crtc, unsigned int seq, ktime_t time, bool high_prec)
{
	CTR4(KTR_DRM, "drm_vblank_event crtc %d, seq %u, time %lld, "
	    "high-prec %s", crtc, seq, time, high_prec ? "true" : "false");
}

/* TRACE_EVENT(drm_vblank_event_queued, */
/* TP_PROTO(struct drm_file *file, int crtc, unsigned int seq), */
static inline void
trace_drm_vblank_event_queued(struct drm_file *file, int crtc, unsigned int seq)
{
	CTR3(KTR_DRM, "drm_vblank_event_queued drm_file %p, crtc %d, seq %u",
	    file, crtc, seq);
}

/* TRACE_EVENT(drm_vblank_event_delivered, */
/* TP_PROTO(struct drm_file *file, int crtc, unsigned int seq), */
static inline void
trace_drm_vblank_event_delivered(struct drm_file *file, int crtc, unsigned int seq)
{
	CTR3(KTR_DRM, "drm_vblank_event_delivered drm_file %p, crtc %d, seq %u", file, crtc, seq);
}

#endif
