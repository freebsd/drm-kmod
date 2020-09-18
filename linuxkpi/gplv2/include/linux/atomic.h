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

#endif
