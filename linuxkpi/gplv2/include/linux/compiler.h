#ifndef _LINUX_GPLV2_COMPILER_H_
#define _LINUX_GPLV2_COMPILER_H_

#include_next <linux/compiler.h>

#include <sys/syslog.h>
#include <linux/types.h>
#include <asm/atomic.h>

#ifndef PRINT_UNIMPLEMENTED
#define PRINT_UNIMPLEMENTED 1
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

#define	DODGY_ONCE() do {			\
	static int seen = 0;			\
						\
	if (!seen && PRINT_UNIMPLEMENTED) {	\
		log(LOG_WARNING,		\
		    "%s is dodgy -- see your local kernel hacker\n", \
		    __FUNCTION__);		\
		seen = 1;			\
	}					\
} while (0)

#undef UNIMPLEMENTED /* is defined to NOP in kernel lkpi */
#define	UNIMPLEMENTED()	UNIMPLEMENTED_ONCE()
#define	WARN_NOT()	UNIMPLEMENTED_ONCE()
#define	DODGY()		DODGY_ONCE()

#define	unreachable()	__unreachable()

static inline void *
memset_p(void **p, void *v, size_t n)
{
	return memset((uintptr_t *)p, (uintptr_t)v, n);
}

// BSDFIXME!
#define	idr_init_base(idr,base)	idr_init(idr)
#define	atomic_fetch_inc(v)	atomic_fetch_add(1, (v))

struct linux_kmem_cache;
static inline int
kmem_cache_shrink(struct linux_kmem_cache *c)
{
	UNIMPLEMENTED();
	return 0;
}

#endif /* _LINUX_GPLV2_COMPILER_H_ */
