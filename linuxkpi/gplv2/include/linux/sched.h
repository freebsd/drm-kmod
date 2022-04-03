#ifndef _LINUX_GPLV2_SCHED_H_
#define _LINUX_GPLV2_SCHED_H_

#include_next <linux/sched.h>

#include <linux/hrtimer.h>

struct seq_file;

static inline int
sched_set_fifo_low(struct task_struct *t)
{
	UNIMPLEMENTED();
	return (0);
}

#endif /* _LINUX_GPLV2_SCHED_H_ */
