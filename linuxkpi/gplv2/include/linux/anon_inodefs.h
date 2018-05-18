#ifndef _LINUX_GPLV2_ANON_INODEFS_H_
#define _LINUX_GPLV2_ANON_INODEFS_H_

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/types.h>

struct dentry *anon_inodefs_create_file(const char *name, umode_t mode,
    struct dentry *parent, void *data,
    const struct file_operations *fops);

void anon_inodefs_remove(struct dentry *dentry);

#endif
