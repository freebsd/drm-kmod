#ifndef _LINUX_HIGHMEM_H_
#define _LINUX_HIGHMEM_H_

#include <linux/page.h>

#define PageHighMem(p) (0)

static inline struct page *kmap_to_page(void *addr)
{
	return virt_to_page(addr);
}

#endif
