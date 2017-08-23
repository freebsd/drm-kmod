#ifdef __notyet__
#define RB_AUGMENT(entry) interval_tree_augment_entry(entry)


#include <linux/interval_tree.h>
#include <linux/kernel.h>


#undef RB_ROOT
#define	RB_ROOT(head)	(head)->rbh_root

#define INTERVAL_MAGIC  0xCAFEBABEDEADF00D

static void
interval_tree_augment_entry(struct interval_tree_node *entry)
{
	unsigned long max, subtree_last;
	struct interval_tree_node *l, *r;

	MPASS(entry->it_magic == INTERVAL_MAGIC);
	for (max = entry->last; entry != NULL; entry = RB_PARENT(entry, rb_entry)) {
		MPASS(entry->it_magic == INTERVAL_MAGIC);
		if ((l = RB_LEFT(entry, rb_entry))) {
			subtree_last = l->__subtree_last;
			max = MAX(subtree_last, max);
		}
		if ((r = RB_LEFT(entry, rb_entry))) {
			subtree_last = r->__subtree_last;
			max = MAX(subtree_last, max);
		}
		if (entry->__subtree_last == max)
			break;
		entry->__subtree_last = max;		
	}
}

static int
it_cmp(struct interval_tree_node *a, struct interval_tree_node *b)
{
	MPASS(a->it_magic == INTERVAL_MAGIC);
	MPASS(b->it_magic == INTERVAL_MAGIC);

	if (a->start < b->start)
		return (-1);
	if (b->start < a->start)
		return (1);
	return (0);
}

RB_HEAD(interval_tree, interval_tree_node);
RB_GENERATE_STATIC(interval_tree, interval_tree_node, rb_entry, it_cmp);

static struct interval_tree_node *
interval_tree_sub_find(struct interval_tree_node *node, unsigned long start,
    unsigned long end)
{
	struct interval_tree_node *l, *r;

	MPASS(node->it_magic == INTERVAL_MAGIC);
	while (true) {
		MPASS(node->it_magic == INTERVAL_MAGIC);
		if ((l = RB_LEFT(node, rb_entry))) {
			if (start <= l->__subtree_last) {
				node = l;
				continue;
			}
		}
		if (node->start <= end) {
			if (start <= node->last)
				return node;
			if ((r = RB_RIGHT(node, rb_entry))) {
				node = r;
				if (start <= node->__subtree_last)
					continue;
			}
		}
		return (NULL);
	}					
}

static void
interval_tree_insert_(struct interval_tree_node *node, struct rb_root *root)
{
	struct rb_node **link = &root->rb_node;
	struct interval_tree_node *entry =  (struct interval_tree_node *)root->rb_node;
	unsigned long start = node->start, last = node->last;
	struct interval_tree_node *parent = node;

	node->it_magic = INTERVAL_MAGIC;

	while (entry != NULL) {
		MPASS(entry->it_magic == INTERVAL_MAGIC);
		parent = RB_PARENT(entry, rb_entry);
		MPASS(parent->it_magic == INTERVAL_MAGIC);
		if (parent->__subtree_last < last)
			parent->__subtree_last = last;
		if (start < parent->start)
			entry = RB_LEFT(parent, rb_entry);
		else
			entry = RB_RIGHT(parent, rb_entry);
	}

	node->__subtree_last = last;
	rb_link_node((struct rb_node *)&node->rb, (struct rb_node *)parent, link);
	interval_tree_RB_INSERT((struct interval_tree *)root, node);
}

void
interval_tree_insert(struct interval_tree_node *node, struct rb_root *root)
{
	interval_tree_insert_(node, root);
	interval_tree_remove(node, root);
	interval_tree_insert_(node, root);
}


void
interval_tree_remove(struct interval_tree_node *entry, struct rb_root *root)
{
	MPASS(entry->it_magic == INTERVAL_MAGIC);
	interval_tree_RB_REMOVE((struct interval_tree *)root, entry);
	if ((uintptr_t)root->rb_node == (uintptr_t)entry)
		root->rb_node = NULL;
}

struct interval_tree_node *
interval_tree_iter_first(struct rb_root *root, uint64_t start, uint64_t end)
{
	struct interval_tree_node *node;

	if (root->rb_node == NULL)
		return (NULL);
	node =(struct interval_tree_node *)root->rb_node;
	MPASS(node->it_magic == INTERVAL_MAGIC);
	if (node->__subtree_last < start)
		return (NULL);
	return (interval_tree_sub_find(node, start, end));
}

struct interval_tree_node *
interval_tree_iter_next(struct interval_tree_node *node,
			unsigned long start, unsigned long last)
{
	struct interval_tree_node *cur, *prev, *r;

	cur = RB_RIGHT(node, rb_entry);
	do {
		MPASS(cur->it_magic == INTERVAL_MAGIC);
		if (cur) {
			r = cur;
			if (start <= r->__subtree_last)
				return (interval_tree_sub_find(r, start, last));
		}
		do {
			cur = RB_PARENT(node, rb_entry);
			if (cur == NULL)
				return (NULL);
			prev = node;
			node = cur;
			cur = RB_RIGHT(node, rb_entry);
		} while (prev == cur);

		if (last < node->start)
			return (NULL);
		else if (start <= node->last)
			return (node);
	} while (true);

	return (NULL);
}
#else
#include <linux/interval_tree.h>
#include <linux/interval_tree_generic.h>
#include <linux/kernel.h>

#define START(node) ((node)->start)
#define LAST(node)  ((node)->last)

static void
interval_tree__insert(struct interval_tree_node *node, struct rb_root *root);

static void
interval_tree__remove(struct interval_tree_node *node, struct rb_root *root);

static struct interval_tree_node *
interval_tree__iter_first(struct rb_root *root,
			 unsigned long start, unsigned long last);

static struct interval_tree_node *
interval_tree__iter_next(struct interval_tree_node *node,
			unsigned long start, unsigned long last);



INTERVAL_TREE_DEFINE(struct interval_tree_node, rb,
                     unsigned long, __subtree_last,
                     START, LAST,, interval_tree_)

void
interval_tree_insert(struct interval_tree_node *node, struct rb_root *root)
{
	interval_tree__insert(node, root);
}

void
interval_tree_remove(struct interval_tree_node *node, struct rb_root *root)
{
	interval_tree__remove(node, root);
}

struct interval_tree_node *
interval_tree_iter_first(struct rb_root *root,
			 unsigned long start, unsigned long last)
{
	return interval_tree__iter_first(root, start, last);
}

struct interval_tree_node *
interval_tree_iter_next(struct interval_tree_node *node,
			unsigned long start, unsigned long last)
{
	return interval_tree__iter_next(node, start, last);
}
#endif
