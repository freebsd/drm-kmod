#ifndef _LINUX_IRQFLAGS_H_
#define	_LINUX_IRQFLAGS_H_

#define	local_irq_disable(void)		do { } while (0)
#define	local_irq_enable(void)		do { } while (0)
#define	local_save_flags(void)		0
#define	local_irq_restore(flags)	do { } while (0)
#define	local_irq_save(flags)		do { (flags) = 0; } while (0)

#endif					/* _LINUX_IRQFLAGS_H_ */
