#ifndef _TRACE_DMA_FENCE_H_
#define _TRACE_DMA_FENCE_H_

#include <linux/dma-fence.h>
#ifndef KTR_DRM
#define	KTR_DRM	KTR_DEV
#endif

static inline void
trace_dma_fence_init(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_init dma_fence %p", fence);
}

static inline void
trace_dma_fence_destroy(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_destroy dma_fence %p", fence);
}

static inline void
trace_dma_fence_enable_signal(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_enable_signal dma_fence %p", fence);
}

static inline void
trace_dma_fence_signaled(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_signaled dma_fence %p", fence);
}

static inline void
trace_dma_fence_wait_start(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_wait_start dma_fence %p", fence);
}

static inline void
trace_dma_fence_wait_end(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_wait_end dma_fence %p", fence);
}

#endif
