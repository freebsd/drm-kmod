#ifndef _LINUX_SYSRQ_H
#define _LINUX_SYSRQ_H

#include <linux/errno.h>
#include <linux/types.h>

/* Enable/disable SYSRQ support by default (0==no, 1==yes). */
#define SYSRQ_DEFAULT_ENABLE	1

/* Possible values of bitmask for enabling sysrq functions */
/* 0x0001 is reserved for enable everything */
#define SYSRQ_ENABLE_LOG	0x0002
#define SYSRQ_ENABLE_KEYBOARD	0x0004
#define SYSRQ_ENABLE_DUMP	0x0008
#define SYSRQ_ENABLE_SYNC	0x0010
#define SYSRQ_ENABLE_REMOUNT	0x0020
#define SYSRQ_ENABLE_SIGNAL	0x0040
#define SYSRQ_ENABLE_BOOT	0x0080
#define SYSRQ_ENABLE_RTNICE	0x0100

struct sysrq_key_op {
	void (*handler)(int);
	char *help_msg;
	char *action_msg;
	int enable_mask;
};
#define register_sysrq_key(a, b)
#define unregister_sysrq_key(a, b)

#endif
