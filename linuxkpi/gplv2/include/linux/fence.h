/*
 * Fence mechanism for dma-buf to allow for asynchronous dma access
 *
 * Copyright (C) 2012 Canonical Ltd
 * Copyright (C) 2012 Texas Instruments
 *
 * Authors:
 * Rob Clark <robdclark@gmail.com>
 * Maarten Lankhorst <maarten.lankhorst@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#ifndef __LINUX_FENCE_H
#define __LINUX_FENCE_H

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

struct fence;
struct fence_cb;

typedef void (*fence_func_t)(struct fence *fence, struct fence_cb *cb);

struct fence_cb {
	struct list_head node;
	fence_func_t func;
};

struct default_wait_cb {
	struct fence_cb base;
	struct task_struct *task;
};

struct fence {
	struct kref refcount;
	const struct fence_ops *ops;
	struct rcu_head rcu;
	struct list_head cb_list;
	spinlock_t *lock;
	u64 context;
	unsigned seqno;
	unsigned long flags;
	ktime_t timestamp;
	int status;
	struct list_head child_list;
	struct list_head active_list;
};

enum fence_flag_bits {
	FENCE_FLAG_SIGNALED_BIT,
	FENCE_FLAG_ENABLE_SIGNAL_BIT,
	FENCE_FLAG_USER_BITS, /* must always be last member */
};

struct fence_ops {
	const char * (*get_driver_name)(struct fence *fence);
	const char * (*get_timeline_name)(struct fence *fence);
	bool (*enable_signaling)(struct fence *fence);
	bool (*signaled)(struct fence *fence);
	signed long (*wait)(struct fence *fence, bool intr, signed long timeout);
	void (*release)(struct fence *fence);

	int (*fill_driver_data)(struct fence *fence, void *data, int size);
	void (*fence_value_str)(struct fence *fence, char *str, int size);
	void (*timeline_value_str)(struct fence *fence, char *str, int size);
};

static inline void
fence_free(struct fence *fence)
{
	kfree_rcu(fence, rcu);
}

static inline struct fence *
fence_get(struct fence *fence)
{
	if (fence)
		kref_get(&fence->refcount);
	return (fence);
}

static inline struct fence *
fence_get_rcu(struct fence *fence)
{
	if (kref_get_unless_zero(&fence->refcount))
		return (fence);
	else
		return (NULL);
}

static inline void
fence_release(struct kref *kref)
{
	struct fence *fence = container_of(kref, struct fence, refcount);

	BUG_ON(!list_empty(&fence->cb_list));
	if (fence->ops->release)
		fence->ops->release(fence);
	else
		kfree(fence);
}

static inline int
fence_signal_locked(struct fence *fence)
{
	struct fence_cb *cur, *tmp;
	int ret = 0;

	if (WARN_ON(!fence))
		return (-EINVAL);

	if (!ktime_to_ns(fence->timestamp)) {
		fence->timestamp = ktime_get();
#ifdef __FreeBSD__
		mb();
#else
		smp_mb__before_atomic();
#endif
	}

	if (test_and_set_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		ret = -EINVAL;
	}
	list_for_each_entry_safe(cur, tmp, &fence->cb_list, node) {
		list_del_init(&cur->node);
		cur->func(fence, cur);
	}
	return (ret);
}

static inline void
fence_enable_sw_signaling(struct fence *fence)
{
	unsigned long flags;

	if (!test_and_set_bit(FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags) &&
	    !test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		spin_lock_irqsave(fence->lock, flags);

		if (!fence->ops->enable_signaling(fence))
			fence_signal_locked(fence);

		spin_unlock_irqrestore(fence->lock, flags);
	}
}


static inline int
fence_signal(struct fence *fence)
{
	unsigned long flags;

	if (!fence)
		return (-EINVAL);

	if (!ktime_to_ns(fence->timestamp)) {
		fence->timestamp = ktime_get();
#ifdef __FreeBSD__
		mb();
#else
		smp_mb__before_atomic();
#endif
	}

	if (test_and_set_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return (-EINVAL);


	if (test_bit(FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags)) {
		struct fence_cb *cur, *tmp;

		spin_lock_irqsave(fence->lock, flags);
		list_for_each_entry_safe(cur, tmp, &fence->cb_list, node) {
			list_del_init(&cur->node);
			cur->func(fence, cur);
		}
		spin_unlock_irqrestore(fence->lock, flags);
	}
	return (0);
}

static inline bool
fence_is_signaled(struct fence *fence)
{
	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return (true);

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		fence_signal(fence);
		return (true);
	}

	return (false);
}

static inline signed long
fence_wait_timeout(struct fence *fence, bool intr, signed long timeout)
{

	/* under FreeBSD jiffies are 32-bit */
	timeout = (int)timeout;

	if (WARN_ON(timeout < 0))
		return (-EINVAL);

	if (timeout == 0)
		return (fence_is_signaled(fence));
	else
		return (fence->ops->wait(fence, intr, timeout));
}


static inline signed long
fence_wait(struct fence *fence, bool intr)
{
	signed long ret;

	ret = fence_wait_timeout(fence, intr, MAX_SCHEDULE_TIMEOUT);

	return (ret < 0 ? ret : 0);
}


static inline void
fence_put(struct fence *fence)
{
	if (fence)
		kref_put(&fence->refcount, fence_release);
}

static inline int
fence_add_callback(struct fence *fence, struct fence_cb *cb,
		   fence_func_t func)
{
	unsigned long flags;
	int ret = 0;
	bool was_set;

	if (WARN_ON(!fence || !func))
		return -EINVAL;

	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags)) {
		INIT_LIST_HEAD(&cb->node);
		return -ENOENT;
	}

	spin_lock_irqsave(fence->lock, flags);

	was_set = test_and_set_bit(FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags);

	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		ret = -ENOENT;
	else if (!was_set) {
		if (!fence->ops->enable_signaling(fence)) {
			fence_signal_locked(fence);
			ret = -ENOENT;
		}
	}

	if (!ret) {
		cb->func = func;
		list_add_tail(&cb->node, &fence->cb_list);
	} else
		INIT_LIST_HEAD(&cb->node);
	spin_unlock_irqrestore(fence->lock, flags);

	return (ret);
}

static inline bool
fence_remove_callback(struct fence *fence, struct fence_cb *cb)
{
	unsigned long flags;
        bool ret;

        spin_lock_irqsave(fence->lock, flags);

        ret = !list_empty(&cb->node);
        if (ret)
                list_del_init(&cb->node);

        spin_unlock_irqrestore(fence->lock, flags);

        return ret;
}

static inline void
fence_default_wait_cb(struct fence *fence, struct fence_cb *cb)
{
	struct default_wait_cb *wait =
		container_of(cb, struct default_wait_cb, base);

	wake_up_process(wait->task);
}

static inline signed long
fence_default_wait(struct fence *fence, bool intr, signed long timeout)
{
	struct default_wait_cb cb;
	unsigned long flags;
	signed long ret;
	bool was_set;

	/* under FreeBSD jiffies are 32-bit */
	timeout = (int)timeout;
	ret = timeout;

	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return timeout;

	spin_lock_irqsave(fence->lock, flags);

	if (intr && signal_pending(current)) {
		ret = -ERESTARTSYS;
		goto out;
	}

	was_set = test_and_set_bit(FENCE_FLAG_ENABLE_SIGNAL_BIT, &fence->flags);

	if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		goto out;

	if (!was_set) {
		if (!fence->ops->enable_signaling(fence)) {
			fence_signal_locked(fence);
			goto out;
		}
	}

	cb.base.func = fence_default_wait_cb;
	cb.task = current;
	list_add(&cb.base.node, &fence->cb_list);

	while (!test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags) && ret > 0) {
		if (intr)
			__set_current_state(TASK_INTERRUPTIBLE);
		else
			__set_current_state(TASK_UNINTERRUPTIBLE);
		spin_unlock_irqrestore(fence->lock, flags);

		ret = schedule_timeout(ret);

		spin_lock_irqsave(fence->lock, flags);
		if (ret > 0 && intr && signal_pending(current))
			ret = -ERESTARTSYS;
	}

	if (!list_empty(&cb.base.node))
		list_del(&cb.base.node);
	__set_current_state(TASK_RUNNING);

out:
	spin_unlock_irqrestore(fence->lock, flags);
	return ret;
}

static inline bool
fence_test_signaled_any(struct fence **fences, uint32_t count)
{
	int i;

	for (i = 0; i < count; ++i) {
		struct fence *fence = fences[i];
		if (test_bit(FENCE_FLAG_SIGNALED_BIT, &fence->flags))
			return true;
	}
	return false;
}

static inline signed long
fence_wait_any_timeout(struct fence **fences, uint32_t count,
		       bool intr, signed long timeout)
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
			if (fence_is_signaled(fences[i]))
				return 1;

		return 0;
	}

	cb = kcalloc(count, sizeof(struct default_wait_cb), GFP_KERNEL);
	if (cb == NULL) {
		ret = -ENOMEM;
		goto err_free_cb;
	}

	for (i = 0; i < count; ++i) {
		struct fence *fence = fences[i];

		if (fence->ops->wait != fence_default_wait) {
			ret = -EINVAL;
			goto fence_rm_cb;
		}

		cb[i].task = current;
		if (fence_add_callback(fence, &cb[i].base,
				       fence_default_wait_cb)) {
			goto fence_rm_cb;
		}
	}

	while (ret > 0) {
		if (intr)
			set_current_state(TASK_INTERRUPTIBLE);
		else
			set_current_state(TASK_UNINTERRUPTIBLE);

		if (fence_test_signaled_any(fences, count))
			break;

		ret = schedule_timeout(ret);

		if (ret > 0 && intr && signal_pending(current))
			ret = -ERESTARTSYS;
	}

	__set_current_state(TASK_RUNNING);

fence_rm_cb:
	while (i-- > 0)
		fence_remove_callback(fences[i], &cb[i].base);

err_free_cb:
	kfree(cb);

	return (ret);
}

u64 fence_context_alloc(unsigned num);

static inline void
fence_init(struct fence *fence, const struct fence_ops *ops,
    spinlock_t *lock, u64 context, unsigned seqno)
{

	memset(fence, 0, sizeof(*fence));
	fence->ops = ops;
	fence->lock = lock;
	fence->context = context;
	fence->seqno = seqno;
	INIT_LIST_HEAD(&fence->cb_list);
	kref_init(&fence->refcount);
}

static inline bool
fence_is_later(struct fence *f1, struct fence *f2){

	// must be in the same context to be a reasonable comparison 
	if(WARN_ON(f1->context != f2->context)) return false;
	
	return (f1->seqno > f2->seqno);
}


#define FENCE_TRACE(f, fmt, args...) \
	do {								\
		struct fence *__ff = (f);				\
		if (config_enabled(CONFIG_FENCE_TRACE))			\
			pr_info("f %u#%u: " fmt,			\
				__ff->context, __ff->seqno, ##args);	\
	} while (0)

#define FENCE_WARN(f, fmt, args...) \
	do {								\
		struct fence *__ff = (f);				\
		pr_warn("f %u#%u: " fmt, __ff->context, __ff->seqno,	\
			 ##args);					\
	} while (0)

#define FENCE_ERR(f, fmt, args...) \
	do {								\
		struct fence *__ff = (f);				\
		pr_err("f %u#%u: " fmt, __ff->context, __ff->seqno,	\
			##args);					\
	} while (0)
#endif
