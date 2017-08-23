#include <linux/kernel.h>
#include <linux/seq_file.h>

ssize_t
linux_seq_read(struct file *f, char __user *ubuf, size_t size, loff_t *ppos)
{
	struct seq_file *m = f->private_data;
	void *p;
	int rc;
	loff_t pos = 0;

	p = m->op->start(m, &pos);
	rc = m->op->show(m, p);		
	if (rc)
		return (rc);
	return (size);
}

int
seq_write(struct seq_file *seq, const void *data, size_t len)
{

	return (sbuf_bcpy(seq->buf, data, len));
}

/*
 * This only needs to be a valid address for lkpi 
 * drivers it should never actually be called
 */
loff_t
seq_lseek(struct file *file, loff_t offset, int whence)
{
	panic("%s not supported\n", __FUNCTION__);
	return (0);
}

static void *
single_start(struct seq_file *p, loff_t *pos)
{
	return NULL + (*pos == 0);
}

static void *
single_next(struct seq_file *p, void *v, loff_t *pos)
{
	++*pos;
	return NULL;
}

static void
single_stop(struct seq_file *p, void *v)
{
}

static int
seq_open(struct file *f, const struct seq_operations *op)
{
	struct seq_file *p;

	WARN_ON(f->private_data);

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	f->private_data = p;
	p->op = op;
	p->file = f;
	return (0);
}

int
single_open(struct file *f, int (*show)(struct seq_file *, void *), void *d)
{
	struct seq_operations *op;
	int rc = -ENOMEM;

	op = kmalloc(sizeof(*op), GFP_KERNEL);
	if (op) {
		op->start = single_start;
		op->next = single_next;
		op->stop = single_stop;
		op->show = show;
		rc = seq_open(f, op);
		if (rc)
			kfree(op);
		else
			((struct seq_file *)f->private_data)->private = d;

	}
	return (rc);
}

static int
seq_release(struct inode *inode, struct file *file)
{
	struct seq_file *m;

	m = file->private_data;
	kfree(m);
	return (0);
}

int
single_release(struct vnode *v, struct file *f)
{
	const struct seq_operations *op = ((struct seq_file *)f->private_data)->op;
	int rc;

	rc = seq_release(v, f);
	kfree(op);
	return (rc);
}
