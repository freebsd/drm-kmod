
#ifndef _LINUX_RELAY_H
#define _LINUX_RELAY_H

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/list.h>
/* #include <linux/irq_work.h> */
#include <linux/bug.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/kref.h>
/* #include <linux/percpu.h> */
#include <asm/smp.h>



#define RELAYFS_CHANNEL_VERSION		7


// Naive (not per-cpu) relay channel impl

#define irq_work_sync(x)
#define irq_work_queue(x)
#define init_irq_work(x, y)
#define alloc_percpu(x) kmalloc(mp_ncpus*sizeof(x), GFP_KERNEL)


struct irq_work {
	
};
	
struct rchan_buf
{
	void *start;			/* start of channel buffer */
	void *data;			/* start of current sub-buffer */
	size_t offset;			/* current offset into sub-buffer */
	size_t subbufs_produced;	/* count of sub-buffers produced */
	size_t subbufs_consumed;	/* count of sub-buffers consumed */
	struct rchan *chan;		/* associated channel */
	wait_queue_head_t read_wait;	/* reader wait queue */
	struct irq_work wakeup_work;	/* reader wakeup */
	struct dentry *dentry;		/* channel file dentry */
	struct kref kref;		/* channel buffer refcount */
	struct page **page_array;	/* array of current buffer pages */
	unsigned int page_count;	/* number of current buffer pages */
	unsigned int finalized;		/* buffer has been finalized */
	size_t *padding;		/* padding counts per sub-buffer */
	size_t prev_padding;		/* temporary variable */
	size_t bytes_consumed;		/* bytes consumed in cur read subbuf */
	size_t early_bytes;		/* bytes consumed before VFS inited */
	unsigned int cpu;		/* this buf's cpu */
};

struct rchan
{
	u32 version;			/* the version of this struct */
	size_t subbuf_size;		/* sub-buffer size */
	size_t n_subbufs;		/* number of sub-buffers per buffer */
	size_t alloc_size;		/* total buffer size allocated */
	struct rchan_callbacks *cb;	/* client callbacks */
	struct kref kref;		/* channel refcount */
	void *private_data;		/* for user-defined data */
	size_t last_toobig;		/* tried to log event > subbuf size */
	struct rchan_buf ** __percpu buf; /* per-cpu channel buffers */
	int is_global;			/* One global buffer ? */
	struct list_head list;		/* for channel list */
	struct dentry *parent;		/* parent dentry passed to open */
	int has_base_filename;		/* has a filename associated? */
	char base_filename[NAME_MAX];	/* saved base filename */
};


struct rchan_callbacks {
	int (*subbuf_start) (struct rchan_buf *buf,
						 void *subbuf,
						 void *prev_subbuf,
						 size_t prev_padding);

	void (*buf_mapped)(struct rchan_buf *buf,
					   struct file *filp);
	
	void (*buf_unmapped)(struct rchan_buf *buf,
						 struct file *filp);

	struct dentry *(*create_buf_file)(const char *filename,
									  struct dentry *parent,
									  umode_t mode,
									  struct rchan_buf *buf,
									  int *is_global);

	int (*remove_buf_file)(struct dentry *dentry);
};



struct rchan *relay_open(const char *base_filename,
						 struct dentry *parent,
						 size_t subbuf_size,
						 size_t n_subbufs,
						 struct rchan_callbacks *cb,
						 void *private_data);
extern int relay_late_setup_files(struct rchan *chan,
								  const char *base_filename,
								  struct dentry *parent);
extern void relay_close(struct rchan *chan);
extern void relay_flush(struct rchan *chan);
extern void relay_subbufs_consumed(struct rchan *chan,
								   unsigned int cpu,
								   size_t subbufs_consumed);
extern void relay_reset(struct rchan *chan);
extern int relay_buf_full(struct rchan_buf *buf);

extern size_t relay_switch_subbuf(struct rchan_buf *buf,
								  size_t length);

static inline void __relay_write(struct rchan *chan,
								 const void *data,
								 size_t length) {
	struct rchan_buf *buf;
	int curr_cpu;
	
	curr_cpu = get_cpu();
	buf = chan->buf[curr_cpu];
	if (unlikely(buf->offset + length > buf->chan->subbuf_size))
		length = relay_switch_subbuf(buf, length);
	memcpy((char*)buf->data + buf->offset, data, length);
	buf->offset += length;
	put_cpu();
}

static inline void relay_write(struct rchan *chan,
							   const void *data,
							   size_t length) {
	/* unsigned long flags; */
	/* local_irq_save(flags); */ // NOOP on FreeBSD
	__relay_write(chan, data, length);
	/* local_irq_restore(flags); */
}

static inline void *relay_reserve(struct rchan *chan, size_t length)
{
	void *reserved = NULL;
	int curr_cpu = get_cpu();
	struct rchan_buf *buf = chan->buf[curr_cpu];

	if (unlikely(buf->offset + length > buf->chan->subbuf_size)) {
		length = relay_switch_subbuf(buf, length);
		if (!length)
			goto end;
	}
	reserved = (char*)buf->data + buf->offset;
	buf->offset += length;

end:
	put_cpu();
	return reserved;
}

static inline void subbuf_start_reserve(struct rchan_buf *buf,
										size_t length) {
	BUG_ON(length >= buf->chan->subbuf_size - 1);
	buf->offset = length;
}

extern const struct file_operations relay_file_operations;


int relay_prepare_cpu(unsigned int cpu);


#endif /* _LINUX_RELAY_H */
