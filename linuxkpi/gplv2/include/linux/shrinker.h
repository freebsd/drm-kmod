#ifndef _LINUX_SHRINKER_H
#define _LINUX_SHRINKER_H

#include <linux/types.h>
#include <linux/list.h>

#include <asm/atomic-long.h>

struct shrink_control {
	gfp_t gfp_mask;

	/*
	 * How many objects scan_objects should scan and try to reclaim.
	 * This is reset before every call, so it is safe for callees
	 * to modify.
	 */
	unsigned long nr_to_scan;

	int nid;
};


#define SHRINK_STOP (~0UL)

struct shrinker {
	unsigned long (*count_objects)(struct shrinker *,
				       struct shrink_control *sc);
	unsigned long (*scan_objects)(struct shrinker *,
				      struct shrink_control *sc);

	int seeks;	/* seeks to recreate an obj */
	long batch;	/* reclaim batch size, 0 = default */
	unsigned long flags;

	/* These are for internal use */
	struct list_head list;
	/* objs pending delete, per node */
	atomic_long_t *nr_deferred;
};
#define DEFAULT_SEEKS 2 /* A good number if you don't know better. */

#define SHRINKER_NUMA_AWARE     (1 << 0)
#define SHRINKER_MEMCG_AWARE    (1 << 1)

extern int register_shrinker(struct shrinker *);
extern void unregister_shrinker(struct shrinker *);

#endif
