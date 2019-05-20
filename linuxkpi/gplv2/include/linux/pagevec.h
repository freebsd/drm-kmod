/*
 * include/linux/pagevec.h
 *
 * In many places it is efficient to batch an operation up against multiple
 * pages.  A pagevec is a multipage container which is used for that.
 */

#ifndef _LINUX_PAGEVEC_H
#define _LINUX_PAGEVEC_H

#include <linux/pagemap.h>

/* 14 pointers + two long's align the pagevec structure to a power of two */
#define PAGEVEC_SIZE	14

struct page;
struct address_space;

struct pagevec {
	unsigned long nr;
	struct page *pages[PAGEVEC_SIZE];
};

static inline void pagevec_init(struct pagevec *pvec)
{
	pvec->nr = 0;
}

static inline void pagevec_reinit(struct pagevec *pvec)
{
	pvec->nr = 0;
}

static inline unsigned pagevec_count(struct pagevec *pvec)
{
	return pvec->nr;
}

static inline unsigned pagevec_space(struct pagevec *pvec)
{
	return PAGEVEC_SIZE - pvec->nr;
}

/*
 * Add a page to a pagevec.  Returns the number of slots still available.
 */
static inline unsigned pagevec_add(struct pagevec *pvec, struct page *page)
{
	pvec->pages[pvec->nr++] = page;
	return pagevec_space(pvec);
}

static inline void __pagevec_release(struct pagevec *pvec)
{
	release_pages(pvec->pages, pagevec_count(pvec));
	pagevec_reinit(pvec);
}

static inline void pagevec_release(struct pagevec *pvec)
{
	if (pagevec_count(pvec))
		__pagevec_release(pvec);
}

static inline void
check_move_unevictable_pages(struct pagevec *pvec)
{

	UNIMPLEMENTED();
}

static inline void
mapping_clear_unevictable(vm_object_t mapping)
{

	UNIMPLEMENTED();
}


#endif /* _LINUX_PAGEVEC_H */
