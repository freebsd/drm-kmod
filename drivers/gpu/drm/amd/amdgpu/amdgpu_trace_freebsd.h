#if !defined(_AMDGPU_TRACE_FREEBSD_H_) || defined(TRACE_HEADER_MULTI_READ)
#define _AMDGPU_TRACE_FREEBSD_H_

#include <sys/param.h>
#include <sys/ktr.h>

#include "amdgpu.h"

static inline void
trace_amdgpu_iv(u_long ih __unused, struct amdgpu_iv_entry *iv){
	CTR1(KTR_DRM, "amdgpu_iv %p", iv);
}

static inline void
trace_amdgpu_cs_ioctl(struct amdgpu_job *job){
	CTR1(KTR_DRM, "amdgpu_cs_ioctl %p", job);
}

static inline void
trace_amdgpu_cs(struct amdgpu_cs_parser *parser, struct amdgpu_job *job,
    struct amdgpu_ib *ib){
	CTR3(KTR_DRM, "amdgpu_cs %p %p %p", parser, job, ib);
}

static inline void
trace_amdgpu_sched_run_job(struct amdgpu_job *job){
	CTR1(KTR_DRM, "amdgpu_sched_run_job %p", job);
}

static inline void
trace_amdgpu_bo_create(void *bo)
{
	CTR1(KTR_DRM, "amdgpu_bo_create %p", bo);
}

static inline void
trace_amdgpu_bo_move(struct amdgpu_bo* bo, uint32_t new, uint32_t old)
{
	CTR3(KTR_DRM, "amdgpu_bo_move %p %u %u", bo, new, old);
}

static inline void
trace_amdgpu_bo_list_set(void *a, void *b) {
	CTR2(KTR_DRM, "amdgpu_bo_list_set %p %p", a, b);
}

static inline void
trace_amdgpu_pasid_allocated(unsigned pasid) {
	CTR1(KTR_DRM, "amdgpu_pasid_allocated %u", pasid);	
}

static inline void
trace_amdgpu_pasid_freed(unsigned pasid) {
	CTR1(KTR_DRM, "amdgpu_pasid_freed %u", pasid);	
}

#define	trace_amdgpu_vm_grab_id(vm, idx, job) \
        CTR3(KTR_DRM, "amdgpu_vm_grab_id %p %u %p", (vm), (idx), (job));

static inline void
trace_amdgpu_vm_flush(struct amdgpu_ring *ring, unsigned vm_id, uint64_t pd_addr){
	CTR3(KTR_DRM, "amdgpu_vm_flush ring %p, vm_id %u, pd_addr %u", ring, vm_id, pd_addr);
}

static inline void
trace_amdgpu_vm_bo_map(struct amdgpu_bo_va * bo_va, struct amdgpu_bo_va_mapping * mapping){
	CTR2(KTR_DRM, "amdgpu_vm_bo_unmap %p %p", bo_va, mapping);
}

static inline void
trace_amdgpu_vm_bo_unmap(struct amdgpu_bo_va * bo_va, struct amdgpu_bo_va_mapping * mapping){
	CTR2(KTR_DRM, "amdgpu_vm_bo_unmap %p %p", bo_va, mapping);
}

static inline void
trace_amdgpu_vm_bo_update(struct amdgpu_bo_va_mapping * mapping){
	CTR1(KTR_DRM, "amdgpu_vm_bo_update %p", mapping);
}

static inline void
trace_amdgpu_vm_set_page(uint64_t pe, uint64_t addr, unsigned count, uint32_t incr, uint32_t flags){
	CTR5(KTR_DRM, "amdgpu_vm_set_page %x %x %u %u %x", pe, addr, count, incr, flags);
}

static inline void
trace_amdgpu_vm_bo_cs(struct amdgpu_bo_va_mapping * mapping){
	CTR1(KTR_DRM, "amdgpu_vm_bo_cs %p", mapping);
}

static inline int
trace_amdgpu_vm_bo_cs_enabled(void){
	CTR0(KTR_DRM, "amdgpu_vm_bo_cs_enabled");
	return (0);
}

static inline int
trace_amdgpu_vm_bo_mapping_enabled(void){
	CTR0(KTR_DRM, "amdgpu_vm_bo_mapping_enabled");
	return (0);
}

static inline void
trace_amdgpu_vm_bo_mapping(struct amdgpu_bo_va_mapping * mapping){
	CTR1(KTR_DRM, "amdgpu_vm_bo_mapping %p", mapping);
}

static inline int
trace_amdgpu_ttm_tt_populate_enabled(void){
	CTR0(KTR_DRM, "trace_amdgpu_ttm_tt_populate_enabled");
	return (0);
}

static inline void
trace_amdgpu_ttm_tt_populate(struct amdgpu_device *adev, uint64_t dma_address,
    uint64_t phys_address){
	CTR3(KTR_DRM, "amdgpu_device %p, dma_address %lu, phys_addr %lu",
	    adev, dma_address, phys_address);
}

static inline int
trace_amdgpu_ttm_tt_unpopulate_enabled(void){
	CTR0(KTR_DRM, "trace_amdgpu_ttm_tt_unpopulate_enabled");
	return (0);
}

static inline void
trace_amdgpu_ttm_tt_unpopulate(struct amdgpu_device *adev, uint64_t dma_address,
    uint64_t phys_address){
	CTR3(KTR_DRM, "amdgpu_device %p, dma_address %lu, phys_addr %lu",
	    adev, dma_address, phys_address);
}

static inline void
trace_amdgpu_ib_pipe_sync(struct amdgpu_job *job, struct dma_fence *fence){
	CTR2(KTR_DRM, "amdgpu_ib_pipe_sync %p, fence %p", job, fence);
}

static inline void
trace_amdgpu_reset_reg_dumps(uint32_t address, uint32_t value){
	CTR2(KTR_DRM, "amdgpu_reset_reg_dumps address %x, value %x",
	    address, value);
}

static inline void
trace_amdgpu_runpm_reference_dumps(uint32_t index, const char *func){
	CTR2(KTR_DRM, "amdgpu runpm reference dump 0x%x: 0x%s\n",
	    index, func);
}

#define trace_amdgpu_mm_rreg(dev, reg, ret)	\
	CTR3(KTR_DRM, "amdgpu_mm_rreg %p %x %d", (dev), (reg), (ret))

#define trace_amdgpu_mm_wreg(dev, reg, x)	\
	CTR3(KTR_DRM, "amdgpu_mm_wreg %p %x %x", (dev), (reg), (x))

#define trace_amdgpu_cs_bo_status(entries, size)	\
	CTR2(KTR_DRM, "amdgpu_cs_bo_status entries %d size %d", (entries), (size))

#define trace_amdgpu_ttm_bo_move(bo, new, old)	\
	do {					\
		u32 old_mem_type;	\
		old_mem_type = (old);					\
		CTR3(KTR_DRM, "amdgpu_ttm_bo_move %p new %d old %d", (bo), (new), (old_mem_type)); \
	} while (0)

static inline void
trace_amdgpu_vm_update_ptes(struct amdgpu_vm_update_params *p, uint64_t start,
    uint64_t end, unsigned int nptes, uint64_t dst, uint64_t incr,
    uint64_t flags, pid_t pid, uint64_t vm_ctx)
{
	CTR6(KTR_DRM, "amdgpu_vm_update_ptes pid:%u vm_ctx:0x%llx "
	    "start:0x%010llx end:0x%010llx flags:0x%llx incr:%llu",
	    pid, vm_ctx, start, end, flags, incr);
#ifdef __FreeBSD_not_yet
	for (int i = 0; i < nptes; ++i) {
		uint64_t addr = p->pages_addr ? amdgpu_vm_map_gart(
		    p->pages_addr, dst) : dst;
		((u64 *)__get_dynamic_array(dst))[i] = addr;
		dst += incr;
	}
	CTR1(KTR_DRM, "dst:\n%s", __print_array(__get_dynamic_array(dst), nptes, 8));
#endif
}

#define trace_amdgpu_vm_set_ptes(pe, addr, count, incr, flags, direct)		\
	CTR5(KTR_DRM, "amdgpu_vm_set_ptes %lx %lx %u %u %x", pe, addr, count, incr, flags)

#define trace_amdgpu_vm_copy_ptes(pe, src, count, direct)			\
	CTR3(KTR_DRM, "amdgpu_vm_copy_ptes %lx %lx %u", pe, src, count)

#define	trace_amdgpu_device_rreg(dev, reg, ret)
#define	trace_amdgpu_device_wreg(dev, reg, value)

#endif

