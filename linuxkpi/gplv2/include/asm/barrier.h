#ifndef _ASM_BARRIER_H
#define _ASM_BARRIER_H

#include <asm/processor.h>

#define smp_store_release(p, v)					\
do {									\
	smp_mb();							\
	WRITE_ONCE(*p, v);						\
} while (0)


#define smp_load_acquire(p)						\
({									\
	typeof(*p) ___p1 = READ_ONCE(*p);				\
	smp_mb();										\
	___p1;								\
})


#endif /* _ASM_BARRIER_H */
