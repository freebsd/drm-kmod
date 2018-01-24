/*
 * Copyright (C) 2001 Momchil Velikov
 * Portions Copyright (C) 2001 Christoph Hellwig
 * Copyright (C) 2006 Nick Piggin
 * Copyright (C) 2012 Konstantin Khlebnikov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _LINUX_RADIX_TREE_ITER_H
#define _LINUX_RADIX_TREE_ITER_H

#include <linux/bitops.h>
#include <linux/compiler.h>
#include <linux/rcupdate.h>
#include <linux/radix-tree.h>

#define	RADIX_TREE_ENTRY_MASK		3UL
#define	RADIX_TREE_INTERNAL_NODE	1UL
#define	RADIX_TREE_EXCEPTIONAL_ENTRY	2
#define	RADIX_TREE_EXCEPTIONAL_SHIFT	2
#define	ROOT_IS_IDR	((__force gfp_t)(1 << __GFP_BITS_SHIFT))
#define	ROOT_TAG_SHIFT	(__GFP_BITS_SHIFT + 1)

struct radix_tree_iter {
	unsigned long	index;
	unsigned long	next_index;
	unsigned long	tags;
	struct radix_tree_node *node;
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	unsigned int	shift;
#endif
};

enum {
	RADIX_TREE_ITER_TAG_MASK = 0x0f,	/* tag index in lower nybble */
	RADIX_TREE_ITER_TAGGED   = 0x10,	/* lookup tagged slots */
	RADIX_TREE_ITER_CONTIG   = 0x20,	/* stop at first hole */
};

static inline int radix_tree_exception(void *arg)
{
	return unlikely((unsigned long)arg & RADIX_TREE_ENTRY_MASK);
}

static inline bool radix_tree_is_internal_node(void *ptr)
{
	return ((unsigned long)ptr & RADIX_TREE_ENTRY_MASK) ==
				RADIX_TREE_INTERNAL_NODE;
}

static inline unsigned int iter_shift(const struct radix_tree_iter *iter)
{
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	return iter->shift;
#else
	return 0;
#endif
}


void __rcu **radix_tree_next_chunk(const struct radix_tree_root *,
			     struct radix_tree_iter *iter, unsigned flags);



static __always_inline void __rcu **
radix_tree_iter_init(struct radix_tree_iter *iter, unsigned long start)
{
	/*
	 * Leave iter->tags uninitialized. radix_tree_next_chunk() will fill it
	 * in the case of a successful tagged chunk lookup.  If the lookup was
	 * unsuccessful or non-tagged then nobody cares about ->tags.
	 *
	 * Set index to zero to bypass next_index overflow protection.
	 * See the comment in radix_tree_next_chunk() for details.
	 */
	iter->index = 0;
	iter->next_index = start;
	return NULL;
}


static inline void __rcu **
radix_tree_iter_lookup(const struct radix_tree_root *root,
			struct radix_tree_iter *iter, unsigned long index)
{
	radix_tree_iter_init(iter, index);
	return radix_tree_next_chunk(root, iter, RADIX_TREE_ITER_CONTIG);
}

static inline void __rcu **
radix_tree_iter_find(const struct radix_tree_root *root,
			struct radix_tree_iter *iter, unsigned long index)
{
	radix_tree_iter_init(iter, index);
	return radix_tree_next_chunk(root, iter, 0);
}

static inline __must_check
void __rcu **radix_tree_iter_retry(struct radix_tree_iter *iter)
{
	iter->next_index = iter->index;
	iter->tags = 0;
	return NULL;
}

static inline unsigned long
__radix_tree_iter_add(struct radix_tree_iter *iter, unsigned long slots)
{
	return iter->index + (slots << iter_shift(iter));
}

void __rcu **__must_check radix_tree_iter_resume(void __rcu **slot,
					struct radix_tree_iter *iter);

static __always_inline long
radix_tree_chunk_size(struct radix_tree_iter *iter)
{
	return (iter->next_index - iter->index) >> iter_shift(iter);
}

#ifdef CONFIG_RADIX_TREE_MULTIORDER
void __rcu **__radix_tree_next_slot(void __rcu **slot,
				struct radix_tree_iter *iter, unsigned flags);
#else
/* Can't happen without sibling entries, but the compiler can't tell that */
static inline void __rcu **__radix_tree_next_slot(void __rcu **slot,
				struct radix_tree_iter *iter, unsigned flags)
{
	return slot;
}
#endif

static __always_inline void __rcu **radix_tree_next_slot(void __rcu **slot,
				struct radix_tree_iter *iter, unsigned flags)
{
	if (flags & RADIX_TREE_ITER_TAGGED) {
		iter->tags >>= 1;
		if (unlikely(!iter->tags))
			return NULL;
		if (likely(iter->tags & 1ul)) {
			iter->index = __radix_tree_iter_add(iter, 1);
			slot++;
			goto found;
		}
		if (!(flags & RADIX_TREE_ITER_CONTIG)) {
			unsigned offset = __ffs(iter->tags);

			iter->tags >>= offset++;
			iter->index = __radix_tree_iter_add(iter, offset);
			slot += offset;
			goto found;
		}
	} else {
		long count = radix_tree_chunk_size(iter);

		while (--count > 0) {
			slot++;
			iter->index = __radix_tree_iter_add(iter, 1);

			if (likely(*slot))
				goto found;
			if (flags & RADIX_TREE_ITER_CONTIG) {
				/* forbid switching to the next chunk */
				iter->next_index = 0;
				break;
			}
		}
	}
	return NULL;

 found:
	if (unlikely(radix_tree_is_internal_node(rcu_dereference_raw(*slot))))
		return __radix_tree_next_slot(slot, iter, flags);
	return slot;
}


#define radix_tree_for_each_slot(slot, root, iter, start)		\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter, 0)) ;	\
	     slot = radix_tree_next_slot(slot, iter, 0))

#endif /* _LINUX_RADIX_TREE_ITER_H */
