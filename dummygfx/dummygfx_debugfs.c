/*-
 * Copyright (c) 2019 Johannes Lundberg <johalun@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/seq_file.h>
#include <linux/debugfs.h>

#include "dummygfx_drv.h"

static struct dentry *debugfs_root;
static char *str;


/*
 * single_open seq_file read only test
 */
static int testseq_show(struct seq_file *m, void *unused)
{
	printf("%s\n", __func__);
	char *buf;

	buf = m->private;
	(void)sbuf_printf(m->buf, "%s\n", buf);
	return 0;
}
static int testseq_open(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);
	
	return single_open(file, testseq_show, (void *)str);
}
static int testseq_release(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);

	return single_release(inode, file);
}
static const struct file_operations testseq_fops = {
	.owner = THIS_MODULE,
	.open = testseq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = testseq_release,
};

/*
 * single_open seq_file read/write test
 */
static int testseqrw_show(struct seq_file *m, void *unused)
{
	printf("%s\n", __func__);
	char *buf;

	buf = m->private;
	(void)sbuf_printf(m->buf, "%s\n", buf);
	return 0;
}
static int testseqrw_open(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);
	
	return single_open(file, testseqrw_show, (void *)str);
}
static ssize_t testseqrw_write(struct file *file, const char __user *ubuf, size_t len, loff_t *offp)
{
	printf("%s\n", __func__);
	/* struct seq_file *m = file->private_data; */
	/* struct drm_i915_private *dev_priv = m->private; */
	return 0;
}
static int testseqrw_release(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);

	return single_release(inode, file);
}
static const struct file_operations testseqrw_fops = {
	.owner = THIS_MODULE,
	.open = testseqrw_open,
	.read = seq_read,
	.write = testseqrw_write,
	.llseek = seq_lseek,
	.release = testseqrw_release,
};



/*
 * custom open read/write test
 */
static int customrw_open(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);
	void *p = inode->i_private;
	printf("%s: inode->i_private %p\n", __func__, p);

	file->private_data = inode->i_private;
	return 0;
}
static ssize_t customrw_read(struct file *file, char __user *ubuf, size_t len, off_t *offp)
{
	printf("%s\n", __func__);
	void *p = file->private_data;
	printf("%s: file->private_data %p, %s\n", __func__, p, (char *)p);
	return 0;
}
static ssize_t customrw_write(struct file *file, const char __user *ubuf, size_t len, off_t *offp)
{
	printf("%s\n", __func__);
	void *p = file->private_data;
	printf("%s: file->private_data %p, %s, ubuf %s\n", __func__, p, (char *)p, ubuf);
	return 0;
}
static int customrw_release(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);
	void *p = inode->i_private;
	printf("%s: inode->i_private %p\n", __func__, p);
	return 0;
}
static const struct file_operations customrw_fops = {
	.owner = THIS_MODULE,
	.open = customrw_open,
	.read = customrw_read,
	.write = customrw_write,
	.release = customrw_release,
};


/*
 * simple test
 */
static int simplerw_open(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);
	void *p = inode->i_private;

	printf("%s: inode->i_private %p\n", __func__, p);
	return 0;
}

static int simplerw_release(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);
	void *p = inode->i_private;

	printf("%s: inode->i_private %p\n", __func__, p);
	return 0;
}

static const struct file_operations simplerw_fops = {
	.owner = THIS_MODULE,
	.open = simplerw_open,
	.release = simplerw_release,
};



int dummygfx_debugfs_init()
{
	printf("%s\n", __func__);
	struct dentry *d;

	str = malloc(10, M_DEVBUF, M_WAITOK);
	strncpy(str, "dummygfx", 9);

	printf("%s: str %p\n", __func__, str);

	debugfs_root = debugfs_create_dir("dummygfx", NULL);
	if (!debugfs_root) {
		DRM_ERROR("Cannot create debugfs root\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("test-seq", S_IRUSR, debugfs_root, NULL, &testseq_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs test-seq\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("test-seq-rw", S_IRUSR | S_IWUSR, debugfs_root, NULL, &testseqrw_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs test-seq-rw\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("custom-rw", S_IRUSR | S_IWUSR, debugfs_root, str, &customrw_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs custom-rw\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("simple", S_IRUSR | S_IWUSR, debugfs_root, str, &simplerw_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs simple\n");
		return -ENOMEM;
	}
	return 0;
}

void dummygfx_debugfs_exit()
{
	printf("%s\n", __func__);

	free(str, M_DEVBUF);
	debugfs_remove(debugfs_root);
}
