#ifndef _LINUX_IRQ_H
#define _LINUX_IRQ_H

#include <linux/smp.h>
#include <linux/cache.h>
#include <linux/spinlock.h>
#include <linux/gfp.h>
#include <linux/interrupt.h>
#include <linux/irqhandler.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/bitmap.h>

#include <asm/processor.h>

#if defined(__i386__) || defined(__amd64__) || defined(__aarch64__) || defined(__powerpc__) || defined(__riscv)
#define NR_IRQS	512 /* XXX need correct value */
#else
#error "NR_IRQS not defined"
#endif

#define IRQ_BITMAP_BITS	NR_IRQS

extern struct mutex sparse_irq_lock;

extern DECLARE_BITMAP(allocated_irqs, IRQ_BITMAP_BITS);

extern struct irq_chip no_irq_chip;
struct msi_desc;
extern int nr_irqs;
struct irq_common_data {
	unsigned int		state_use_accessors;
	void			*handler_data;
	struct msi_desc		*msi_desc;
};

struct irq_data {
	u32			mask;
	unsigned int		irq;
	unsigned long		hwirq;
	struct irq_common_data	*common;
	struct irq_chip		*chip;
	struct irq_domain	*domain;
	void			*chip_data;
};

struct irq_chip {
	const char	*name;
	void		(*irq_mask)(struct irq_data *data);
	void		(*irq_unmask)(struct irq_data *data);
};
#ifndef ARCH_IRQ_INIT_FLAGS
# define ARCH_IRQ_INIT_FLAGS	0
#endif

#define IRQ_DEFAULT_INIT_FLAGS	ARCH_IRQ_INIT_FLAGS

enum {
	IRQCHIP_SET_TYPE_MASKED		= (1 <<  0),
	IRQCHIP_EOI_IF_HANDLED		= (1 <<  1),
	IRQCHIP_MASK_ON_SUSPEND		= (1 <<  2),
	IRQCHIP_ONOFFLINE_ENABLED	= (1 <<  3),
	IRQCHIP_SKIP_SET_WAKE		= (1 <<  4),
	IRQCHIP_ONESHOT_SAFE		= (1 <<  5),
	IRQCHIP_EOI_THREADED		= (1 <<  6),
};

struct irqaction {
	irq_handler_t		handler;
	void			*dev_id;
	struct irqaction	*next;
	irq_handler_t		thread_fn;
	struct task_struct	*thread;
	struct irqaction	*secondary;
	unsigned int		irq;
	unsigned int		flags;
	unsigned long		thread_flags;
	unsigned long		thread_mask;
	const char		*name;
};

#include <linux/irqdesc.h>

enum {
	IRQ_TYPE_NONE		= 0x00000000,
	IRQ_TYPE_EDGE_RISING	= 0x00000001,
	IRQ_TYPE_EDGE_FALLING	= 0x00000002,
	IRQ_TYPE_EDGE_BOTH	= (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
	IRQ_TYPE_LEVEL_HIGH	= 0x00000004,
	IRQ_TYPE_LEVEL_LOW	= 0x00000008,
	IRQ_TYPE_LEVEL_MASK	= (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
	IRQ_TYPE_SENSE_MASK	= 0x0000000f,
	IRQ_TYPE_DEFAULT	= IRQ_TYPE_SENSE_MASK,

	IRQ_TYPE_PROBE		= 0x00000010,

	IRQ_LEVEL		= (1 <<  8),
	IRQ_PER_CPU		= (1 <<  9),
	IRQ_NOPROBE		= (1 << 10),
	IRQ_NOREQUEST		= (1 << 11),
	IRQ_NOAUTOEN		= (1 << 12),
	IRQ_NO_BALANCING	= (1 << 13),
	IRQ_MOVE_PCNTXT		= (1 << 14),
	IRQ_NESTED_THREAD	= (1 << 15),
	IRQ_NOTHREAD		= (1 << 16),
	IRQ_PER_CPU_DEVID	= (1 << 17),
	IRQ_IS_POLLED		= (1 << 18),
	IRQ_DISABLE_UNLAZY	= (1 << 19),
};

enum {
	IRQD_TRIGGER_MASK		= 0xf,
	IRQD_SETAFFINITY_PENDING	= (1 <<  8),
	IRQD_NO_BALANCING		= (1 << 10),
	IRQD_PER_CPU			= (1 << 11),
	IRQD_AFFINITY_SET		= (1 << 12),
	IRQD_LEVEL			= (1 << 13),
	IRQD_WAKEUP_STATE		= (1 << 14),
	IRQD_MOVE_PCNTXT		= (1 << 15),
	IRQD_IRQ_DISABLED		= (1 << 16),
	IRQD_IRQ_MASKED			= (1 << 17),
	IRQD_IRQ_INPROGRESS		= (1 << 18),
	IRQD_WAKEUP_ARMED		= (1 << 19),
	IRQD_FORWARDED_TO_VCPU		= (1 << 20),
};

enum {
	IRQS_AUTODETECT		= 0x00000001,
	IRQS_SPURIOUS_DISABLED	= 0x00000002,
	IRQS_POLL_INPROGRESS	= 0x00000008,
	IRQS_ONESHOT		= 0x00000020,
	IRQS_REPLAY		= 0x00000040,
	IRQS_WAITING		= 0x00000080,
	IRQS_PENDING		= 0x00000200,
	IRQS_SUSPENDED		= 0x00000800,
};

enum {
	IRQTF_RUNTHREAD,
	IRQTF_WARNED,
	IRQTF_AFFINITY,
	IRQTF_FORCED_THREAD,
};


#define IRQF_MODIFY_MASK	\
	(IRQ_TYPE_SENSE_MASK | IRQ_NOPROBE | IRQ_NOREQUEST | \
	 IRQ_NOAUTOEN | IRQ_MOVE_PCNTXT | IRQ_LEVEL | IRQ_NO_BALANCING | \
	 IRQ_PER_CPU | IRQ_NESTED_THREAD | IRQ_NOTHREAD | IRQ_PER_CPU_DEVID | \
	 IRQ_IS_POLLED | IRQ_DISABLE_UNLAZY)

enum {
	_IRQ_DEFAULT_INIT_FLAGS	= IRQ_DEFAULT_INIT_FLAGS,
	_IRQ_PER_CPU		= IRQ_PER_CPU,
	_IRQ_LEVEL		= IRQ_LEVEL,
	_IRQ_NOPROBE		= IRQ_NOPROBE,
	_IRQ_NOREQUEST		= IRQ_NOREQUEST,
	_IRQ_NOTHREAD		= IRQ_NOTHREAD,
	_IRQ_NOAUTOEN		= IRQ_NOAUTOEN,
	_IRQ_MOVE_PCNTXT	= IRQ_MOVE_PCNTXT,
	_IRQ_NO_BALANCING	= IRQ_NO_BALANCING,
	_IRQ_NESTED_THREAD	= IRQ_NESTED_THREAD,
	_IRQ_PER_CPU_DEVID	= IRQ_PER_CPU_DEVID,
	_IRQ_IS_POLLED		= IRQ_IS_POLLED,
	_IRQ_DISABLE_UNLAZY	= IRQ_DISABLE_UNLAZY,
	_IRQF_MODIFY_MASK	= IRQF_MODIFY_MASK,
};


#define linux_irqd_to_state(d) ((d)->common->state_use_accessors)

static inline void
irq_settings_clr_and_set(struct irq_desc *desc, u32 clr, u32 set)
{
	desc->status_use_accessors &= ~(clr & _IRQF_MODIFY_MASK);
	desc->status_use_accessors |= (set & _IRQF_MODIFY_MASK);
}

static inline u32 irq_settings_get_trigger_mask(struct irq_desc *desc)
{
	return desc->status_use_accessors & IRQ_TYPE_SENSE_MASK;
}

static inline void
irq_settings_set_trigger_mask(struct irq_desc *desc, u32 mask)
{
	desc->status_use_accessors &= ~IRQ_TYPE_SENSE_MASK;
	desc->status_use_accessors |= mask & IRQ_TYPE_SENSE_MASK;
}

static inline void
irqd_clear(struct irq_data *d, unsigned int mask)
{
	linux_irqd_to_state(d) &= ~mask;
}

static inline void
irqd_set(struct irq_data *d, unsigned int mask)
{
	linux_irqd_to_state(d) |= mask;
}

static inline void
irq_modify_status(unsigned int irq, unsigned long clr, unsigned long set)
{
	unsigned long flags;
	struct irq_desc *desc = irq_get_desc_lock(irq, &flags, 0);

	if (!desc)
		return;
	irq_settings_clr_and_set(desc, clr, set);

	irqd_clear(&desc->irq_data, IRQD_NO_BALANCING | IRQD_PER_CPU |
		   IRQD_TRIGGER_MASK | IRQD_LEVEL | IRQD_MOVE_PCNTXT);
	irqd_set(&desc->irq_data, irq_settings_get_trigger_mask(desc));

	irq_put_desc_unlock(desc, flags);
}


#define irq_alloc_descs_from(from, cnt, node)	\
	irq_alloc_descs(-1, from, cnt, node)


static inline void irq_set_status_flags(unsigned int irq, unsigned long set)
{
	irq_modify_status(irq, 0, set);
}

static inline void irq_clear_status_flags(unsigned int irq, unsigned long clr)
{
	irq_modify_status(irq, clr, 0);
}


static inline bool
irqd_has_set(struct irq_data *d, unsigned int mask)
{
	return linux_irqd_to_state(d) & mask;
}

static inline bool
irqd_irq_disabled(struct irq_data *d)
{
	return linux_irqd_to_state(d) & IRQD_IRQ_DISABLED;
}

static inline bool
irqd_irq_inprogress(struct irq_data *d)
{
	return linux_irqd_to_state(d) & IRQD_IRQ_INPROGRESS;
}


static inline bool
irq_wait_for_poll(struct irq_desc *desc)
{

	do {
		spin_unlock(&desc->lock);
		while (irqd_irq_inprogress(&desc->irq_data))
			cpu_relax();
		spin_lock(&desc->lock);
	} while (irqd_irq_inprogress(&desc->irq_data));

	return !irqd_irq_disabled(&desc->irq_data) && desc->action;
}

static inline bool
irq_check_poll(struct irq_desc *desc)
{
	if (!(desc->istate & IRQS_POLL_INPROGRESS))
		return false;
	return irq_wait_for_poll(desc);
}

static inline bool
irq_pm_check_wakeup(struct irq_desc *desc)
{
	return (false);
}

static inline bool
irq_may_run(struct irq_desc *desc)
{
	unsigned int mask = IRQD_IRQ_INPROGRESS | IRQD_WAKEUP_ARMED;

	if (!irqd_has_set(&desc->irq_data, mask))
		return true;

	if (irq_pm_check_wakeup(desc))
		return false;

	return irq_check_poll(desc);
}

static inline void
linux_irq_wake_thread(struct irq_desc *desc, struct irqaction *action)
{
	if (kthread_should_stop_task(action->thread))
		return;
	if (test_and_set_bit(IRQTF_RUNTHREAD, &action->thread_flags))
		return;
	desc->threads_oneshot |= action->thread_mask;
	atomic_inc(&desc->threads_active);
	wake_up_process(action->thread);
}

static inline irqreturn_t
handle_irq_event_percpu(struct irq_desc *desc)
{
	irqreturn_t retval = IRQ_NONE;
	unsigned int flags = 0, irq = desc->irq_data.irq;
	struct irqaction *action;

	for_each_action_of_desc(desc, action) {
		irqreturn_t res;

		res = action->handler(irq, action->dev_id);
		switch (res) {
		case IRQ_WAKE_THREAD:
			MPASS(action->thread_fn != NULL);
			linux_irq_wake_thread(desc, action);
		case IRQ_HANDLED:
			flags |= action->flags;
			break;
		default:
			break;
		}
		retval |= res;
	}
	return (retval);
}

static inline irqreturn_t
handle_irq_event(struct irq_desc *desc)
{
	irqreturn_t ret;

	desc->istate &= ~IRQS_PENDING;
	irqd_set(&desc->irq_data, IRQD_IRQ_INPROGRESS);
	spin_unlock(&desc->lock);

	ret = handle_irq_event_percpu(desc);

	spin_lock(&desc->lock);
	irqd_clear(&desc->irq_data, IRQD_IRQ_INPROGRESS);
	return ret;
}

static inline void
handle_simple_irq(struct irq_desc *desc)
{
	spin_lock(&desc->lock);

	if (!irq_may_run(desc))
		goto out_unlock;

	desc->istate &= ~(IRQS_REPLAY | IRQS_WAITING);

	if (unlikely(desc->action == NULL || irqd_irq_disabled(&desc->irq_data))) {
		desc->istate |= IRQS_PENDING;
		goto out_unlock;
	}

	handle_irq_event(desc);

out_unlock:
	spin_unlock(&desc->lock);
	
}

static inline int
irq_set_chip(unsigned int irq, struct irq_chip *chip)
{
	unsigned long flags;
	struct irq_desc *desc = irq_get_desc_lock(irq, &flags, 0);

	if (!desc)
		return -EINVAL;

	if (!chip)
		chip = &no_irq_chip;

	desc->irq_data.chip = chip;
	irq_put_desc_unlock(desc, flags);
	return 0;
}

static inline void
linux_irq_set_handler(unsigned int irq, irq_flow_handler_t handle, int is_chained,
		  const char *name)
{
	unsigned long flags;
	struct irq_desc *desc = irq_get_desc_lock(irq, &flags, 0);

	if (!desc)
		return;
	desc->handle_irq = handle;
	desc->name = name;
	irq_put_desc_unlock(desc, flags);
}

static inline void
irq_set_chip_and_handler_name(unsigned int irq, struct irq_chip *chip,
			      irq_flow_handler_t handle, const char *name)
{
	irq_set_chip(irq, chip);
	linux_irq_set_handler(irq, handle, 0, name);
}

static inline void
irq_set_chip_and_handler(unsigned int irq, struct irq_chip *chip,
					    irq_flow_handler_t handle)
{
	irq_set_chip_and_handler_name(irq, chip, handle, NULL);
}

static inline int
irq_expand_nr_irqs(unsigned int nr)
{
	if (nr > IRQ_BITMAP_BITS)
		return -ENOMEM;
	nr_irqs = nr;
	return (0);
}

static inline void
irq_free_desc(unsigned int irq)
{
	struct irq_desc *desc = irq_to_desc(irq);
	
	if (irq >= nr_irqs)
		return;

	mutex_lock(&sparse_irq_lock);
	delete_irq_desc(irq);
	bitmap_clear(allocated_irqs, irq, 1);
	mutex_unlock(&sparse_irq_lock);
	call_rcu(&desc->rcu, delayed_free_desc);
}

static inline int
irq_alloc_descs(int irq, unsigned int from, unsigned int cnt, int node)
{
	int start, ret;

	if (!cnt)
		return (-EINVAL);

	if (irq >= 0) {
		if (from > irq)
			return (-EINVAL);
		from = irq;
	}
#ifdef __notyet__
	else {
		from = arch_dynirq_lower_bound(from);
	}
#endif
	mutex_lock(&sparse_irq_lock);

	start = bitmap_find_next_zero_area(allocated_irqs, IRQ_BITMAP_BITS,
					   from, cnt, 0);
	ret = -EEXIST;
	if (irq >=0 && start != irq)
		goto err;

	if (start + cnt > nr_irqs) {
		ret = irq_expand_nr_irqs(start + cnt);
		if (ret)
			goto err;
	}

	bitmap_set(allocated_irqs, start, cnt);
	mutex_unlock(&sparse_irq_lock);
	return (start);

err:
	mutex_unlock(&sparse_irq_lock);
	return ret;
}

#endif
