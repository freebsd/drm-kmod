#ifndef _BSD_LKPI_LINUX_KERNEL_H_
#define	_BSD_LKPI_LINUX_KERNEL_H_

#include_next <linux/kernel.h>

/* XXX */
#define	irqs_disabled() (curthread->td_critnest != 0 || curthread->td_intr_nesting_level != 0)

#define add_taint(a,b)

#include <linux/irqflags.h>
#include <linux/kconfig.h>
#include <linux/typecheck.h>

#include <asm/cpufeature.h>
#include <asm/processor.h>

#endif /* _BSD_LKPI_LINUX_KERNEL_H_ */
