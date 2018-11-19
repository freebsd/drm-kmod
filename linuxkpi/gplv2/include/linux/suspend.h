#ifndef _LINUX_GPLV2_SUSPEND_H_
#define _LINUX_GPLV2_SUSPEND_H_

#include <linux/notifier.h>

/* Hibernation and suspend events */
#define PM_HIBERNATION_PREPARE	0x0001 /* Going to hibernate */
#define PM_POST_HIBERNATION	0x0002 /* Hibernation finished */
#define PM_SUSPEND_PREPARE	0x0003 /* Going to suspend the system */
#define PM_POST_SUSPEND		0x0004 /* Suspend finished */
#define PM_RESTORE_PREPARE	0x0005 /* Going to restore a saved image */
#define PM_POST_RESTORE		0x0006 /* Restore failed */

static inline int
register_pm_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int
unregister_pm_notifier(struct notifier_block *nb)
{
	return 0;
}

#endif
