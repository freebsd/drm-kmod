/*
 * Copyright (C) 2001 Momchil Velikov
 * Portions Copyright (C) 2001 Christoph Hellwig
 * Copyright (C) 2005 SGI, Christoph Lameter
 * Copyright (C) 2006 Nick Piggin
 * Copyright (C) 2012 Konstantin Khlebnikov
 * Copyright (C) 2016 Intel, Matthew Wilcox
 * Copyright (C) 2016 Intel, Ross Zwisler
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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/radix-tree.h>
#include <linux/radix-tree-iter.h>
#include <linux/err.h>

static inline void __set_iter_shift(struct radix_tree_iter *iter,
					unsigned int shift)
{
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	iter->shift = shift;
#endif
}

static unsigned int iter_offset(const struct radix_tree_iter *iter)
{
	return (iter->index >> iter_shift(iter)) & RADIX_TREE_MAP_MASK;
}

static inline struct radix_tree_node *entry_to_node(void *ptr)
{
	return (void *)((unsigned long)ptr & ~RADIX_TREE_INTERNAL_NODE);
}

static inline unsigned long shift_maxindex(unsigned int shift)
{
	return (RADIX_TREE_MAP_SIZE << shift) - 1;
}

static inline unsigned long node_maxindex(const struct radix_tree_node *node)
{
	return shift_maxindex(node->shift);
}

static unsigned radix_tree_load_root(const struct radix_tree_root *root,
		struct radix_tree_node **nodep, unsigned long *maxindex)
{
	struct radix_tree_node *node = rcu_dereference_raw(root->rnode);

	*nodep = node;

	if (likely(radix_tree_is_internal_node(node))) {
		node = entry_to_node(node);
		*maxindex = node_maxindex(node);
		return node->shift + RADIX_TREE_MAP_SHIFT;
	}

	*maxindex = 0;
	return 0;
}

static __always_inline unsigned long
radix_tree_find_next_bit(struct radix_tree_node *node, unsigned int tag,
			 unsigned long offset)
{
	const unsigned long *addr = node->tags[tag];

	if (offset < RADIX_TREE_MAP_SIZE) {
		unsigned long tmp;

		addr += offset / BITS_PER_LONG;
		tmp = *addr >> (offset % BITS_PER_LONG);
		if (tmp)
			return __ffs(tmp) + offset;
		offset = (offset + BITS_PER_LONG) & ~(BITS_PER_LONG - 1);
		while (offset < RADIX_TREE_MAP_SIZE) {
			tmp = *++addr;
			if (tmp)
				return __ffs(tmp) + offset;
			offset += BITS_PER_LONG;
		}
	}
	return RADIX_TREE_MAP_SIZE;
}

static inline void *node_to_entry(void *ptr)
{
	return (void *)((unsigned long)ptr | RADIX_TREE_INTERNAL_NODE);
}

#define RADIX_TREE_RETRY	node_to_entry(NULL)

#ifdef CONFIG_RADIX_TREE_MULTIORDER
/* Sibling slots point directly to another slot in the same node */
static inline
bool is_sibling_entry(const struct radix_tree_node *parent, void *node)
{
	void __rcu **ptr = node;
	return (parent->slots <= ptr) &&
			(ptr < parent->slots + RADIX_TREE_MAP_SIZE);
}
#else
static inline
bool is_sibling_entry(const struct radix_tree_node *parent, void *node)
{
	return false;
}
#endif

/* Construct iter->tags bit-mask from node->tags[tag] array */
static void set_iter_tags(struct radix_tree_iter *iter,
				struct radix_tree_node *node, unsigned offset,
				unsigned tag)
{
	unsigned tag_long = offset / BITS_PER_LONG;
	unsigned tag_bit  = offset % BITS_PER_LONG;

	if (!node) {
		iter->tags = 1;
		return;
	}

	iter->tags = node->tags[tag][tag_long] >> tag_bit;

	/* This never happens if RADIX_TREE_TAG_LONGS == 1 */
	if (tag_long < RADIX_TREE_TAG_LONGS - 1) {
		/* Pick tags from next element */
		if (tag_bit)
			iter->tags |= node->tags[tag][tag_long + 1] <<
						(BITS_PER_LONG - tag_bit);
		/* Clip chunk size, here only BITS_PER_LONG tags */
		iter->next_index = __radix_tree_iter_add(iter, BITS_PER_LONG);
	}
}

static inline int tag_get(const struct radix_tree_node *node, unsigned int tag,
		int offset)
{
	return test_bit(offset, __DECONST(unsigned long *, node->tags[tag]));
}

static inline int root_tag_get(const struct radix_tree_root *root, unsigned tag)
{
	return (__force int)root->gfp_mask & (1 << (tag + ROOT_TAG_SHIFT));
}

static unsigned int radix_tree_descend(const struct radix_tree_node *parent,
			struct radix_tree_node **nodep, unsigned long index)
{
	unsigned int offset = (index >> parent->shift) & RADIX_TREE_MAP_MASK;
	void __rcu **entry = rcu_dereference_raw(parent->slots[offset]);

#ifdef CONFIG_RADIX_TREE_MULTIORDER
	if (radix_tree_is_internal_node(entry)) {
		if (is_sibling_entry(parent, entry)) {
			void __rcu **sibentry;
			sibentry = (void __rcu **) entry_to_node(entry);
			offset = get_slot_offset(parent, sibentry);
			entry = rcu_dereference_raw(*sibentry);
		}
	}
#endif

	*nodep = (void *)entry;
	return offset;
}

void __rcu **radix_tree_next_chunk(const struct radix_tree_root *root,
			     struct radix_tree_iter *iter, unsigned flags)
{
	unsigned tag = flags & RADIX_TREE_ITER_TAG_MASK;
	struct radix_tree_node *node, *child;
	unsigned long index, offset, maxindex;

	if ((flags & RADIX_TREE_ITER_TAGGED) && !root_tag_get(root, tag))
		return NULL;

	/*
	 * Catch next_index overflow after ~0UL. iter->index never overflows
	 * during iterating; it can be zero only at the beginning.
	 * And we cannot overflow iter->next_index in a single step,
	 * because RADIX_TREE_MAP_SHIFT < BITS_PER_LONG.
	 *
	 * This condition also used by radix_tree_next_slot() to stop
	 * contiguous iterating, and forbid switching to the next chunk.
	 */
	index = iter->next_index;
	if (!index && iter->index)
		return NULL;

 restart:
	radix_tree_load_root(root, &child, &maxindex);
	if (index > maxindex)
		return NULL;
	if (!child)
		return NULL;

	if (!radix_tree_is_internal_node(child)) {
		/* Single-slot tree */
		iter->index = index;
		iter->next_index = maxindex + 1;
		iter->tags = 1;
		iter->node = NULL;
		__set_iter_shift(iter, 0);
		return __DECONST(void __rcu **, &root->rnode);
	}

	do {
		node = entry_to_node(child);
		offset = radix_tree_descend(node, &child, index);

		if ((flags & RADIX_TREE_ITER_TAGGED) ?
				!tag_get(node, tag, offset) : !child) {
			/* Hole detected */
			if (flags & RADIX_TREE_ITER_CONTIG)
				return NULL;

			if (flags & RADIX_TREE_ITER_TAGGED)
				offset = radix_tree_find_next_bit(node, tag,
						offset + 1);
			else
				while (++offset	< RADIX_TREE_MAP_SIZE) {
					void *slot = rcu_dereference_raw(
							node->slots[offset]);
					if (is_sibling_entry(node, slot))
						continue;
					if (slot)
						break;
				}
			index &= ~node_maxindex(node);
			index += offset << node->shift;
			/* Overflow after ~0UL */
			if (!index)
				return NULL;
			if (offset == RADIX_TREE_MAP_SIZE)
				goto restart;
			child = rcu_dereference_raw(node->slots[offset]);
		}

		if (!child)
			goto restart;
		if (child == RADIX_TREE_RETRY)
			break;
	} while (radix_tree_is_internal_node(child));

	/* Update the iterator state */
	iter->index = (index &~ node_maxindex(node)) | (offset << node->shift);
	iter->next_index = (index | node_maxindex(node)) + 1;
	iter->node = node;
	__set_iter_shift(iter, node->shift);

	if (flags & RADIX_TREE_ITER_TAGGED)
		set_iter_tags(iter, node, offset, tag);

	return node->slots + offset;
}
