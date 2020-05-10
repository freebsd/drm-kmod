#ifndef __LINUX_BITFIELD_H__
#define	__LINUX_BITFIELD_H__

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

#define FIELD_GET(_mask, _reg)	\
	(typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask))

#endif
