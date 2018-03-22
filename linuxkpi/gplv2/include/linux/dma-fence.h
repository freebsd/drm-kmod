/*
 * XXX needs rewrite
 */
#ifndef __LINUX_GPLV2_DMA_FENCE_H
#define __LINUX_GPLV2_DMA_FENCE_H

#include <linux/err.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <linux/kref.h>
#include <linux/kthread.h>
#include <linux/printk.h>
#include <linux/rcupdate.h>
#include <linux/kernel.h>

#include <linux/compat.h>
#include <linux/ktime.h>

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/condvar.h>

struct dma_fence;
struct dma_fence_cb;

typedef void (*dma_fence_func_t)(struct dma_fence *fence, struct dma_fence_cb *cb);

struct dma_fence_cb {
	struct list_head node;
	dma_fence_func_t func;
};

struct default_wait_cb {
	struct dma_fence_cb base;
	struct task_struct *task;
};

struct dma_fence {
	struct kref refcount;
	const struct dma_fence_ops *ops;
	struct rcu_head rcu;
	struct list_head cb_list;
	spinlock_t *lock;
	u64 context;
	unsigned seqno;
	unsigned long flags;
	ktime_t timestamp;
	int status;
	int error;
	struct list_head child_list;
	struct list_head active_list;
};

enum dma_fence_flag_bits {
	DMA_FENCE_FLAG_SIGNALED_BIT,
	DMA_FENCE_FLAG_TIMESTAMP_BIT,
	DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT,
	DMA_FENCE_FLAG_USER_BITS, /* must always be last member */
};

struct dma_fence_ops {
	const char * (*get_driver_name)(struct dma_fence *fence);
	const char * (*get_timeline_name)(struct dma_fence *fence);
	bool (*enable_signaling)(struct dma_fence *fence);
	bool (*signaled)(struct dma_fence *fence);
	signed long (*wait)(struct dma_fence *fence, bool intr, signed long timeout);
	void (*release)(struct dma_fence *fence);

	int (*fill_driver_data)(struct dma_fence *fence, void *data, int size);
	void (*fence_value_str)(struct dma_fence *fence, char *str, int size);
	void (*timeline_value_str)(struct dma_fence *fence, char *str, int size);
};


u64 dma_fence_context_alloc(unsigned num);
int dma_fence_get_status(struct dma_fence *fence);


static inline void
dma_fence_free(struct dma_fence *fence)
{
	kfree_rcu(fence, rcu);
}

static inline void
dma_fence_release(struct kref *kref)
{
	struct dma_fence *fence = container_of(kref, struct dma_fence, refcount);

	BUG_ON(!list_empty(&fence->cb_list));
	if (fence->ops->release)
		fence->ops->release(fence);
	else
		kfree(fence);
}

static inline void
dma_fence_put(struct dma_fence *fence)
{
	if (fence)
		kref_put(&fence->refcount, dma_fence_release);
}

static inline struct dma_fence *
dma_fence_get(struct dma_fence *fence)
{
	if (fence)
		kref_get(&fence->refcount);
	return (fence);
}

static inline struct dma_fence *
dma_fence_get_rcu(struct dma_fence *fence)
{
	if (kref_get_unless_zero(&fence->refcount))
		return (fence);
	else
		return (NULL);
}

static inline struct dma_fence *
dma_fence_get_rcu_safe(struct dma_fence * __rcu *fencep)
{
	do {
		struct dma_fence *fence;

		fence = rcu_dereference(*fencep);
		if (!fence || !dma_fence_get_rcu(fence))
			return NULL;

		if (fence == rcu_access_pointer(*fencep))
			return rcu_pointer_handoff(fence);

		dma_fence_put(fence);
	} while (1);
}

static inline void
dma_fence_signal_locked_sub(struct dma_fence *fence)
{
	struct dma_fence_cb *cur;

	while ((cur = list_first_entry_or_null(&fence->cb_list,
	            struct dma_fence_cb, node)) != NULL) {
		list_del_init(&cur->node);
		spin_unlock(fence->lock);
		cur->func(fence, cur);
		spin_lock(fence->lock);
	}
}

static inline int
dma_fence_signal_locked(struct dma_fence *fence)
{
	int ret = 0;

	if (WARN_ON(!fence))
		return (-EINVAL);

	if (test_and_set_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		ret = (-EINVAL);
	} else {
		fence->timestamp = ktime_get();
		set_bit(DMA_FENCE_FLAG_TIMESTAMP_BIT, &fence->flags);
	}

	dma_fence_signal_locked_sub(fence);
	return (ret);
}

static inline int
dma_fence_signal(struct dma_fence *fence)
{
	if (!fence)
		return (-EINVAL);

	if (test_and_set_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return (-EINVAL);

	fence->timestamp = ktime_get();
	set_bit(DMA_FENCE_FLAG_TIMESTAMP_BIT, &fence->flags);

	if (test_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags)) {
		spin_lock(fence->lock);
		dma_fence_signal_locked_sub(fence);
		spin_unlock(fence->lock);
	}
	return (0);
}

static inline void
dma_fence_enable_sw_signaling(struct dma_fence *fence)
{
	if (!test_and_set_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags)
	    && !test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		spin_lock(fence->lock);
		if (!fence->ops->enable_signaling(fence))
			dma_fence_signal_locked(fence);
		spin_unlock(fence->lock);
	}
}

static inline bool
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

static inline bool
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

static inline int
dma_fence_get_status_locked(struct dma_fence *fence)
{
	if (dma_fence_is_signaled_locked(fence))
		return (fence->error ?: 1);
	else
		return (0);
}

static inline signed long
dma_fence_wait_timeout(struct dma_fence *fence, bool intr, signed long timeout)
{

	/* under FreeBSD jiffies are 32-bit */
	timeout = (int)timeout;

	if (WARN_ON(timeout < 0))
		return (-EINVAL);

	if (timeout == 0)
		return (dma_fence_is_signaled(fence));
	else
		return (fence->ops->wait(fence, intr, timeout));
}


static inline signed long
dma_fence_wait(struct dma_fence *fence, bool intr)
{
	signed long ret;

	ret = dma_fence_wait_timeout(fence, intr, MAX_SCHEDULE_TIMEOUT);

	return (ret < 0 ? ret : 0);
}


static inline int
dma_fence_add_callback(struct dma_fence *fence, struct dma_fence_cb *cb,
		   dma_fence_func_t func)
{
	int ret = 0;
	bool was_set;

	if (WARN_ON(!fence || !func))
		return -EINVAL;

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		INIT_LIST_HEAD(&cb->node);
		return -ENOENT;
	}

	spin_lock(fence->lock);

	was_set = test_and_set_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags);

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		INIT_LIST_HEAD(&cb->node);
		ret = -ENOENT;
	} else if (was_set == 0 &&
		   fence->ops->enable_signaling(fence) == 0) {
		INIT_LIST_HEAD(&cb->node);
		ret = -ENOENT;
		dma_fence_signal_locked(fence);
	} else {
		cb->func = func;
		list_add_tail(&cb->node, &fence->cb_list);
	}
	spin_unlock(fence->lock);

	return (ret);
}

static inline bool
dma_fence_remove_callback(struct dma_fence *fence, struct dma_fence_cb *cb)
{
        bool ret;

        spin_lock(fence->lock);

        ret = !list_empty(&cb->node);
        if (ret)
                list_del_init(&cb->node);

        spin_unlock(fence->lock);

        return ret;
}

static inline void
dma_fence_default_wait_cb(struct dma_fence *fence, struct dma_fence_cb *cb)
{
	struct default_wait_cb *wait =
		container_of(cb, struct default_wait_cb, base);

	wake_up_process(wait->task);
}

static inline signed long
dma_fence_default_wait(struct dma_fence *fence, bool intr, signed long timeout)
{
	struct default_wait_cb cb;
	signed long ret;
	bool was_set;

	/* under FreeBSD jiffies are 32-bit */
	timeout = (int)timeout;
	ret = timeout;

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return timeout;

	spin_lock(fence->lock);

	if (intr && signal_pending(current)) {
		ret = -ERESTARTSYS;
		goto out;
	}

	was_set = test_and_set_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags);

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		goto out;

	if (!was_set) {
		if (!fence->ops->enable_signaling(fence)) {
			dma_fence_signal_locked(fence);
			goto out;
		}
	}

	cb.base.func = dma_fence_default_wait_cb;
	cb.task = current;
	list_add(&cb.base.node, &fence->cb_list);

	while (!test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags) && ret > 0) {
		if (intr)
			__set_current_state(TASK_INTERRUPTIBLE);
		else
			__set_current_state(TASK_UNINTERRUPTIBLE);
		spin_unlock(fence->lock);

		ret = schedule_timeout(ret);

		spin_lock(fence->lock);
		if (ret > 0 && intr && signal_pending(current))
			ret = -ERESTARTSYS;
	}

	if (!list_empty(&cb.base.node))
		list_del(&cb.base.node);
	__set_current_state(TASK_RUNNING);

out:
	spin_unlock(fence->lock);
	return ret;
}

static inline bool
dma_fence_test_signaled_any(struct dma_fence **fences, uint32_t count)
{
	int i;

	for (i = 0; i < count; ++i) {
		struct dma_fence *fence = fences[i];
		if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
			return true;
	}
	return false;
}

static inline signed long
dma_fence_wait_any_timeout(struct dma_fence **fences, uint32_t count,
    bool intr, signed long timeout, uint32_t *idx)
{
	struct default_wait_cb *cb;
	signed long ret;
	unsigned i;

	/* under FreeBSD jiffies are 32-bit */
	timeout = (int)timeout;
	ret = timeout;

	if (WARN_ON(!fences || !count || timeout < 0))
		return -EINVAL;

	if (timeout == 0) {
		for (i = 0; i < count; ++i)
			if (dma_fence_is_signaled(fences[i]))
				return 1;

		return 0;
	}

	cb = kcalloc(count, sizeof(struct default_wait_cb), GFP_KERNEL);
	if (cb == NULL) {
		ret = -ENOMEM;
		goto err_free_cb;
	}

	for (i = 0; i < count; ++i) {
		struct dma_fence *fence = fences[i];

		if (fence->ops->wait != dma_fence_default_wait) {
			ret = -EINVAL;
			goto dma_fence_rm_cb;
		}

		cb[i].task = current;
		if (dma_fence_add_callback(fence, &cb[i].base,
				       dma_fence_default_wait_cb)) {
			goto dma_fence_rm_cb;
		}
	}

	while (ret > 0) {
		if (intr)
			set_current_state(TASK_INTERRUPTIBLE);
		else
			set_current_state(TASK_UNINTERRUPTIBLE);

		if (dma_fence_test_signaled_any(fences, count))
			break;

		ret = schedule_timeout(ret);

		if (ret > 0 && intr && signal_pending(current))
			ret = -ERESTARTSYS;
	}

	__set_current_state(TASK_RUNNING);

dma_fence_rm_cb:
	while (i-- > 0)
		dma_fence_remove_callback(fences[i], &cb[i].base);

err_free_cb:
	kfree(cb);

	return (ret);
}

static inline void
dma_fence_init(struct dma_fence *fence, const struct dma_fence_ops *ops,
    spinlock_t *lock, u64 context, unsigned seqno)
{

	memset(fence, 0, sizeof(*fence));
	fence->ops = ops;
	fence->lock = lock;
	fence->context = context;
	fence->seqno = seqno;
	INIT_LIST_HEAD(&fence->cb_list);
	kref_init(&fence->refcount);
	fence->error = 0;
}

static inline bool
dma_fence_is_later(struct dma_fence *f1, struct dma_fence *f2){

	// must be in the same context to be a reasonable comparison
	if(WARN_ON(f1->context != f2->context)) return false;

	return (f1->seqno > f2->seqno);
}

static inline void
dma_fence_set_error(struct dma_fence *f, int error)
{

	f->error = error;
}

#define DMA_FENCE_TRACE(f, fmt, args...)		\
	do {								\
		struct dma_fence *__ff = (f);				\
		if (config_enabled(CONFIG_DMA_FENCE_TRACE))			\
			pr_info("f %u#%u: " fmt,			\
				__ff->context, __ff->seqno, ##args);	\
	} while (0)

#define DMA_FENCE_WARN(f, fmt, args...) \
	do {								\
		struct dma_fence *__ff = (f);				\
		pr_warn("f %u#%u: " fmt, __ff->context, __ff->seqno,	\
			 ##args);					\
	} while (0)

#define DMA_FENCE_ERR(f, fmt, args...) \
	do {								\
		struct dma_fence *__ff = (f);				\
		pr_err("f %u#%u: " fmt, __ff->context, __ff->seqno,	\
			##args);					\
	} while (0)
#endif
