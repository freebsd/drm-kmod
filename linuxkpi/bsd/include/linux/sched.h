#ifndef _BSD_LKPI_LINUX_SCHED_H_
#define _BSD_LKPI_LINUX_SCHED_H_

#include_next <linux/sched.h>

#include <linux/hrtimer.h>

#include <sys/rtprio.h>

struct seq_file;

static inline void
sched_set_fifo(struct task_struct *t)
{
	struct rtprio rtp;

	rtp.prio = RTP_PRIO_MAX / 2;
	rtp.type = RTP_PRIO_FIFO;
	rtp_to_pri(&rtp, t->task_thread);
}

static inline void
sched_set_fifo_low(struct task_struct *t)
{
	struct rtprio rtp;

	rtp.prio = RTP_PRIO_MAX;	/* lowest priority */
	rtp.type = RTP_PRIO_FIFO;
	rtp_to_pri(&rtp, t->task_thread);
}

#endif	/* _BSD_LKPI_LINUX_SCHED_H_ */
