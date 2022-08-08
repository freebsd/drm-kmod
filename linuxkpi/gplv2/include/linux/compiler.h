#ifndef _LINUX_GPLV2_COMPILER_H_
#define _LINUX_GPLV2_COMPILER_H_

#include_next <linux/compiler.h>

#include <sys/syslog.h>
#include <linux/types.h>
#include <linux/math64.h>

#ifndef PRINT_UNIMPLEMENTED
#define PRINT_UNIMPLEMENTED 0
#endif

#define	UNIMPLEMENTED_ONCE() do {		\
	static int seen = 0;			\
						\
	if (!seen && PRINT_UNIMPLEMENTED) {	\
		log(LOG_WARNING,		\
		    "%s not implemented -- see your local kernel hacker\n", \
		    __FUNCTION__);		\
		seen = 1;			\
	}					\
} while (0)

#undef UNIMPLEMENTED /* is defined to NOP in kernel lkpi */
#define	UNIMPLEMENTED()	UNIMPLEMENTED_ONCE()

struct linux_kmem_cache;
static inline int
kmem_cache_shrink(struct linux_kmem_cache *c)
{
	UNIMPLEMENTED();
	return 0;
}

#endif /* _LINUX_GPLV2_COMPILER_H_ */
