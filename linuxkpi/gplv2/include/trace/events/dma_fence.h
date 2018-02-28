#ifndef _TRACE_DMA_FENCE_H_
#define _TRACE_DMA_FENCE_H_

#include <linux/dma-fence.h>

/* trace_dma_fence_enable_signal(&rq->fence); */
static inline void
trace_dma_fence_enable_signal(struct dma_fence *fence)
{
	CTR1(KTR_DRM, "dma_fence_enable_signal dma_fence %p", fence);
}


#endif
