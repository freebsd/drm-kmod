#ifndef __GPLV2_ATOMIC_H__
#define	__GPLV2_ATOMIC_H__

#include_next <linux/atomic.h>

#define try_cmpxchg(p, op, n)						\
({									\
	__typeof(p) __op = (__typeof((p)))(op);				\
	__typeof(*(p)) __o = *__op;					\
	__typeof(*(p)) __p = __sync_val_compare_and_swap((p), (__o), (n)); \
	if (__p != __o)							\
		*__op = __p;						\
	(__p == __o);							\
})

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
