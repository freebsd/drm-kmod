#ifndef _BSD_LKPI_LINUX_SCHED_H_
#define _BSD_LKPI_LINUX_SCHED_H_

#include_next <linux/sched.h>

#include <linux/hrtimer.h>
#include <linux/spinlock.h>

#include <sys/rtprio.h>

struct seq_file;

static inline int
bsd_lkpi_cond_resched_lock(spinlock_t *lock)
{
	if (need_resched() == 0)
		return (0);
	spin_unlock(lock);
	cond_resched();
	spin_lock(lock);
	return (1);
}
#define	cond_resched_lock(lock)	bsd_lkpi_cond_resched_lock(lock)

static inline void
bsd_lkpi_sched_set_fifo(struct task_struct *t)
{
	struct rtprio rtp;

	rtp.prio = RTP_PRIO_MAX / 2;
	rtp.type = RTP_PRIO_FIFO;
	rtp_to_pri(&rtp, t->task_thread);
}
#define	sched_set_fifo(t)	bsd_lkpi_sched_set_fifo(t)

static inline void
bsd_lkpi_sched_set_fifo_low(struct task_struct *t)
{
	struct rtprio rtp;

	rtp.prio = RTP_PRIO_MAX;	/* lowest priority */
	rtp.type = RTP_PRIO_FIFO;
	rtp_to_pri(&rtp, t->task_thread);
}
#define	sched_set_fifo_low(t)	bsd_lkpi_sched_set_fifo_low(t)

#endif	/* _BSD_LKPI_LINUX_SCHED_H_ */
