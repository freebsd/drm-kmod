#include <sys/param.h>
#include <vm/vm.h>
#include <vm/pmap.h>
#include_next <linux/page.h>
#undef clflush
#define	clflush(addr)	clflush((unsigned long)(addr))
#undef clflush_cache_range
#define	clflush_cache_range(addr, size)				\
	pmap_force_invalidate_cache_range(			\
	    (vm_offset_t)(addr), (vm_offset_t)(addr) + (size))
