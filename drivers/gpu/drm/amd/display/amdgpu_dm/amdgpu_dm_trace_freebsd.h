#ifndef _AMDGPU_DM_TRACE_FREEBSD_H_
#define _AMDGPU_DM_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>
#include <drm/drm_os_freebsd.h>	/* KTR_DRM */

/* TRACE_EVENT(amdgpu_dc_rreg, */
/* 	TP_PROTO(unsigned long *read_count, uint32_t reg, uint32_t value), */
static inline void
trace_amdgpu_dc_rreg(unsigned long *read_count, uint32_t reg, uint32_t value)
{
	CTR3(KTR_DRM, "amdgpu_dc_rreg %p %d %d", read_count, reg, value);
}

/* TRACE_EVENT(amdgpu_dc_wreg, */
	/* TP_PROTO(unsigned long *write_count, uint32_t reg, uint32_t value), */
static inline void
trace_amdgpu_dc_wreg(unsigned long *write_count, uint32_t reg, uint32_t value)
{
	CTR3(KTR_DRM, "amdgpu_dc_wreg %p %d %d", write_count, reg, value);
}

/* TRACE_EVENT(amdgpu_dc_performance, */
/* 	TP_PROTO(unsigned long read_count, unsigned long write_count, */
/* 		unsigned long *last_read, unsigned long *last_write, */
/* 		const char *func, unsigned int line), */

static inline void
trace_amdgpu_dc_performance(unsigned long read_count, unsigned long write_count,
    unsigned long *last_read, unsigned long *last_write, const char *func, unsigned int line)
{
	CTR6(KTR_DRM, "amdgpu_dc_performance %u %u %p %p %s %u", read_count,
	    write_count, last_read, last_write, func, line);
}

#endif /* _AMDGPU_DM_TRACE_FREEBSD_H_ */
