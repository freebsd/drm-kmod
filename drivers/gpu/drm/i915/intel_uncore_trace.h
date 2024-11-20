/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2026 The FreeBSD Foundation
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM i915

#if !defined(__INTEL_UNCORE_TRACE_H__) || defined(TRACE_HEADER_MULTI_READ)
#define __INTEL_UNCORE_TRACE_H__

#include <sys/param.h>
#include <sys/ktr.h>

#include "i915_reg_defs.h"

static inline void
trace_i915_reg_rw(bool write, i915_reg_t reg, u64 val, int len, bool trace)
{
	CTR(KTR_DRM, "%s reg=0x%x, len=%d, val=(0x%x, 0x%x)",
	    write ? "write" : "read",
	    i915_mmio_reg_offset(reg),
	    len,
	    (u32)(val & 0xffffffff),
	    (u32)(val >> 32));
}
#endif /* __INTEL_UNCORE_TRACE_H__ */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH ../../drivers/gpu/drm/i915
#define TRACE_INCLUDE_FILE intel_uncore_trace
#include <trace/define_trace.h>
