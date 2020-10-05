#ifndef _LINUX_GPLV2_COMPILER_H_
#define _LINUX_GPLV2_COMPILER_H_

#include_next <linux/compiler.h>

#include <sys/syslog.h>
#include <linux/types.h>

#define	outb(a,b) outb(b,a)
#define	outw(a,b) outw(b,a)
#define	outl(a,b) outl(b,a)

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

// Changed in base linuxkpi in 13.0-CURRENT
#ifdef __deprecated
#undef __deprecated
#endif
#define	__deprecated

// BSDFIXME! (everything to end of this file)

#include <linux/bitops.h>

// XXX: Move to better place?

static inline void *
memset32(uint32_t *s, uint32_t v, size_t count)
{
	uint32_t *xs = s;

	while (count--)
		*xs++ = v;
	return s;
}

static inline void *
memset64(uint64_t *s, uint64_t v, size_t count)
{
	uint64_t *xs = s;

	while (count--)
		*xs++ = v;
	return s;
}

static inline void *
memset_p(void **p, void *v, size_t n)
{
	if (BITS_PER_LONG == 32)
		return memset32((uint32_t *)p, (uintptr_t)v, n);
	else
		return memset64((uint64_t *)p, (uintptr_t)v, n);
}

#include <sys/types.h>
#include <linux/math64.h>


#define	atomic_fetch_inc(v)	(atomic_inc_return(v) - 1)

static inline int
pfn_valid(unsigned long pfn)
{
	return 1;
}

struct linux_kmem_cache;
static inline int
kmem_cache_shrink(struct linux_kmem_cache *c)
{
	UNIMPLEMENTED();
	return 0;
}

static inline uint64_t mul_u64_u32_div(uint64_t a, uint32_t mul, uint32_t divisor)
{
	union {
		uint64_t ll;
		struct {
#ifdef __BIG_ENDIAN
			uint32_t high, low;
#else
			uint32_t low, high;
#endif
		} l;
	} u, rl, rh;

	u.ll = a;
	rl.ll = mul_u32_u32(u.l.low, mul);
	rh.ll = mul_u32_u32(u.l.high, mul) + rl.l.high;

	/* Bits 32-63 of the result will be in rh.l.low. */
	rl.l.high = do_div(rh.ll, divisor);

	/* Bits 0-31 of the result will be in rl.l.low.	*/
	do_div(rl.ll, divisor);

	rl.l.high = rh.l.low;
	return rl.ll;
}

static inline uint64_t mul_u64_u32_shr(uint64_t a, uint32_t mul, unsigned int shift)
{
	return (uint64_t)((a * mul) >> shift);
}

/* Copied from Linux */
static inline unsigned long array_index_mask_nospec(unsigned long index,
						    unsigned long size)
{
	return ~(long)(index | (size - 1UL - index)) >> (BITS_PER_LONG - 1);
}
/* Copied from Linux */
#define array_index_nospec(index, size)					\
({									\
	typeof(index) _i = (index);					\
	typeof(size) _s = (size);					\
	unsigned long _mask = array_index_mask_nospec(_i, _s);		\
									\
	BUILD_BUG_ON(sizeof(_i) > sizeof(long));			\
	BUILD_BUG_ON(sizeof(_s) > sizeof(long));			\
									\
	(typeof(_i)) (_i & _mask);					\
})

#endif /* _LINUX_GPLV2_COMPILER_H_ */
