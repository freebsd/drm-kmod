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

#include <linux/debugfs.h>

#include "dummygfx_drv.h"

static struct dentry *debugfs_root;
static const char str[16] = "dummygfx string";


static int seqtest_read(struct seq_file *m, void *arg)
{
	const char *buf;

	buf = (const char *)arg;
	(void)sbuf_printf(m->buf, "%s\n", buf);
	return 0;
}

static int seqtest_open(struct inode *inode, struct file *file)
{

	return single_open(file, seqtest_read, (void *)&str);
}

static int seqtest_release(struct inode *inode, struct file *file)
{

	return single_release(inode, file);
}

static const struct file_operations seqtest_fops = {
	.owner = THIS_MODULE,
	.open = seqtest_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seqtest_release,
};


int dummygfx_debugfs_init()
{
	struct dentry *d;

	debugfs_root = debugfs_create_dir("dummygfx", NULL);
	if (!debugfs_root) {
		DRM_ERROR("Cannot create debugfs root\n");
		return -ENOMEM;
	}
	d = debugfs_create_file("test-seq", S_IRUSR, debugfs_root, NULL, &seqtest_fops);
	if (!d) {
		DRM_ERROR("Cannot create debugfs test-seq\n");
		return -ENOMEM;
	}
	return 0;
}

void dummygfx_debugfs_exit()
{

	debugfs_remove(debugfs_root);
}
