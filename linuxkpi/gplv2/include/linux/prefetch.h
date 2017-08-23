#ifndef _LINUX_PREFETCH_H
#define _LINUX_PREFETCH_H

#include <linux/types.h>
#include <asm/processor.h>

#ifndef ARCH_HAS_PREFETCH
#define prefetch(x) __builtin_prefetch(x)
#endif

#ifndef ARCH_HAS_PREFETCHW
#define prefetchw(x) __builtin_prefetch(x,1)
#endif

#endif
