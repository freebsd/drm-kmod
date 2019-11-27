/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2019 Intel Corporation
 */

#ifndef _I915_ACTIVE_TYPES_H_
#define _I915_ACTIVE_TYPES_H_

#include <linux/atomic.h>
#include <linux/dma-fence.h>
#include <linux/llist.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/rcupdate.h>
#include <linux/workqueue.h>
#ifdef __FreeBSD__
#include <linux/irq_work.h>
#endif

#include "i915_utils.h"

struct i915_active_fence {
	struct dma_fence __rcu *fence;
	struct dma_fence_cb cb;
};

struct active_node;

#define I915_ACTIVE_MAY_SLEEP BIT(0)

#define __i915_active_call __aligned(4)
#define i915_active_may_sleep(fn) ptr_pack_bits(&(fn), I915_ACTIVE_MAY_SLEEP, 2)

struct i915_active {
	atomic_t count;
	struct mutex mutex;

	spinlock_t tree_lock;
	struct active_node *cache;
	struct rb_root tree;

	/* Preallocated "exclusive" node */
	struct i915_active_fence excl;

	unsigned long flags;
#define I915_ACTIVE_RETIRE_SLEEPS BIT(0)

	int (*active)(struct i915_active *ref);
	void (*retire)(struct i915_active *ref);

#ifdef __linux__
	struct work_struct work;
#elif defined(__FreeBSD__)
	/* On FreeBSD this work is sporadically scheduled
	 * within a critical section. */
	struct irq_work work;
#endif

	struct llist_head preallocated_barriers;
};

#endif /* _I915_ACTIVE_TYPES_H_ */
