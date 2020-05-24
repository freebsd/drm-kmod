#ifndef _LINUX_REFCOUNT_DRM_H
#define _LINUX_REFCOUNT_DRM_H

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

#endif /* _LINUX_REFCOUNT_DRM_H */
