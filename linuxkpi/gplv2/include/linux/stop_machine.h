#ifndef _LINUX_STOP_MACHINE
#define _LINUX_STOP_MACHINE

#include <sys/param.h>
#include <sys/types.h>
#include <sys/proc.h>
#include <sys/sched.h>

#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/list.h>

typedef int (*cpu_stop_fn_t)(void *arg);

static inline int
stop_machine(cpu_stop_fn_t fn, void *data, void *dummy)
{
	int ret;
	sched_pin();
	ret = fn(data);
	sched_unpin();
	return (ret);
}

#endif
