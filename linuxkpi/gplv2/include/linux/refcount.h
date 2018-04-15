#ifndef _LINUX_REFCOUNT_H
#define _LINUX_REFCOUNT_H

#include <linux/atomic.h>


static inline bool
refcount_dec_and_test(atomic_t *r)
{

	return atomic_dec_and_test(r);
}


#endif /* _LINUX_REFCOUNT_H */
