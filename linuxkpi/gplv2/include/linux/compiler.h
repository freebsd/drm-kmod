#ifndef _LINUX_GPLV2_COMPILER_H_
#define _LINUX_GPLV2_COMPILER_H_

#include_next <linux/compiler.h>

#include <sys/syslog.h>
#include <linux/types.h>

#ifndef outb
#define	outb(a,b) outb(b,a)
#endif
#ifndef outw
#define	outw(a,b) outw(b,a)
#endif
#ifndef outl
#define	outl(a,b) outl(b,a)
#endif

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

// Changed in base linuxkpi in 13.0-CURRENT
#ifdef __deprecated
#undef __deprecated
#endif
#define	__deprecated

// BSDFIXME! (everything to end of this file)

#include <linux/bitops.h>

// XXX: Move to better place?

#if __FreeBSD_version < 1301507
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
#endif

#include <sys/types.h>
#include <linux/math64.h>

#if __FreeBSD_version < 1301507
#define	atomic_fetch_inc(v)	(atomic_inc_return(v) - 1)
#endif

#if __FreeBSD_version < 1301507
struct linux_kmem_cache;
static inline int
kmem_cache_shrink(struct linux_kmem_cache *c)
{
	UNIMPLEMENTED();
	return 0;
}
#endif



#if __FreeBSD_version < 1301507

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
#endif

#if __FreeBSD_version < 1301507
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
#endif

#endif /* _LINUX_GPLV2_COMPILER_H_ */
