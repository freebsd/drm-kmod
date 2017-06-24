#ifndef _LINUX_GPLV2_SCHED_H_
#define _LINUX_GPLV2_SCHED_H_

#include_next <linux/sched.h>

#include <linux/hrtimer.h>

/* XXX */
#define	smp_mb__before_atomic()	atomic_thread_fence_seq_cst()
struct seq_file;

static inline int
sched_setscheduler(struct task_struct *t, int policy,
    const struct sched_param *param)
{
	UNIMPLEMENTED();
	return (0);
}

static inline int
sched_setscheduler_nocheck(struct task_struct *t, int policy,
    const struct sched_param *param)
{
	UNIMPLEMENTED();
	return (0);
}

#endif /* _LINUX_GPLV2_SCHED_H_ */
