/*
 * Copyright (c) 2015 Michael Neumann
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

#ifndef _LINUX_SEQ_FILE_H_
#define _LINUX_SEQ_FILE_H_

#include <linux/types.h>
#include <linux/bug.h>
#include <linux/fs.h>
#include <sys/sbuf.h>

struct seq_operations;

struct seq_file {
	struct sbuf	*buf;

	const struct seq_operations *op;
	const struct linux_file *file;
	void *private;
};

struct seq_operations {
	void * (*start) (struct seq_file *m, loff_t *pos);
	void (*stop) (struct seq_file *m, void *v);
	void * (*next) (struct seq_file *m, void *v, loff_t *pos);
	int (*show) (struct seq_file *m, void *v);
};

ssize_t linux_seq_read(struct file *, char __user *, size_t, loff_t *);
int seq_write(struct seq_file *seq, const void *data, size_t len);

loff_t seq_lseek(struct file *file, loff_t offset, int whence);
int single_open(struct file *, int (*)(struct seq_file *, void *), void *);
int single_release(struct inode *, struct file *);

#define seq_printf(m, fmt, ...) sbuf_printf((m)->buf, (fmt), ##__VA_ARGS__)

#define seq_puts(m, str)	sbuf_printf((m)->buf, str)
#define seq_putc(m, str)	sbuf_putc((m)->buf, str)
#define seq_read		linux_seq_read


#endif	/* _LINUX_SEQ_FILE_H_ */
