#ifndef	_LINUX_GPLV2_IO_H_
#define	_LINUX_GPLV2_IO_H_

#include <asm/set_memory.h>

#include_next <linux/io.h>
 
#if defined(__amd64__) || defined(__i386__) || defined(__aarch64__) || defined(__powerpc__) || defined(__riscv)
static inline int
arch_io_reserve_memtype_wc(resource_size_t start, resource_size_t size)
{
	return (set_memory_wc(start, size >> PAGE_SHIFT));
}

static inline void
arch_io_free_memtype_wc(resource_size_t start, resource_size_t size)
{
	set_memory_wb(start, size >> PAGE_SHIFT);
}
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
