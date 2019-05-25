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

	return single_open(file, testseq_show, (void *)inode->i_private);
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
static ssize_t testseqrw_write(struct file *file, const char __user *ubuf, size_t len, loff_t *offp)
{
	printf("%s\n", __func__);
	copyin(ubuf, str, min(len, 10));
	return 0;
}
static int testseqrw_open(struct inode *inode, struct file *file)
{
	printf("%s\n", __func__);

	return single_open(file, testseqrw_show, (void *)inode->i_private);
}
static const struct file_operations testseqrw_fops = {
	.owner = THIS_MODULE,
	.open = testseqrw_open,
	.read = seq_read,
	.write = testseqrw_write,
	.llseek = seq_lseek,
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
	printf("%s: file->private_data %p, private=%s\n", __func__, p, (char *)p);
	/* private_data always overwritten with seq_file for read functions... */
	return 0;
}
static ssize_t customrw_write(struct file *file, const char __user *ubuf, size_t len, off_t *offp)
{
	printf("%s\n", __func__);
	void *p = file->private_data;
	char *kbuf = malloc(len, M_DEVBUF, M_WAITOK);
	copyin(ubuf, kbuf, len);
	printf("%s: file->private_data %p, private=%s, kbuf=%s\n", __func__, p, (char *)p, kbuf);
	free(kbuf, M_DEVBUF);
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
 * simple-rw test
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


/*
 * simple test
 */
static const struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.open = simple_open,
	.release = simple_release,
};



/*
 * attr test
 */
static int attr_val = 12345;

static int
attr_get(void *data, u64 *val)
{
	printf("%s: data %p\n", __func__, data);

	*val = attr_val;
	return 0;
}
static int
attr_set(void *data, u64 val)
{
	printf("%s: data %p\n", __func__, data);

	attr_val = val;
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(attr_fops, attr_get, attr_set, "%llu\n");


int dummygfx_debugfs_init()
{
	printf("%s\n", __func__);
	struct dentry *d;

	str = malloc(10, M_DEVBUF, M_WAITOK);
	strncpy(str, "dummygfx", 9);

	printf("%s: str %p, str=%s\n", __func__, str, str);

	debugfs_root = debugfs_create_dir("dummygfx", NULL);
	if (!debugfs_root) {
		DRM_ERROR("Cannot create debugfs root\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("test-seq", S_IRUSR, debugfs_root, str, &testseq_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs test-seq\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("test-seq-rw", S_IRUSR | S_IWUSR, debugfs_root, str, &testseqrw_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs test-seq-rw\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("custom-rw", S_IRUSR | S_IWUSR, debugfs_root, str, &customrw_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs custom-rw\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("simple-rw", S_IRUSR | S_IWUSR, debugfs_root, str, &simplerw_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs simple\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("simple", S_IRUSR | S_IWUSR, debugfs_root, str, &simple_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs simple-rw\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("attr", S_IRUSR | S_IWUSR, debugfs_root, str, &attr_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs attr\n");
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
