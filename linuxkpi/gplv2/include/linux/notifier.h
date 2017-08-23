#ifndef	_LINUX_GPLV2_NOTIFIER_H_
#define	_LINUX_GPLV2_NOTIFIER_H_

#include <linux/mutex.h>
#include <linux/rwsem.h>

#include_next <linux/notifier.h>

#define	NOTIFY_OK		0x01
#define	NOTIFY_STOP_MASK	0x80
#define	NOTIFY_BAD		(NOTIFY_STOP_MASK|0x02)

struct atomic_notifier_head {
	spinlock_t lock;
	struct notifier_block __rcu *head;
};

struct blocking_notifier_head {
	struct rw_semaphore rwsem;
	struct notifier_block __rcu *head;
};

#define ATOMIC_INIT_NOTIFIER_HEAD(name) do {	\
		spin_lock_init(&(name)->lock);	\
		(name)->head = NULL;		\
	} while (0)
#define BLOCKING_INIT_NOTIFIER_HEAD(name) do {	\
		init_rwsem(&(name)->rwsem);	\
		(name)->head = NULL;		\
	} while (0)

extern int atomic_notifier_call_chain(struct atomic_notifier_head *nh,
		unsigned long val, void *v);
extern int __atomic_notifier_call_chain(struct atomic_notifier_head *nh,
		unsigned long val, void *v, int nr_to_call, int *nr_calls);

extern int blocking_notifier_chain_register(struct blocking_notifier_head *nh,
		struct notifier_block *nb);
extern int blocking_notifier_call_chain(struct blocking_notifier_head *nh,
		unsigned long val, void *v);
extern int blocking_notifier_chain_unregister(struct blocking_notifier_head *nh,
		struct notifier_block *nb);

#endif /* _LINUX_GPLV2_NOTIFIER_H_ */
