#ifndef	_LINUX_GPLV2_IO_H_
#define	_LINUX_GPLV2_IO_H_

#include_next <linux/io.h>
 
#if defined(__amd64__) || defined(__i386__) || defined(__aarch64__) || defined(__powerpc__)
extern int arch_io_reserve_memtype_wc(resource_size_t start, resource_size_t size);
extern void arch_io_free_memtype_wc(resource_size_t start, resource_size_t size);
#endif


/* 32 bit fallback of readq/writeq from Linux */
/* TODO: Clean up and move to base io.h */
static inline uint64_t lo_hi_readq(const volatile void *addr)
{
	const volatile uint32_t *p = addr;
	uint32_t low, high;

	low = readl(p);
	high = readl(p + 1);

	return low + ((uint64_t)high << 32);
}
#ifndef readq
#define readq(addr) lo_hi_readq(addr)
#endif

static inline void lo_hi_writeq(uint64_t val, volatile void *addr)
{
	volatile uint32_t *p = addr;

        writel(val, addr);
	writel(val >> 32, p + 1);
}
#ifndef writeq
#define writeq(val, addr) lo_hi_writeq(val, addr)
#endif

#endif	/* _LINUX_IO_H_ */
