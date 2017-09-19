#ifndef _LINUX_FS_NEXT_H_
#define	_LINUX_FS_NEXT_H_

#include <linux/capability.h>
#include <linux/mutex.h>
#include <linux/shrinker.h>

#include_next <linux/fs.h>

static inline void
i_size_write(void *inode, uint64_t size)
{
}

extern loff_t default_llseek(struct file *file, loff_t offset, int whence);
extern loff_t generic_file_llseek(struct file *file, loff_t offset, int whence);

struct simple_attr {
	struct sbuf *sb;	/* must be first */
	int (*get)(void *, u64 *);
	int (*set)(void *, u64);
	void *data;
	const char *fmt;	/* format for read operation */
	struct mutex mutex;	/* protects access to these buffers */
};

extern ssize_t simple_read_from_buffer(void __user *to, size_t count,
			loff_t *ppos, const void *from, size_t available);
extern ssize_t simple_write_to_buffer(void *to, size_t available, loff_t *ppos,
		const void __user *from, size_t count);

static inline loff_t fixed_size_llseek(struct file *file, loff_t offset,
				       int whence, loff_t size)
{
	panic("%s unimplemented", __FUNCTION__);
}


static inline __printf(1, 2)
void __simple_attr_check_format(const char *fmt, ...)
{
	/* don't do anything, just let the compiler check the arguments; */
}

int simple_attr_open(struct inode *inode, struct file *file,
		     int (*get)(void *, u64 *), int (*set)(void *, u64),
		     const char *fmt);
int simple_attr_release(struct inode *inode, struct file *file);
ssize_t simple_attr_read(struct file *file, char __user *buf,
			 size_t len, loff_t *ppos);
ssize_t simple_attr_write(struct file *file, const char __user *buf,
			  size_t len, loff_t *ppos);


#define DEFINE_SIMPLE_ATTRIBUTE(__fops, __get, __set, __fmt)		\
static int __fops ## _open(struct inode *inode, struct file *file)	\
{									\
	__simple_attr_check_format(__fmt, 0ull);			\
	return simple_attr_open(inode, file, __get, __set, __fmt);	\
}									\
static const struct file_operations __fops = {				\
	.owner	 = THIS_MODULE,						\
	.open	 = __fops ## _open,					\
	.release = simple_attr_release,					\
	.read	 = simple_attr_read,					\
	.write	 = simple_attr_write,					\
	.llseek	 = generic_file_llseek,					\
}

#endif /* _LINUX_FS_NEXT_H_ */
