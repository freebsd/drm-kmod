#ifndef __LINUX_RANDOMGPLV2_H__
#define	__LINUX_RANDOMGPLV2_H__

#include_next <linux/random.h>

static inline u32 next_pseudo_random32(u32 seed)
{
        return seed * 1664525 + 1013904223;
}

#endif
