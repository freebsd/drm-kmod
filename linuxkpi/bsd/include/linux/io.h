#ifndef	_BSD_LKPI_LINUX_IO_H_
#define	_BSD_LKPI_LINUX_IO_H_

#include <asm/set_memory.h>

#include_next <linux/io.h>

#if __FreeBSD_version < 1301507
 
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
#endif

#endif	/* _BSD_LKPI_LINUX_IO_H_ */
