#ifndef _BSD_LKPI_LINUX_UACCESS_H
#define	_BSD_LKPI_LINUX_UACCESS_H

#include_next <linux/uaccess.h>

#ifndef __copy_to_user_inatomic_nocache
static inline int
__copy_to_user_inatomic(void __user *to, const void *from, unsigned n)
{
	return (copyout_nofault(from, to, n) != 0 ? n : 0);
}
#define	__copy_to_user_inatomic_nocache(to, from, n) \
    __copy_to_user_inatomic((to), (from), (n))
#endif

#ifndef __copy_from_user_inatomic_nocache
static inline unsigned long
__copy_from_user_inatomic(void *to, const void __user *from,
    unsigned long n)
{
	/*
	 * XXXKIB.  Equivalent Linux function is implemented using
	 * MOVNTI for aligned moves.  For unaligned head and tail,
	 * normal move is performed.  As such, it is not incorrect, if
	 * only somewhat slower, to use normal copyin.  All uses
	 * except shmem_pwrite_fast() have the destination mapped WC.
	 */
	return ((copyin_nofault(__DECONST(void *, from), to, n) != 0 ? n : 0));
}
#define	__copy_from_user_inatomic_nocache(to, from, n) \
    __copy_from_user_inatomic((to), (from), (n))
#endif

#endif /* _BSD_LKPI_LINUX_UACCESS_H */
