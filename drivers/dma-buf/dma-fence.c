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

#include <linux/dma-fence.h>

MALLOC_DECLARE(M_DMABUF);

static struct dma_fence dma_fence_stub;
static DEFINE_SPINLOCK(dma_fence_stub_lock);

static const char *
dma_fence_stub_get_name(struct dma_fence *fence)
{

	return ("stub");
}

static const struct dma_fence_ops dma_fence_stub_ops = {
	.get_driver_name = dma_fence_stub_get_name,
	.get_timeline_name = dma_fence_stub_get_name,
};

/*
 * return a signaled fence
 */
struct dma_fence *
dma_fence_get_stub(void)
{

	spin_lock(&dma_fence_stub_lock);
	if (dma_fence_stub.ops == NULL) {
		dma_fence_init(&dma_fence_stub,
		    &dma_fence_stub_ops,
		    &dma_fence_stub_lock,
		    0,
		    0);
		dma_fence_signal_locked(&dma_fence_stub);
	}
	spin_unlock(&dma_fence_stub_lock);
	return (dma_fence_get(&dma_fence_stub));
}

static atomic64_t dma_fence_context_counter = ATOMIC64_INIT(1);

/*
 * allocate an array of fence contexts
 */
u64
dma_fence_context_alloc(unsigned num)
{

	return (atomic64_fetch_add(num, &dma_fence_context_counter));
}

/*
 * signal completion of a fence
 */
int
dma_fence_signal_timestamp_locked(struct dma_fence *fence,
				  ktime_t timestamp)
{
	struct dma_fence_cb *cur, *tmp;
	struct list_head cb_list;

	if (fence == NULL)
		return (-EINVAL);
	if (test_and_set_bit(DMA_FENCE_FLAG_SIGNALED_BIT,
	      &fence->flags))
		return (-EINVAL);

	list_replace(&fence->cb_list, &cb_list);

	fence->timestamp = timestamp;
	set_bit(DMA_FENCE_FLAG_TIMESTAMP_BIT, &fence->flags);

	list_for_each_entry_safe(cur, tmp, &cb_list, node) {
		INIT_LIST_HEAD(&cur->node);
		cur->func(fence, cur);
	}

	return (0);
}

/*
 * signal completion of a fence
 */
int
dma_fence_signal_timestamp(struct dma_fence *fence, ktime_t timestamp)
{
	int rv;

	if (fence == NULL)
		return (-EINVAL);

	spin_lock(fence->lock);
	rv = dma_fence_signal_timestamp_locked(fence, timestamp);
	spin_unlock(fence->lock);
	return (rv);
}

/*
 * signal completion of a fence
 */
int
dma_fence_signal_locked(struct dma_fence *fence)
{
	return dma_fence_signal_timestamp_locked(fence, ktime_get());
}

/*
 * signal completion of a fence
 */
int
dma_fence_signal(struct dma_fence *fence)
{
	int rv;

	if (fence == NULL)
		return (-EINVAL);

	spin_lock(fence->lock);
	rv = dma_fence_signal_timestamp_locked(fence, ktime_get());
	spin_unlock(fence->lock);
	return (rv);
}

/*
 * sleep until the fence gets signaled or until timeout elapses
 */
signed long
dma_fence_wait_timeout(struct dma_fence *fence, bool intr, signed long timeout)
{
	int rv;

	if (fence == NULL)
		return (-EINVAL);

	if (fence->ops && fence->ops->wait != NULL)
		rv = fence->ops->wait(fence, intr, timeout);
	else
		rv = dma_fence_default_wait(fence, intr, timeout);
	return (rv);
}

/*
 * default relese function for fences
 */
void
dma_fence_release(struct kref *kref)
{
	struct dma_fence *fence;

	fence = container_of(kref, struct dma_fence, refcount);
	if (fence->ops && fence->ops->release)
		fence->ops->release(fence);
	else
		dma_fence_free(fence);
}

/*
 * default release function for dma_fence
 */
void
dma_fence_free(struct dma_fence *fence)
{

	kfree_rcu(fence, rcu);
}

/*
 * enable signaling on fence
 */
void
dma_fence_enable_sw_signaling(struct dma_fence *fence)
{
	bool was_enabled;

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return;
	spin_lock(fence->lock);
	was_enabled = test_and_set_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT,
	    &fence->flags);
	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		goto out;
	if (was_enabled == false &&
	    fence->ops && fence->ops->enable_signaling) {
		if (fence->ops->enable_signaling(fence) == false)
			dma_fence_signal_locked(fence);
	}
out:
	spin_unlock(fence->lock);
}

/*
 * add a callback to be called when the fence is signaled
 */
int
dma_fence_add_callback(struct dma_fence *fence, struct dma_fence_cb *cb,
			   dma_fence_func_t func)
{
	int rv = 0;
	bool was_enabled;

	if (fence == NULL || func == NULL)
		return (-EINVAL);

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		INIT_LIST_HEAD(&cb->node);
		return (-ENOENT);
	}

	spin_lock(fence->lock);
	was_enabled = test_and_set_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT,
	    &fence->flags);

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		rv = -ENOENT;
	else if (was_enabled == false && fence->ops
	    && fence->ops->enable_signaling) {
		if (!fence->ops->enable_signaling(fence)) {
			dma_fence_signal_locked(fence);
			rv = -ENOENT;
		}
	}

	if (!rv) {
		cb->func = func;
		list_add_tail(&cb->node, &fence->cb_list);
	} else
		INIT_LIST_HEAD(&cb->node);
	spin_unlock(fence->lock);

	return (rv);
}

/*
 * returns the status upon completion
 */
int
dma_fence_get_status(struct dma_fence *fence)
{
	int rv;

	spin_lock(fence->lock);
	rv = dma_fence_get_status_locked(fence);
	spin_unlock(fence->lock);
	return (rv);
}

/*
 * remove a callback from the signaling list
 */
bool
dma_fence_remove_callback(struct dma_fence *fence, struct dma_fence_cb *cb)
{
	int rv;

	spin_lock(fence->lock);
	rv = !list_empty(&cb->node);
	if (rv)
		list_del_init(&cb->node);
	spin_unlock(fence->lock);
	return (rv);
}


struct default_wait_cb {
	struct dma_fence_cb base;
	struct task_struct *task;
};

static void
dma_fence_default_wait_cb(struct dma_fence *fence, struct dma_fence_cb *cb)
{
	struct default_wait_cb *wait =
		container_of(cb, struct default_wait_cb, base);

	wake_up_state(wait->task, TASK_NORMAL);
}

/*
 * default sleep until the fence gets signaled or until timeout elapses
 */
signed long
dma_fence_default_wait(struct dma_fence *fence, bool intr, signed long timeout)
{
	struct default_wait_cb cb;
	signed long rv = timeout ? timeout : 1;
	bool was_enabled;

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return (rv);

	spin_lock(fence->lock);

	was_enabled = test_and_set_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT,
	    &fence->flags);

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		goto out;

	if (was_enabled == false && fence->ops &&
	    fence->ops->enable_signaling) {
		if (!fence->ops->enable_signaling(fence)) {
			dma_fence_signal_locked(fence);
			goto out;
		}
	}

	if (timeout == 0) {
		rv = 0;
		goto out;
	}

	cb.base.func = dma_fence_default_wait_cb;
	cb.task = current;
	list_add(&cb.base.node, &fence->cb_list);

	while (!test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags) && rv > 0) {
		if (intr)
			__set_current_state(TASK_INTERRUPTIBLE);
		else
			__set_current_state(TASK_UNINTERRUPTIBLE);
		spin_unlock(fence->lock);

		rv = schedule_timeout(rv);

		spin_lock(fence->lock);
		if (rv > 0 && intr && signal_pending(current))
			rv = -ERESTARTSYS;
	}

	if (!list_empty(&cb.base.node))
		list_del(&cb.base.node);
	__set_current_state(TASK_RUNNING);
out:
	spin_unlock(fence->lock);
	return (rv);
}

static bool
dma_fence_test_signaled_any(struct dma_fence **fences, uint32_t count,
    uint32_t *idx)
{
	int i;

	for (i = 0; i < count; ++i) {
		struct dma_fence *fence = fences[i];
		if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
			if (idx)
				*idx = i;
			return true;
		}
	}
	return false;
}

/*
 * sleep until any fence gets signaled or until timeout elapses
 */
signed long
dma_fence_wait_any_timeout(struct dma_fence **fences, uint32_t count,
			   bool intr, signed long timeout, uint32_t *idx)
{
	struct default_wait_cb *cb;
	long rv = timeout;
	int i;

	if (timeout == 0) {
		for (i = 0; i < count; i++) {
			if (dma_fence_is_signaled(fences[i])) {
				if (idx)
					*idx = i;
				return (1);
			}
		}
		return (0);
	}

	cb = malloc(sizeof(*cb), M_DMABUF, M_WAITOK | M_ZERO);
	for (i = 0; i < count; i++) {
		struct dma_fence *fence = fences[i];
		cb[i].task = current;
		if (dma_fence_add_callback(fence, &cb[i].base,
		    dma_fence_default_wait_cb)) {
			if (idx)
				*idx = i;
			goto cb_cleanup;
		}
	}

	while (rv > 0) {
		if (intr)
			set_current_state(TASK_INTERRUPTIBLE);
		else
			set_current_state(TASK_UNINTERRUPTIBLE);

		if (dma_fence_test_signaled_any(fences, count, idx))
			break;

		rv = schedule_timeout(rv);

		if (rv > 0 && intr && signal_pending(current))
			rv = -ERESTARTSYS;
	}

	__set_current_state(TASK_RUNNING);

cb_cleanup:
	while (i-- > 0)
		dma_fence_remove_callback(fences[i], &cb[i].base);
	free(cb, M_DMABUF);
	return (rv);
}

/*
 * Initialize a custom fence.
 */
void
dma_fence_init(struct dma_fence *fence, const struct dma_fence_ops *ops,
    spinlock_t *lock, u64 context, u64 seqno)
{

	kref_init(&fence->refcount);
	INIT_LIST_HEAD(&fence->cb_list);
	fence->ops = ops;
	fence->lock = lock;
	fence->context = context;
	fence->seqno = seqno;
	fence->flags = 0;
	fence->error = 0;
}

/*
 * decreases refcount of the fence
 */
void
dma_fence_put(struct dma_fence *fence)
{

	if (fence)
		kref_put(&fence->refcount, dma_fence_release);
}

/* 
 * increases refcount of the fence
 */
struct dma_fence *
dma_fence_get(struct dma_fence *fence)
{

	if (fence)
		kref_get(&fence->refcount);
	return (fence);
}

/*
 * get a fence from a dma_resv_list with rcu read lock
 */
struct dma_fence *
dma_fence_get_rcu(struct dma_fence *fence)
{

	if (kref_get_unless_zero(&fence->refcount))
		return (fence);
	else
		return (NULL);
}

/*
 * acquire a reference to an RCU tracked fence
 */
struct dma_fence *
dma_fence_get_rcu_safe(struct dma_fence __rcu **fencep)
{

	do {
		struct dma_fence *fence;

		fence = rcu_dereference(*fencep);
		if (!fence)
			return (NULL);

		if (!dma_fence_get_rcu(fence))
			continue;

		if (fence == rcu_access_pointer(*fencep))
			return rcu_pointer_handoff(fence);

		dma_fence_put(fence);
	} while (1);
}

/*
 * Return an indication if the fence is signaled yet.
 */
bool
dma_fence_is_signaled_locked(struct dma_fence *fence)
{

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return (true);

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		dma_fence_signal_locked(fence);
		return (true);
	}

	return (false);
}

/*
 * Return an indication if the fence is signaled yet.
 */
bool
dma_fence_is_signaled(struct dma_fence *fence)
{

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return (true);

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		dma_fence_signal(fence);
		return (true);
	}

	return (false);
}

/*
 * return if f1 is chronologically later than f2
 */
bool
__dma_fence_is_later(u64 f1, u64 f2, const struct dma_fence_ops *ops)
{

	if (ops->use_64bit_seqno)
		return (f1 > f2);

	return (int)(lower_32_bits(f1) - lower_32_bits(f2)) > 0;
}

/*
 * return if f1 is chronologically later than f2
 */
bool
dma_fence_is_later(struct dma_fence *f1,
    struct dma_fence *f2)
{

	if (WARN_ON(f1->context != f2->context))
		return (false);

	return (__dma_fence_is_later(f1->seqno, f2->seqno, f1->ops));
}

/*
 * return the chronologically later fence
 */
struct dma_fence *
dma_fence_later(struct dma_fence *f1,
    struct dma_fence *f2)
{
	if (WARN_ON(f1->context != f2->context))
		return (NULL);

	if (dma_fence_is_later(f1, f2))
		return (dma_fence_is_signaled(f1) ? NULL : f1);
	else
		return (dma_fence_is_signaled(f2) ? NULL : f2);
}

/*
 * returns the status upon completion
 */
int
dma_fence_get_status_locked(struct dma_fence *fence)
{

	assert_spin_locked(fence->lock);
	if (dma_fence_is_signaled_locked(fence))
		return (fence->error ?: 1);
	else
		return (0);
}

/*
 * flag an error condition on the fence
 */
void
dma_fence_set_error(struct dma_fence *fence,
    int error)
{

	fence->error = error;
}

/*
 * sleep until the fence gets signaled
 */
signed long
dma_fence_wait(struct dma_fence *fence, bool intr)
{
	signed long ret;

	ret = dma_fence_wait_timeout(fence, intr, MAX_SCHEDULE_TIMEOUT);

	return (ret < 0 ? ret : 0);
}
