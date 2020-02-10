#ifndef _LINUX_GPLV2_MM_TYPES_H_
#define	_LINUX_GPLV2_MM_TYPES_H_

#include_next <linux/mm_types.h>

#if __FreeBSD_version < 1300077
static inline bool
mmget_not_zero(struct mm_struct *mm)
{
	return (atomic_inc_not_zero(&mm->mm_users));
}
#endif

#endif /* _LINUX_GPLV2_MM_TYPES_H_ */

