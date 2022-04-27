#include_next <linux/rwsem.h>

#ifndef down_write_nest_lock
#define	down_write_nest_lock(sem, _rw)	down_write(_rw)
#endif
