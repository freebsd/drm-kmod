#ifndef _I915_TRACE_FREEBSD_H_
#define _I915_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>

#include "i915_drv.h"
#include "intel_crtc.h"
#include "intel_display_types.h"
#include "gt/intel_engine.h"

static inline void
trace_i915_flip_complete(int plane, struct drm_i915_gem_object *pending_flip_obj)
{
	CTR2(KTR_DRM, "i915_flip_complete %d %p", plane, pending_flip_obj);
}

static inline void
trace_i915_flip_request(int plane, struct drm_i915_gem_object *obj)
{
	CTR2(KTR_DRM, "i915_flip_request %d %p", plane, obj);
}
#if 0
static inline void
trace_i915_ring_wait_begin(struct intel_ring_buffer *ring){
	CTR1(KTR_DRM, "ring_wait_begin %s", ring->name);
}

static inline void
trace_i915_ring_wait_end(struct intel_ring_buffer *ring)
{
	CTR1(KTR_DRM, "ring_wait_end %s", ring->name);
}

static inline void
trace_i915_request_complete(struct intel_ring_buffer *ring, u32 seqno)
{
	CTR2(KTR_DRM, "request_complete %s %d", ring->name, seqno);
}
#endif

static inline void
trace_i915_gem_ring_flush(void *req, u32 arg1, u32 arg2)
{
	CTR3(KTR_DRM, "ring_flush %p %d %d", req, arg1, arg2);
}

static inline void
trace_i915_gem_ring_dispatch(void *req, u32 flags)
{
	CTR2(KTR_DRM, "ring_dispatch req %p flags %x", req, flags);
}

static inline void
trace_i915_gem_object_create(struct drm_i915_gem_object *obj)
{
	CTR1(KTR_DRM, "object_create %p", obj);
}

static inline void
trace_i915_gem_object_pread(struct drm_i915_gem_object *obj, u64 offset, u64 size)
{
	CTR3(KTR_DRM, "pread %p %jx %jx", obj, offset, size);
}

static inline void
trace_i915_gem_object_pwrite(struct drm_i915_gem_object *obj, u64 offset, u64 size)
{
	CTR3(KTR_DRM, "pwrite %p %jx %jx", obj, offset, size);
}
#if 0
static inline void
trace_i915_request_wait_end(struct intel_ring_buffer *ring, u32 seqno)
{
	CTR2(KTR_DRM, "request_wait_end %s %d", ring->name, seqno);
}

static inline void
trace_i915_request_add(struct intel_ring_buffer *ring, u32 seqno)
{
	CTR2(KTR_DRM, "request_add %s %d", ring->name, seqno);
}

static inline void
trace_i915_request_retire(struct intel_ring_buffer *ring, u32 seqno)
{
	CTR2(KTR_DRM, "retire_request_seqno_passed %s %d",
		ring->name, seqno);
}

#endif
static inline void
trace_i915_gem_object_change_domain(struct drm_i915_gem_object *obj, u32 old_read_domains, u32 old_write_domain)
{
	CTR3(KTR_DRM, "object_change_domain  %p %x %x",
		obj, old_read_domains, old_write_domain);
}

#if 0
static inline void
trace_i915_request_wait_begin(struct intel_ring_buffer *ring, u32 seqno)
{
	CTR2(KTR_DRM, "request_wait_begin %s %d", ring->name, seqno);
}
#endif

static inline void
trace_i915_gem_object_unbind(struct drm_i915_gem_object *obj)
{
	CTR1(KTR_DRM, "object_unbind %p", obj);
}

static inline void
trace_i915_gem_object_clflush(struct drm_i915_gem_object *obj)
{
	CTR1(KTR_DRM, "object_clflush %p", obj);
}

static inline void
trace_i915_gem_object_bind(struct drm_i915_gem_object *obj, bool map_and_fenceable)
{
	CTR3(KTR_DRM, "object_bind %p %x %d", obj,
		obj->base.size, map_and_fenceable);
}

static inline void
trace_i915_gem_object_destroy(struct drm_i915_gem_object *obj)
{
	CTR1(KTR_DRM, "object_destroy_tail %p", obj);
}

#define trace_i915_gem_evict(vm, min_size, alignment, flags) \
CTR4(KTR_DRM, "evict_something %p %d %u %x", vm, min_size, alignment, flags)

static inline void
trace_i915_gem_evict_vm(struct i915_address_space *vm)
{
	CTR1(KTR_DRM, "evict_vm %p", vm);
}

static inline void
trace_i915_gem_evict_everything(struct drm_device *dev){
	CTR1(KTR_DRM, "evict_everything %p", dev);
}

static inline void
trace_i915_gem_evict_node(struct i915_address_space *vm,
						  struct drm_mm_node *target,
						  unsigned int flags) {
	CTR3(KTR_DRM, "evict_node vm %p, target %p, flags %u", vm, target, flags);
}

static inline void
trace_i915_gem_object_fault(void *obj, off_t off, int bit, int write)
{
	CTR4(KTR_DRM, "gem_object_fault obj %p off %zd bit %d write: %d", obj, off, bit, write);
}

static inline void
trace_i915_gem_shrink(void *dev, int target, int flags)
{
	CTR3(KTR_DRM, "gem_shrink %p %d %x", dev, target, flags);
}

static inline void
trace_switch_mm(void *ring, void *to) {
	CTR2(KTR_DRM, "switch_mm ring %p to %p", ring, to);
}

#define	trace_i915_context(ctx) \
    CTR4(KTR_DRM, \
	"%s dev=%u, ctx=%p, ctx_vm=%p", \
	__func__, \
	ctx->i915->drm.primary->index, \
	ctx, \
	rcu_access_pointer(ctx->vm))

static inline void
trace_i915_context_create(struct i915_gem_context *ctx)
{
	trace_i915_context(ctx);
}

static inline void
trace_i915_context_free(struct i915_gem_context *ctx)
{
	trace_i915_context(ctx);
}

#define	trace_intel_context(ctx) \
    CTR5(KTR_DRM, \
	"%s guc_id=%d, pin_count=%d sched_state=0x%x, guc_prio=%u", \
	__func__, \
	ctx->guc_id.id, \
	atomic_read(&ctx->pin_count), \
	ctx->guc_state.sched_state, \
	ctx->guc_state.prio)

static inline void
trace_intel_context_set_prio(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_reset(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_ban(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_register(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_deregister(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_deregister_done(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_sched_enable(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_sched_disable(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_sched_done(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_create(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_fence_release(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_free(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_steal_guc_id(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_do_pin(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_intel_context_do_unpin(struct intel_context *ctx)
{
	trace_intel_context(ctx);
}

static inline void
trace_i915_request_wait_begin(void *req, uint32_t flags) {
	CTR2(KTR_DRM, "request_wait_begin req %p flags %x", req, flags);
}

static inline void
trace_i915_request_wait_end(void *req) {
	CTR1(KTR_DRM, "request_wait_end req %p", req);
}

static inline void
trace_i915_request_retire(void *req) {
	CTR1(KTR_DRM, "request_retire req %p", req);
}

static inline void
trace_i915_request_notify(void *req) {
	CTR1(KTR_DRM, "request_notify req %p", req);
}

static inline void
trace_i915_request_execute(void *req) {
	CTR1(KTR_DRM, "request_execute req %p", req);
}

static inline void
trace_i915_request_submit(void *req) {
	CTR1(KTR_DRM, "request_submit req %p", req);
}

static inline void
trace_i915_request_queue(struct i915_request *req, uint32_t flags) {
	CTR2(KTR_DRM, "request_queue req %p flags %x", req, flags);
}

static inline void
trace_i915_request_in(struct i915_request *req, uint32_t flags) {
	CTR2(KTR_DRM, "request_in req %p flags %x", req, flags);
}

static inline void
trace_i915_request_out(struct i915_request *req) {
	CTR1(KTR_DRM, "request_out req %p", req);
}

static inline void
trace_i915_page_table_entry_alloc(void *vm, uint32_t pde, uint64_t start, int shift)
{
	CTR4(KTR_DRM, "page_table_entry_alloc vm %p pde %x start %zx shift %x", vm, pde, start, shift);
}

static inline void
trace_i915_page_directory_entry_alloc(void *vm, uint32_t pdpe, uint64_t start, int shift)
{
	CTR4(KTR_DRM, "page_table_directory_entry_alloc vm %p pdpe %x start %zx shift %x", vm, pdpe, start, shift);
}

static inline void
trace_i915_page_directory_pointer_entry_alloc(void *vm, uint32_t pml4e, uint64_t start, int shift)
{
	CTR4(KTR_DRM, "page_table_directory_pointer_entry_alloc vm %p pml4e %x start %zx shift %x", vm, pml4e, start, shift);
}

static inline void
trace_i915_page_table_entry_map(void *base, uint32_t pde, void *pt, int index, int count, uint32_t flags)
{
	CTR6(KTR_DRM, "page_table_entry_map base %p pde %x pt %p index %x count %d flags %x", base, pde, pt, index, count, flags);
}

static inline void
trace_i915_pipe_update_start(void *crtc)
{
	CTR1(KTR_DRM, "pipe_update_start %p", crtc);
}

static inline void
trace_i915_pipe_update_vblank_evaded(void *crtc)
{
	CTR1(KTR_DRM, "pipe_update_vblank_evaded %p", crtc);
}

static inline void
trace_i915_pipe_update_end(void *crtc, u32 end_vbl_count, int scanline_end)
{
	CTR3(KTR_DRM, "pipe_update_end %p end_vbl_count %d scanline_end %d", crtc, end_vbl_count, scanline_end);
}

#define	trace_i915_request(req) \
do { \
    CTR5(KTR_DRM, \
	"%s dev=%u, engine=%u:%u, ctx=%llu", \
	__func__, \
	req->engine->i915->drm.primary->index, \
	req->engine->uabi_class, \
	req->engine->uabi_instance, \
	req->fence.context); \
    CTR2(KTR_DRM, \
	"seqno=%u, tail=%u", \
	req->fence.seqno, \
	req->tail); \
} while(0)

static inline void
trace_i915_request_add(struct i915_request *req)
{
	trace_i915_request(req);
}

static inline void
trace_i915_request_guc_submit(struct i915_request *req)
{
	trace_i915_request(req);
}

#define trace_i915_gem_ring_sync_to(to_req, from) \
CTR2(KTR_DRM, "gem_ring_sync_to  to_req %p from %p", to_req, from);


static inline void
trace_i915_vma_bind(void *vma, uint32_t flags)
{
	CTR2(KTR_DRM, "vma_bind vma %p flags %x", vma, flags);
}


static inline void
trace_i915_va_alloc(void *vma)
{
	CTR1(KTR_DRM, "va_alloc vma %p", vma);
}

static inline void
trace_i915_vma_unbind(void *vma)
{
	CTR1(KTR_DRM, "vma_bind vma %p", vma);
}

static inline void
trace_i915_ppgtt_create(void *base)
{

	CTR1(KTR_DRM, "ppgtt_create %p", base);
}

static inline void
trace_i915_ppgtt_release(void *base)
{

	CTR1(KTR_DRM, "ppgtt_release %p", base);
}

static inline void
trace_i915_reg_rw(boolean_t rw, i915_reg_t reg, uint64_t val, int sz, bool trace)
{

        CTR4(KTR_DRM_REG, "[%x/%d] %c %x", reg.reg, sz, rw ? "w" : "r", val);
}

static inline void
trace_intel_gpu_freq_change(uint32_t freq)
{
	CTR1(KTR_DRM, "gpu_freq_change %x", freq);
}

static inline void
trace_intel_engine_notify(struct intel_engine_cs *engine, bool waiters)
{
	CTR2(KTR_DRM, "engine_notify engine %p waiters %x", engine, waiters);
}

#endif /* _I915_TRACE_H_ */
