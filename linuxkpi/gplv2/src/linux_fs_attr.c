#include <sys/types.h>
#include <sys/sbuf.h>

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>

ssize_t
simple_read_from_buffer(void __user * to, size_t count,
    loff_t *ppos, const void *from, size_t available)
{
	loff_t pos = *ppos;
	size_t ret;

	if ((int64_t)pos < 0)
		return (-EINVAL);
	if (pos >= available || !count)
		return (0);
	if (count > available - pos)
		count = available - pos;
	ret = copyout(from, to + pos, count);
	if (ret != 0)
		return (-EFAULT);
	*ppos = pos + count;
	return (0);
}

ssize_t
simple_write_to_buffer(void *to, size_t available, loff_t *ppos,
    const void __user * from, size_t count)
{
	loff_t pos = *ppos;
	size_t res;

	if ((int64_t)pos < 0)
		return (-EINVAL);
	if (pos >= available || !count)
		return (0);
	if (count > available - pos)
		count = available - pos;
	res = copyin(from, to + pos, count);
	if (res != 0)
		return (-EFAULT);
	*ppos = pos + count;
	return (0);
}

int
simple_attr_open(struct inode *inode, struct file *file,
    int (*get) (void *, u64 *), int (*set) (void *, u64),
    const char *fmt)
{
	struct simple_attr *attr;

	attr = kmalloc(sizeof(*attr), GFP_KERNEL);
	if (!attr)
		return -ENOMEM;

	attr->get = get;
	attr->set = set;
	attr->data = inode->i_private;
	attr->fmt = fmt;
	mutex_init(&attr->mutex);

	file->private_data = attr;

	return (nonseekable_open(inode, file));
}

int
simple_attr_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return (0);
}

ssize_t
simple_attr_read(struct file *file, char __user * buf, size_t len, loff_t *ppos)
{
	struct sbuf *sb;
	struct simple_attr *attr;
	ssize_t ret;

	attr = file->private_data;

	if (!attr->get)
		return -EACCES;

	ret = mutex_lock_interruptible(&attr->mutex);
	if (ret)
		return ret;

	sb = attr->sb;
	if (*ppos == 0) {
		u64 val;

		ret = attr->get(attr->data, &val);
		if (ret)
			goto out;
		(void)sbuf_printf(sb, attr->fmt, (unsigned long long)val);
	}
out:
	mutex_unlock(&attr->mutex);
	return ret;
}

ssize_t
simple_attr_write(struct file *file, const char *buf, size_t len, loff_t *ppos)
{
	struct sbuf *sb;
	struct simple_attr *attr;
	u64 val;
	ssize_t ret;

	attr = file->private_data;
	if (!attr->set)
		return -EACCES;

	ret = mutex_lock_interruptible(&attr->mutex);
	if (ret)
		return ret;

	sb = attr->sb;
	(void)sbuf_finish(sb);
	val = simple_strtoll(sbuf_data(sb), NULL, 0);
	ret = attr->set(attr->data, val);
	mutex_unlock(&attr->mutex);
	return (ret);
}
