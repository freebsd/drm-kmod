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

#include <sys/param.h>

#include <linux/dma-fence-chain.h>

static bool dma_fence_chain_enable_signaling(struct dma_fence *fence);

MALLOC_DECLARE(M_DMABUF);

static const char *
dma_fence_chain_get_driver_name(struct dma_fence *fence)
{
        return ("dma_fence_chain");
}

static const char *
dma_fence_chain_get_timeline_name(struct dma_fence *fence)
{

        return ("unbound");
}

static void
dma_fence_chain_irq_work(struct irq_work *work)
{
	struct dma_fence_chain *chain;

	chain = container_of(work, typeof(*chain), work);
	if (dma_fence_chain_enable_signaling(&chain->base) == false)
		dma_fence_signal(&chain->base);
	dma_fence_put(&chain->base);
}

static void
dma_fence_chain_cb(struct dma_fence *fence, struct dma_fence_cb *cb)
{
	struct dma_fence_chain *chain;

	chain = container_of(cb, typeof(*chain), cb);
	init_irq_work(&chain->work, dma_fence_chain_irq_work);
	irq_work_queue(&chain->work);
	dma_fence_put(fence);
}

static bool
dma_fence_chain_enable_signaling(struct dma_fence *fence)
{
	struct dma_fence_chain *chain, *c;
	struct dma_fence *f;

	chain = to_dma_fence_chain(fence);
	dma_fence_get(&chain->base);
	dma_fence_chain_for_each(fence, &chain->base) {
		if ((c = to_dma_fence_chain(fence)) == NULL)
			f = fence;
		else
			f = c->fence;
		dma_fence_get(f);
		if (dma_fence_add_callback(f, &chain->cb,
		    dma_fence_chain_cb) == false) {
			dma_fence_put(fence);
			return (true);
		}
		dma_fence_put(f);
	}
	dma_fence_put(&chain->base);
	return (false);
}

static bool
dma_fence_chain_signaled(struct dma_fence *fence)
{
	struct dma_fence_chain *chain;
	struct dma_fence *f;

	dma_fence_chain_for_each(fence, fence) {
		if ((chain = to_dma_fence_chain(fence)) == NULL)
			f = fence;
		else
			f = chain->fence;
		if (dma_fence_is_signaled(f) == false) {
			dma_fence_put(fence);
			return (false);
		}
	}
	return (true);
}

static void
dma_fence_chain_release(struct dma_fence *fence)
{
	struct dma_fence_chain *chain, *prev_chain;
	struct dma_fence *prev;
	
	chain = to_dma_fence_chain(fence);
	while ((prev = rcu_dereference_protected(chain->prev, true)) != NULL) {
		if (kref_read(&prev->refcount) > 1)
			break;
		if ((prev_chain = to_dma_fence_chain(prev)) == NULL)
			break;
		chain->prev = prev_chain->prev;
		RCU_INIT_POINTER(prev_chain->prev, NULL);
		dma_fence_put(prev);
	}
	dma_fence_put(prev);
	dma_fence_put(chain->fence);
	dma_fence_free(fence);
}


static void
dma_fence_chain_set_deadline(struct dma_fence *fence, ktime_t deadline)
{
	struct dma_fence_chain *chain;
	struct dma_fence *f;

	dma_fence_chain_for_each(fence, fence) {
		if ((chain = to_dma_fence_chain(fence)) == NULL)
			f = fence;
		else
			f = chain->fence;
		dma_fence_set_deadline(f, deadline);
	}
}

const struct dma_fence_ops dma_fence_chain_ops = {
	.use_64bit_seqno = true,
	.get_driver_name = dma_fence_chain_get_driver_name,
	.get_timeline_name = dma_fence_chain_get_timeline_name,
	.enable_signaling = dma_fence_chain_enable_signaling,
	.signaled = dma_fence_chain_signaled,
	.release = dma_fence_chain_release,
	.set_deadline = dma_fence_chain_set_deadline,
};

bool
dma_fence_is_chain(struct dma_fence *fence)
{
	return fence->ops == &dma_fence_chain_ops;
}

void
dma_fence_chain_init(struct dma_fence_chain *chain,
			  struct dma_fence *prev,
			  struct dma_fence *fence,
			  uint64_t seqno)
{
	struct dma_fence_chain *prev_chain;
	uint64_t context;

	spin_lock_init(&chain->lock);
	chain->fence = fence;
	rcu_assign_pointer(chain->prev, prev);
	prev_chain = to_dma_fence_chain(prev);
	if (prev_chain != NULL &&
	    __dma_fence_is_later(seqno, prev->seqno, prev->ops)) {
		chain->prev_seqno = prev->seqno;
		context = prev->context;
	} else {
		if (prev_chain != NULL)
			seqno = max(prev->seqno, seqno);
		context = dma_fence_context_alloc(1);
	}

	dma_fence_init(&chain->base, &dma_fence_chain_ops,
	    &chain->lock, context, seqno);
}

int
dma_fence_chain_find_seqno(struct dma_fence **fence, uint64_t seqno)
{
	struct dma_fence_chain *chain;

	if (seqno == 0)
		return (0);
	if ((chain = to_dma_fence_chain(*fence)) == NULL)
		return (-EINVAL);
	if (chain->base.seqno < seqno)
		return (-EINVAL);
	dma_fence_chain_for_each(*fence, &chain->base) {
		if ((*fence)->context != chain->base.context)
			break;
		if (to_dma_fence_chain(*fence)->prev_seqno < seqno)
			break;
	}
	dma_fence_put(&chain->base);
	return (0);
}

static struct dma_fence *
dma_fence_chain_get_prev(struct dma_fence_chain *chain)
{
	struct dma_fence *prev;

	rcu_read_lock();
	prev = dma_fence_get_rcu_safe(&chain->prev);
	rcu_read_unlock();

	return (prev);
}

struct dma_fence *
dma_fence_chain_walk(struct dma_fence *fence)
{
	struct dma_fence_chain *chain, *prev_chain;
	struct dma_fence *prev, *new_prev, *tmp;

	if ((chain = to_dma_fence_chain(fence)) == NULL) {
		dma_fence_put(fence);
		return (NULL);
	}

	while ((prev = dma_fence_chain_get_prev(chain)) != NULL) {
		if ((prev_chain = to_dma_fence_chain(prev)) != NULL) {
			if (dma_fence_is_signaled(prev_chain->fence) == false)
				break;
			new_prev = dma_fence_chain_get_prev(prev_chain);
		} else {
			if (dma_fence_is_signaled(prev) == false)
				break;
			new_prev = NULL;
		}
		tmp = cmpxchg((struct dma_fence **)&chain->prev,
			      prev, new_prev);
		if (tmp == prev)
			dma_fence_put(tmp);
		else
			dma_fence_put(new_prev);
		dma_fence_put(prev);
	}

	dma_fence_put(fence);
	return (prev);
}

struct dma_fence_chain *
to_dma_fence_chain(struct dma_fence *fence)
{

	if (fence == NULL || !dma_fence_is_chain(fence))
		return (NULL);

	return (container_of(fence, struct dma_fence_chain, base));
}
