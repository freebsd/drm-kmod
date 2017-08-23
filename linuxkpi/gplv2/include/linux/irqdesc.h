#ifndef _LINUX_IRQDESC_H
#define _LINUX_IRQDESC_H

#include <linux/rcupdate.h>
#include <linux/idr.h>
#include <linux/interrupt.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>

extern struct idr irq_idr;
struct irq_desc {
	struct irq_data		irq_data;
	irq_flow_handler_t	handle_irq;
	spinlock_t		lock;
	struct irqaction	*action;	/* IRQ action list */
	unsigned int		istate;
	unsigned int		status_use_accessors;
	unsigned long		threads_oneshot;
	atomic_t		threads_active;

	struct rcu_head		rcu;
	const char		*name;
};

#define for_each_action_of_desc(desc, act)			\
	for (act = desc->act; act; act = act->next)

static inline struct irq_desc *
irq_to_desc(unsigned int irq)
{
	return (idr_find(&irq_idr, irq));
}

static inline struct irq_desc *
irq_get_desc_lock(unsigned int irq, unsigned long *flags, unsigned int check)
{
	struct irq_desc *desc = irq_to_desc(irq);

	if (desc)
		spin_lock_irqsave(&desc->lock, *flags);
	return (desc);
}

static inline void
irq_put_desc_unlock(struct irq_desc *desc, unsigned long flags)
{
	spin_unlock_irqrestore(&desc->lock, flags);
}

extern struct idr irq_idr;

static inline void
generic_handle_irq_desc(struct irq_desc *desc)
{
	desc->handle_irq(desc);
}


static inline int
generic_handle_irq(unsigned int irq)
{
	struct irq_desc *desc = irq_to_desc(irq);

	if (!desc)
		return -EINVAL;
	generic_handle_irq_desc(desc);
	return 0;
}


static inline void
delete_irq_desc(unsigned int irq)
{
	idr_remove(&irq_idr, irq);
}


static inline void
delayed_free_desc(struct rcu_head *rhp)
{
	struct irq_desc *desc = container_of(rhp, struct irq_desc, rcu);

	kfree(desc);
}
#endif
