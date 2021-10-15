#ifndef _BSD_LKPI_LINUX_SORT_H_
#define	_BSD_LKPI_LINUX_SORT_H_

#include <linux/types.h>

void sort(void *base, size_t num, size_t size,
	  int (*cmp)(const void *, const void *),
	  void (*swap)(void *, void *, int));

#endif	/* _BSD_LKPI_LINUX_SORT_H_ */
