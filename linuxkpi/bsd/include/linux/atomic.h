#ifndef _BSD_LKPI_LINUX_ATOMIC_H_
#define	_BSD_LKPI_LINUX_ATOMIC_H_

#include_next <linux/atomic.h>

#ifndef try_cmpxchg
#define try_cmpxchg(p, op, n)						\
({									\
	__typeof(p) __op = (__typeof((p)))(op);				\
	__typeof(*(p)) __o = *__op;					\
	__typeof(*(p)) __p = __sync_val_compare_and_swap((p), (__o), (n)); \
	if (__p != __o)							\
		*__op = __p;						\
	(__p == __o);							\
})
#endif

#ifndef atomic_try_cmpxchg
#define __atomic_try_cmpxchg(type, _p, _po, _n)				\
({									\
	typeof(_po) __po = (_po);					\
	typeof(*(_po)) __r, __o = *__po;				\
	__r = atomic_cmpxchg##type((_p), __o, (_n));			\
	if (unlikely(__r != __o))					\
		*__po = __r;						\
	likely(__r == __o);						\
})

#define atomic_try_cmpxchg(_p, _po, _n)	__atomic_try_cmpxchg(, _p, _po, _n)
#endif

#ifndef mb
#define	mb()	__asm __volatile("mfence;" : : : "memory")
#endif
#ifndef wmb
#define	wmb()	__asm __volatile("sfence;" : : : "memory")
#endif
#ifndef rmb
#define	rmb()	__asm __volatile("lfence;" : : : "memory")
#endif

#ifndef smp_mb
#define	smp_mb()	mb()
#endif
#ifndef smp_wmb
#define	smp_wmb()	wmb()
#endif
#ifndef smp_rmb
#define	smp_rmb()	rmb()
#endif

#ifndef	smp_store_mb
#define	__smp_store_mb(var, value) do { (void)xchg(&(var), value); } while (0)
#define	smp_store_mb __smp_store_mb
#endif

#ifndef smp_mb__before_atomic
#define	smp_mb__before_atomic()	barrier()
#endif

#ifndef smp_mb__after_atomic
#define	smp_mb__after_atomic()	barrier()
#endif

#endif	/* _BSD_LKPI_LINUX_ATOMIC_H_ */
