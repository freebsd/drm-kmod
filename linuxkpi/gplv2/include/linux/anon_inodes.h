#ifndef _LINUX_GPLV2_ANON_INODES_H_
#define _LINUX_GPLV2_ANON_INODES_H_

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/types.h>


struct file *
anon_inode_getfile(const char *name, const struct file_operations *fops,
    void *priv, int flags);

void anon_inode_release(struct file *file);

/*
// Only used by i915_perf.c so far (which is disabled)..
int
anon_inode_getfd(const char *name, const struct file_operations *fops,
				 void *priv, int flags);
*/

#endif
