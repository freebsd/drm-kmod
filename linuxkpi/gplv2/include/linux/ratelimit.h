#ifndef _LINUX_RATELIMIT_H
#define _LINUX_RATELIMIT_H

#include <linux/sched.h>
#include <linux/spinlock.h>

#define DEFAULT_RATELIMIT_INTERVAL      (5 * 1000 /* HZ */)
#define DEFAULT_RATELIMIT_BURST         10

/* issue num suppressed message on exit */
#define RATELIMIT_MSG_ON_RELEASE        BIT(0)

struct ratelimit_state {
        spinlock_t  lock;           /* protect the state */

	int             interval;
        int             burst;
        int             printed;
        int             missed;
        unsigned long   begin;
        unsigned long   flags;
};

#define RATELIMIT_STATE_INIT(name, interval_init, burst_init) {         \
                .interval       = interval_init,                        \
                .burst          = burst_init,                           \
	}

#define RATELIMIT_STATE_INIT_DISABLED                                   \
        RATELIMIT_STATE_INIT(ratelimit_state, 0, DEFAULT_RATELIMIT_BURST)

#define DEFINE_RATELIMIT_STATE(name, interval_init, burst_init)         \
                                                                        \
	struct ratelimit_state name =                                   \
	        RATELIMIT_STATE_INIT(name, interval_init, burst_init)   \

static inline void ratelimit_state_init(struct ratelimit_state *rs,
					int interval, int burst)
{
        memset(rs, 0, sizeof(*rs));

        spin_lock_init(&rs->lock);
	rs->interval    = interval;
	rs->burst       = burst;
}

/* XXX not really implemented */
static inline int
___ratelimit(struct ratelimit_state *rs, const char *func)
{
	return (1);

}
#define __ratelimit(state) ___ratelimit(state, __func__)


#endif
