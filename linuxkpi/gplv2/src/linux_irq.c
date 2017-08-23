#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>

DECLARE_BITMAP(allocated_irqs, IRQ_BITMAP_BITS);
int nr_irqs;
DEFINE_MUTEX(irq_domain_mutex); 
struct irq_domain *irq_default_domain;

DEFINE_MUTEX(revmap_trees_mutex);
LIST_HEAD(irq_domain_list);
DEFINE_IDR(irq_idr);
DEFINE_MUTEX(sparse_irq_lock);

struct irq_chip no_irq_chip  = {
	.name		= "none",
};
