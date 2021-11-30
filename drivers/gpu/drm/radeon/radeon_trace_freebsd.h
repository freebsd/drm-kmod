#if !defined(_RADEON_TRACE_FREEBSD_H_) || defined(TRACE_HEADER_MULTI_READ)
#define _RADEON_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>

#include "radeon.h"

static inline void
trace_radeon_cs(struct radeon_cs_parser * parser){
	CTR1(KTR_DRM, "radeon_cs %p", parser);
}

static inline void
trace_radeon_semaphore_signale(int ridx, struct radeon_semaphore * semaphore){
	CTR2(KTR_DRM, "radeon_semaphore_signale %d %p", ridx, semaphore);
}

static inline void
trace_radeon_semaphore_wait(int ridx, struct radeon_semaphore * semaphore){
	CTR2(KTR_DRM, "radeon_semaphore_wait %d %p", ridx, semaphore);
}

static inline void
trace_radeon_bo_create(void *bo)
{
        CTR1(KTR_DRM, "radeon_bo_create %p", bo);
}

static inline void
trace_radeon_fence_emit(void* ddev, int ring, int seq){
	CTR3(KTR_DRM, "radeon_fence_emit %p %d %d", ddev, ring, seq);
}

static inline void
trace_radeon_fence_wait_begin(void* ddev, int i, int seq){
	CTR3(KTR_DRM, "radeon_fence_wait_begin %p %d %d", ddev, i, seq);
}

static inline void
trace_radeon_fence_wait_end(void* ddev, int i, int seq){
	CTR3(KTR_DRM, "radeon_fence_wait_end %p %d %d", ddev, i, seq);
}

static inline void
trace_radeon_vm_grab_id(int i, int ring){
	CTR2(KTR_DRM, "radeon_vm_grab_idi %d %d", i, ring);
}

static inline void
trace_radeon_vm_flush(uint64_t pd_addr, int ring, int id){
	CTR3(KTR_DRM, "radeon_vm_flush %x %d %d", pd_addr, ring, id);
}

static inline void
trace_radeon_vm_set_page(uint64_t pe, uint64_t addr, unsigned count, uint32_t incr, uint32_t flags){
	CTR5(KTR_DRM, "radeon_vm_set_page %x %x %d %d %d", pe, addr, count, incr, flags);
	
}

static inline void
trace_radeon_vm_bo_update(void* bo_va){
	CTR1(KTR_DRM, "radeon_vm_bo_update %p", bo_va);
}

#endif
