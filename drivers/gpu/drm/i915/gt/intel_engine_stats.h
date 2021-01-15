/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2020 Intel Corporation
 */

#ifndef __INTEL_ENGINE_STATS_H__
#define __INTEL_ENGINE_STATS_H__

#include <linux/atomic.h>
#include <linux/ktime.h>
#include <linux/seqlock.h>

#include "i915_gem.h" /* GEM_BUG_ON */
#include "intel_engine.h"

static inline void intel_engine_context_in(struct intel_engine_cs *engine)
{
#ifdef __linux__
	unsigned long flags;
#endif

	if (engine->stats.active) {
		engine->stats.active++;
		return;
	}

	/* The writer is serialised; but the pmu reader may be from hardirq */
#ifdef __linux__
	local_irq_save(flags);
#endif
	write_seqcount_begin(&engine->stats.lock);

	engine->stats.start = ktime_get();
	engine->stats.active++;

	write_seqcount_end(&engine->stats.lock);
#ifdef __linux__
	local_irq_restore(flags);
#endif

	GEM_BUG_ON(!engine->stats.active);
}

static inline void intel_engine_context_out(struct intel_engine_cs *engine)
{
#ifdef __linux__
	unsigned long flags;
#endif

	GEM_BUG_ON(!engine->stats.active);
	if (engine->stats.active > 1) {
		engine->stats.active--;
		return;
	}

#ifdef __linux__
	local_irq_save(flags);
#endif
	write_seqcount_begin(&engine->stats.lock);

	engine->stats.active--;
	engine->stats.total =
		ktime_add(engine->stats.total,
			  ktime_sub(ktime_get(), engine->stats.start));

	write_seqcount_end(&engine->stats.lock);
#ifdef __linux__
	local_irq_restore(flags);
#endif
}

#endif /* __INTEL_ENGINE_STATS_H__ */
