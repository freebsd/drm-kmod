#ifndef	_LINUX_GPLV2_IO_H_
#define	_LINUX_GPLV2_IO_H_

#include_next <linux/io.h>
 
#if defined(__amd64__) || defined(__i386__) || defined(__powerpc__)
extern int arch_io_reserve_memtype_wc(resource_size_t start, resource_size_t size);
extern void arch_io_free_memtype_wc(resource_size_t start, resource_size_t size);
#endif

#endif	/* _LINUX_IO_H_ */
