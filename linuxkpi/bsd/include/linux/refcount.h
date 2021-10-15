#ifndef _BSD_LKPI_LINUX_REFCOUNT_H_
#define	_BSD_LKPI_LINUX_REFCOUNT_H_

#include_next <linux/refcount.h>

/*
 * We can't change this right now as struct kref from linuxkpi base doesn't
 * use refcount_t but an atomic_t directly
 */
static inline bool
refcount_dec_and_test(atomic_t *r)
{

	return atomic_dec_and_test(r);
}

#endif /* _BSD_LKPI_LINUX_REFCOUNT_H_ */
