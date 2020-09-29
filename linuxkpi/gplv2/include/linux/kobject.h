#ifndef __LINUX_KOBJECT2_H__
#define __LINUX_KOBJECT2_H__

#include_next <linux/kobject.h>

#if !defined(LINUXKPI_COOKIE) || (LINUXKPI_COOKIE < 1600256818)
enum kobject_action {
	KOBJ_ADD,
	KOBJ_REMOVE,
	KOBJ_CHANGE,
	KOBJ_MOVE,
	KOBJ_ONLINE,
	KOBJ_OFFLINE,
	KOBJ_BIND,
	KOBJ_UNBIND,
	KOBJ_MAX
};

static inline int
kobject_uevent_env(struct kobject *kobj __unused,
    enum kobject_action action __unused, char *envp[] __unused)
{

	return (0);
}
#endif

#endif
