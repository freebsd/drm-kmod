/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  linux/include/linux/rbtree.h

  To use rbtrees you'll have to implement your own insert and search cores.
  This will avoid us to use callbacks and to drop drammatically performances.
  I know it's not the cleaner way,  but in C (not in C++) to get
  performances and genericity...

  See Documentation/rbtree.txt for documentation and samples.
*/

#ifndef	_LINUX_RBTREE_H
#define	_LINUX_RBTREE_H

/*
 * Include some FreeBSD headers which depend on tree.h.
 */
#include <sys/param.h>
#include <sys/socket.h>

#include <net/if.h>
#include <net/if_var.h>

#include <netinet/in.h>
#include <netinet/in_var.h>

#undef RB_ROOT
#undef RB_RED
#undef RB_BLACK

#include <linux/stddef.h>
#include <linux/rcupdate.h>

struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));

struct rb_root {
	struct rb_node *rb_node;
};


#define rb_parent(r)   ((struct rb_node *)((r)->__rb_parent_color & ~3))

#define RB_ROOT	(struct rb_root) { NULL, }
#define	rb_entry(ptr, type, member) container_of(ptr, type, member)

#define RB_EMPTY_ROOT(root)  (READ_ONCE((root)->rb_node) == NULL)

/* 'empty' nodes are nodes that are known not to be inserted in an rbtree */
#define RB_EMPTY_NODE(node)  \
	((node)->__rb_parent_color == (unsigned long)(node))
#define RB_CLEAR_NODE(node)  \
	((node)->__rb_parent_color = (unsigned long)(node))

static inline struct rb_node *
rb_first(const struct rb_root *root)
{
	struct rb_node	*n;

	n = __DECONST(struct rb_node *, root->rb_node);
	if (n == NULL)
		return (NULL);
	while (n->rb_left)
		n = n->rb_left;
	return (n);
}

static inline struct rb_node *
rb_last(const struct rb_root *root)
{
	struct rb_node	*n;

	n = __DECONST(struct rb_node *, root->rb_node);
	if (n == NULL)
		return (NULL);
	while (n->rb_right)
		n = n->rb_right;
	return (n);
}

static inline struct rb_node *
rb_next(const struct rb_node *node)
{
	struct rb_node *parent;
	struct rb_node *n = __DECONST(struct rb_node *, node);

	if (RB_EMPTY_NODE(n))
		return (NULL);

	if (n->rb_right) {
		n = n->rb_right; 
		while (n->rb_left)
			n = n->rb_left;
		return (n);
	}

	while ((parent = rb_parent(n)) && n == parent->rb_right)
		n = parent;

	return (parent);
}

static inline struct rb_node *
rb_prev(const struct rb_node *node)
{
	struct rb_node *parent;
	struct rb_node *n = __DECONST(struct rb_node *, node);

	if (RB_EMPTY_NODE(n))
		return (NULL);

	if (n->rb_left) {
		n = n->rb_left; 
		while (n->rb_right)
			n = n->rb_right;
		return (n);
	}

	while ((parent = rb_parent(n)) && n == parent->rb_left)
		n = parent;

	return (parent);
}

extern void rb_replace_node_rcu(struct rb_node *victim, struct rb_node *new,
				struct rb_root *root);

static inline void
rb_link_node(struct rb_node *node, struct rb_node *parent,
	     struct rb_node **rb_link)
{
	node->__rb_parent_color = (unsigned long)parent;
	node->rb_left = node->rb_right = NULL;

	*rb_link = node;
}

static inline void
rb_link_node_rcu(struct rb_node *node, struct rb_node *parent,
		 struct rb_node **rb_link)
{
	node->__rb_parent_color = (unsigned long)parent;
	node->rb_left = node->rb_right = NULL;

	rcu_assign_pointer(*rb_link, node);
}

#define rb_entry_safe(ptr, type, member) \
	({ typeof(ptr) ____ptr = (ptr); \
	   ____ptr ? rb_entry(____ptr, type, member) : NULL; \
	})

static inline struct rb_node *
rb_left_deepest_node(const struct rb_node *node)
{
	struct rb_node *n = __DECONST(struct rb_node *, node);
	
	for (;;) {
		if (n->rb_left)
			n = n->rb_left;
		else if (n->rb_right)
			n = node->rb_right;
		else
			return (n);
	}
}

/* Postorder iteration - always visit the parent after its children */
static inline struct rb_node *
rb_next_postorder(const struct rb_node *node)
{
	struct rb_node *parent;

	if (node == NULL)
		return (NULL);
	parent = rb_parent(node);

	if (parent && (node == parent->rb_left) && parent->rb_right) {
		return rb_left_deepest_node(parent->rb_right);
	} else
		return (parent);
}

static inline struct rb_node *
rb_first_postorder(const struct rb_root *root)
{
	if (!root->rb_node)
		return (NULL);

	return (rb_left_deepest_node(root->rb_node));
}
/**
 * rbtree_postorder_for_each_entry_safe - iterate in post-order over rb_root of
 * given type allowing the backing memory of @pos to be invalidated
 *
 * @pos:	the 'type *' to use as a loop cursor.
 * @n:		another 'type *' to use as temporary storage
 * @root:	'rb_root *' of the rbtree.
 * @field:	the name of the rb_node field within 'type'.
 *
 * rbtree_postorder_for_each_entry_safe() provides a similar guarantee as
 * list_for_each_entry_safe() and allows the iteration to continue independent
 * of changes to @pos by the body of the loop.
 *
 * Note, however, that it cannot handle other modifications that re-order the
 * rbtree it is iterating over. This includes calling rb_erase() on @pos, as
 * rb_erase() may rebalance the tree, causing us to miss some nodes.
 */
#define rbtree_postorder_for_each_entry_safe(pos, n, root, field) \
	for (pos = rb_entry_safe(rb_first_postorder(root), typeof(*pos), field); \
	     pos && ({ n = rb_entry_safe(rb_next_postorder(&pos->field), \
			typeof(*pos), field); 1; }); \
	     pos = n)

#include <linux/rbtree_augmented.h>

#endif	/* _LINUX_RBTREE_H */
