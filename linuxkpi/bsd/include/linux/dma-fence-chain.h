/*-
 * Copyright (c) 2022 Beckhoff Automation GmbH & Co. KG
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
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef _LINUX_DMA_FENCE_CHAIN_H_
#define _LINUX_DMA_FENCE_CHAIN_H_

#include <linux/dma-fence.h>
#include <linux/irq_work.h>

struct dma_fence_chain {
	struct dma_fence base;
	struct dma_fence __rcu *prev;
	u64 prev_seqno;
	struct dma_fence *fence;
	union {
		struct dma_fence_cb cb;
		struct irq_work work;
	};
	spinlock_t lock;
};

#define dma_fence_chain_for_each(iter, head)	\
	for (iter = dma_fence_get(head); iter; \
	     iter = dma_fence_chain_walk(iter))

struct dma_fence_chain *to_dma_fence_chain(struct dma_fence *fence);
struct dma_fence *dma_fence_chain_walk(struct dma_fence *fence);
int dma_fence_chain_find_seqno(struct dma_fence **fence, uint64_t seqno);
void dma_fence_chain_init(struct dma_fence_chain *chain, struct dma_fence *prev,
  struct dma_fence *fence, uint64_t seqno);

static inline struct dma_fence *
dma_fence_chain_contained(struct dma_fence *fence)
{
	struct dma_fence_chain *chain = to_dma_fence_chain(fence);
	return (chain != NULL ? chain->fence : fence);
}

MALLOC_DECLARE(M_DMABUF);

static inline struct dma_fence_chain *
dma_fence_chain_alloc(void)
{
	return (kmalloc(sizeof(struct dma_fence_chain), GFP_KERNEL));
}

static inline void
dma_fence_chain_free(struct dma_fence_chain *chain)
{
	kfree(chain);
}

#endif /* _LINUX_DMA_FENCE_CHAIN_H_ */
