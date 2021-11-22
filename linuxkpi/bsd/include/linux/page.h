#include_next <linux/page.h>
#undef clflush
#define	clflush(addr)	clflush((unsigned long)(addr))
