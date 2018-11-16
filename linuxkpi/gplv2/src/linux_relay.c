#include <sys/param.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <fs/pseudofs/pseudofs.h>

#include <linux/seq_file.h>
#include <linux/relay.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/dcache.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

/* list of open channels, for cpu hotplug */
static DEFINE_MUTEX(relay_channels_mutex);
static LIST_HEAD(relay_channels);


/* 
 Default channel callbacks
 */

static int subbuf_start_default_callback (struct rchan_buf *buf,
					  void *subbuf,
					  void *prev_subbuf,
					  size_t prev_padding) {
	printf("%s: WARNING! Default callback is used\n", __func__);
	if (relay_buf_full(buf))
		return 0;
	return 1;
}

static void buf_mapped_default_callback(struct rchan_buf *buf,
					struct file *filp) {
	printf("%s: WARNING! Default callback is used\n", __func__);
}

static void buf_unmapped_default_callback(struct rchan_buf *buf,
					  struct file *filp) {
	printf("%s: WARNING! Default callback is used\n", __func__);
}

static struct dentry *create_buf_file_default_callback(const char *filename,
						       struct dentry *parent,
						       umode_t mode,
						       struct rchan_buf *buf,
						       int *is_global) {
	printf("%s: WARNING! Default callback is used\n", __func__);
	return NULL;
}

static int remove_buf_file_default_callback(struct dentry *dentry)
{
	printf("%s: WARNING! Default callback is used\n", __func__);
	return -EINVAL;
}

static struct rchan_callbacks default_channel_callbacks = {
	.subbuf_start = subbuf_start_default_callback,
	.buf_mapped = buf_mapped_default_callback,
	.buf_unmapped = buf_unmapped_default_callback,
	.create_buf_file = create_buf_file_default_callback,
	.remove_buf_file = remove_buf_file_default_callback,
};

static void setup_callbacks(struct rchan *chan,
				   struct rchan_callbacks *cb) {
	printf("%s\n", __func__);
	if(!cb) {
		printf("%s: WARNING! Default callback is used\n", __func__);
		chan->cb = &default_channel_callbacks;
		return;
	}

	if(!cb->subbuf_start)
		cb->subbuf_start = subbuf_start_default_callback;
	if(!cb->buf_mapped)
		cb->buf_mapped = buf_mapped_default_callback;
	if(!cb->buf_unmapped)
		cb->buf_unmapped = buf_unmapped_default_callback;
	if(!cb->create_buf_file)
		cb->create_buf_file = create_buf_file_default_callback;
	if(!cb->remove_buf_file)
		cb->remove_buf_file = remove_buf_file_default_callback;
	chan->cb = cb;
}



static struct page **relay_alloc_page_array(unsigned int n_pages)
{
	printf("%s\n", __func__);
	const size_t pa_size = n_pages * sizeof(struct page *);
	if (pa_size > PAGE_SIZE)
		return vzalloc(pa_size);
	return kzalloc(pa_size, GFP_KERNEL);
}

static void relay_free_page_array(struct page **array)
{
	printf("%s\n", __func__);
	kvfree(array);
}

static inline void relay_set_buf_dentry(struct rchan_buf *buf,
					struct dentry *dentry)
{
	printf("%s\n", __func__);
	buf->dentry = dentry;

	// XXX How to deal with this?
	/* d_inode(buf->dentry)->i_size = buf->early_bytes; */
}

struct rchan_percpu_buf_dispatcher {
	struct rchan_buf *buf;
	struct dentry *dentry;
};

__unused static void __relay_set_buf_dentry(void *info)
{
	struct rchan_percpu_buf_dispatcher *p = info;

	relay_set_buf_dentry(p->buf, p->dentry);
}

static void *relay_alloc_buf(struct rchan_buf *buf, size_t *size)
{
	printf("%s\n", __func__);
	void *mem;
	unsigned int i, j, n_pages;

	*size = PAGE_ALIGN(*size);
	n_pages = *size >> PAGE_SHIFT;

	buf->page_array = relay_alloc_page_array(n_pages);
	if (!buf->page_array)
		return NULL;

	for (i = 0; i < n_pages; i++) {
		buf->page_array[i] = alloc_page(GFP_KERNEL);
		if (unlikely(!buf->page_array[i]))
			goto depopulate;
		// Not on FreeBSD...
		/* set_page_private(buf->page_array[i], (unsigned long)buf); */
	}
	mem = vmap(buf->page_array, n_pages, VM_MAP, PAGE_KERNEL);
	if (!mem)
		goto depopulate;

	memset(mem, 0, *size);
	buf->page_count = n_pages;
	return mem;

depopulate:
	for (j = 0; j < i; j++)
		__free_page(buf->page_array[j]);
	relay_free_page_array(buf->page_array);
	return NULL;
}


static struct dentry *relay_create_buf_file(struct rchan *chan,
											struct rchan_buf *buf,
											unsigned int cpu) {
	printf("%s\n", __func__);
	struct dentry *dentry;
	char *tmpname;

	tmpname = kzalloc(NAME_MAX + 1, GFP_KERNEL);
	if (!tmpname)
		return NULL;
	snprintf(tmpname, NAME_MAX, "%s%d", chan->base_filename, cpu);

	printf("%s: Creating file with name: %s\n", __func__, tmpname);

	/* Create file in fs */
	dentry = chan->cb->create_buf_file(tmpname,
									   chan->parent,
									   S_IRUSR,
									   buf,
									   &chan->is_global);
	kfree(tmpname);
	return dentry;
}


static void relay_destroy_channel(struct kref *kref)
{
	printf("%s\n", __func__);
	struct rchan *chan = container_of(kref, struct rchan, kref);
	kfree(chan);
}

static struct rchan_buf *relay_create_buf(struct rchan *chan)
{
	printf("%s\n", __func__);
	struct rchan_buf *buf;

	if (chan->n_subbufs > UINT_MAX / sizeof(size_t *))
		return NULL;

	buf = kzalloc(sizeof(struct rchan_buf), GFP_KERNEL);
	if (!buf)
		return NULL;
	buf->padding = kmalloc(chan->n_subbufs * sizeof(size_t *), GFP_KERNEL);
	if (!buf->padding)
		goto free_buf;

	buf->start = relay_alloc_buf(buf, &chan->alloc_size);
	if (!buf->start)
		goto free_buf;

	buf->chan = chan;
	kref_get(&buf->chan->kref);
	return buf;

free_buf:
	kfree(buf->padding);
	kfree(buf);
	return NULL;
}

static void relay_destroy_buf(struct rchan_buf *buf)
{
	printf("%s\n", __func__);
	struct rchan *chan = buf->chan;
	unsigned int i;

	if (likely(buf->start)) {
		vunmap(buf->start);
		for (i = 0; i < buf->page_count; i++)
			__free_page(buf->page_array[i]);
		relay_free_page_array(buf->page_array);
	}
	chan->buf = NULL;
	kfree(buf->padding);
	kfree(buf);
	kref_put(&chan->kref, relay_destroy_channel);
}

static void relay_remove_buf(struct kref *kref)
{
	printf("%s\n", __func__);
	struct rchan_buf *buf = container_of(kref, struct rchan_buf, kref);
	relay_destroy_buf(buf);
}

static void __relay_reset(struct rchan_buf *buf, unsigned int init) {
	printf("%s\n", __func__);
	size_t i;

	if (init) {
		init_waitqueue_head(&buf->read_wait);
		kref_init(&buf->kref);
		init_irq_work(&buf->wakeup_work, wakeup_readers);
	} else {
		irq_work_sync(&buf->wakeup_work);
	}

	buf->subbufs_produced = 0;
	buf->subbufs_consumed = 0;
	buf->bytes_consumed = 0;
	buf->finalized = 0;
	buf->data = buf->start;
	buf->offset = 0;

	for (i = 0; i < buf->chan->n_subbufs; i++)
		buf->padding[i] = 0;

	buf->chan->cb->subbuf_start(buf, buf->data, NULL, 0);
}

extern void relay_reset(struct rchan *chan) {

	printf("%s\n", __func__);
	struct rchan_buf *buf;

	if (!chan)
		return;

	if (chan->is_global && (buf = chan->buf[0])) {
		__relay_reset(buf, 0);
		return;
	}

	// XXX: percpu stuff (this ok?) 
	unsigned int i;
	CPU_FOREACH(i) {
		if ((buf = chan->buf[i]))
			__relay_reset(buf, 0);
	}
}

static struct rchan_buf *relay_open_buf(struct rchan *chan, unsigned int cpu)
{
	printf("%s\n", __func__);

 	struct rchan_buf *buf = NULL;
	struct dentry *dentry;

 	if (chan->is_global)
		return chan->buf[0];

	buf = relay_create_buf(chan);
	if (!buf)
		return NULL;

	if (chan->has_base_filename) {
		dentry = relay_create_buf_file(chan, buf, cpu);
		if (!dentry)
			goto free_buf;
		relay_set_buf_dentry(buf, dentry);
	} else {
		/* Only retrieve global info, nothing more, nothing less */
		dentry = chan->cb->create_buf_file(NULL, NULL,
						   S_IRUSR, buf,
						   &chan->is_global);
		if (WARN_ON(dentry))
			goto free_buf;
	}

 	buf->cpu = cpu;
 	__relay_reset(buf, 1);

 	if(chan->is_global) {
		chan->buf[0] = buf;
 		buf->cpu = 0;
  	}

	return buf;

free_buf:
 	relay_destroy_buf(buf);
	return NULL;
}

static void relay_close_buf(struct rchan_buf *buf)
{
	printf("%s\n", __func__);
	buf->finalized = 1;
	irq_work_sync(&buf->wakeup_work);
	buf->chan->cb->remove_buf_file(buf->dentry);
	kref_put(&buf->kref, relay_remove_buf);
}

__unused static void wakeup_readers(struct irq_work *work)
{
	printf("%s\n", __func__);
	struct rchan_buf *buf;
	buf = container_of(work, struct rchan_buf, wakeup_work);
	wake_up_interruptible(&buf->read_wait);
}

int relay_prepare_cpu(unsigned int cpu)
{
	struct rchan *chan;
	struct rchan_buf *buf;

	mutex_lock(&relay_channels_mutex);
	list_for_each_entry(chan, &relay_channels, list) {
		if ((buf = chan->buf[cpu]))
			continue;
		buf = relay_open_buf(chan, cpu);
		if (!buf) {
			pr_err("relay: cpu %d buffer creation failed\n", cpu);
			mutex_unlock(&relay_channels_mutex);
			return -ENOMEM;
		}
		chan->buf[cpu] = buf;
	}
	mutex_unlock(&relay_channels_mutex);
	return 0;
}

struct rchan *relay_open(const char *base_filename,
						 struct dentry *parent,
						 size_t subbuf_size,
						 size_t n_subbufs,
						 struct rchan_callbacks *cb,
						 void *private_data)
{
	printf("%s: Base filename: %s\n", __func__, base_filename);

	unsigned int i;
	struct rchan *chan;
	struct rchan_buf *buf;

	if (!(subbuf_size && n_subbufs))
		return NULL;
	if (subbuf_size > UINT_MAX / n_subbufs)
		return NULL;

	chan = kzalloc(sizeof(struct rchan), GFP_KERNEL);
	if (!chan)
		return NULL;

	chan->buf = alloc_percpu(struct rchan_buf *);
	chan->version = RELAYFS_CHANNEL_VERSION;
	chan->n_subbufs = n_subbufs;
	chan->subbuf_size = subbuf_size;
	chan->alloc_size = PAGE_ALIGN(subbuf_size * n_subbufs);
	chan->parent = parent;
	chan->private_data = private_data;
	if (base_filename) {
		chan->has_base_filename = 1;
		strlcpy(chan->base_filename, base_filename, NAME_MAX);
	}
	setup_callbacks(chan, cb);
	kref_init(&chan->kref);

	mutex_lock(&relay_channels_mutex);
    CPU_FOREACH(i) {
		buf = relay_open_buf(chan, i);
		if (!buf)
			goto free_bufs;
		chan->buf[i] = buf;
	}
	list_add(&chan->list, &relay_channels);
	mutex_unlock(&relay_channels_mutex);
	printf("%s: finished\n", __func__);
	return chan;

free_bufs:
    CPU_FOREACH(i) {
		if ((buf = chan->buf[i]))
			relay_close_buf(buf);
	}

	kref_put(&chan->kref, relay_destroy_channel);
	mutex_unlock(&relay_channels_mutex);
	kfree(chan);
	return NULL;
}

extern int relay_late_setup_files(struct rchan *chan,
								  const char *base_filename,
								  struct dentry *parent) {
	printf("%s: With base filename: %s\n", __func__, base_filename);
	int err = 0;
	unsigned long flags __unused;
	struct dentry *dentry;
	struct rchan_buf *buf;
	unsigned int i, curr_cpu;
	struct rchan_percpu_buf_dispatcher disp __unused;

	if (!chan || !base_filename) {
		printf("%s: ERROR: chan or base_filename not set!\n", __func__);
		return -EINVAL;
	}
	strlcpy(chan->base_filename, base_filename, NAME_MAX);

	mutex_lock(&relay_channels_mutex);
	/* Is chan already set up? */
	if (unlikely(chan->has_base_filename)) {
		printf("%s: ERROR: chan already set up!\n", __func__);
		mutex_unlock(&relay_channels_mutex);
		return -EEXIST;
	}
	chan->has_base_filename = 1;
	chan->parent = parent;
	
	if (chan->is_global) {
		err = -EINVAL;
		buf = chan->buf[0];
		if (!WARN_ON_ONCE(!buf)) {
			dentry = relay_create_buf_file(chan, buf, 0);
			if (dentry && !WARN_ON_ONCE(!chan->is_global)) {
				relay_set_buf_dentry(buf, dentry);
				err = 0;
			}
		}
		mutex_unlock(&relay_channels_mutex);
		return err;
	}

	printf("%s: ERROR: Only global channel supported (for now)\n", __func__);
	mutex_unlock(&relay_channels_mutex);
	return -EINVAL;
	
	curr_cpu = get_cpu();

	CPU_FOREACH(i) {
		buf = chan->buf[i];
		if (unlikely(!buf)) {
			WARN_ONCE(1, KERN_ERR "CPU has no buffer!\n");
			err = -EINVAL;
			break;
		}

		dentry = relay_create_buf_file(chan, buf, i);
		if (unlikely(!dentry)) {
			err = -EINVAL;
			break;
		}

		if (curr_cpu == i) {
			local_irq_save(flags); // NOOP on FreeBSD
			relay_set_buf_dentry(buf, dentry);
			local_irq_restore(flags); // NOOP on FreeBSD
		} else {
			/* relay_set_buf_dentry(buf, dentry); */
			disp.buf = buf;
			disp.dentry = dentry;
			smp_mb();
			printf("%s: ERROR: We have to execute code on other cpu and wait for return here\n", __func__);
			/* struct callout c; */
		    /* callout_init(&c, 1); */
			/* callout_reset_on(&c, 0, __relay_set_buf_dentry, &disp, i); */
			// Wait for return here??
		}
		if (unlikely(err))
			break;
	}
	put_cpu();
	mutex_unlock(&relay_channels_mutex);
	return err;
}

extern void relay_close(struct rchan *chan) {

	printf("%s\n", __func__);
	
	struct rchan_buf *buf;
	unsigned int i;

	if (!chan)
		return;

	mutex_lock(&relay_channels_mutex);
	if (chan->is_global && (buf = chan->buf[0]))
		relay_close_buf(buf);
	else
		CPU_FOREACH(i) {
			if ((buf = chan->buf[i]))
				relay_close_buf(buf);
		}
	if (chan->last_toobig)
		printk(KERN_WARNING "relay: one or more items not logged "
		       "[item size (%zd) > sub-buffer size (%zd)]\n",
		       chan->last_toobig, chan->subbuf_size);

	list_del(&chan->list);
	kref_put(&chan->kref, relay_destroy_channel);
	mutex_unlock(&relay_channels_mutex);
}

extern void relay_flush(struct rchan *chan) {

	printf("%s\n", __func__);

	struct rchan_buf *buf;
	unsigned int i;

	if (!chan)
		return;

	if (chan->is_global && (buf = chan->buf[0])) {
		relay_switch_subbuf(buf, 0);
		return;
	}

	mutex_lock(&relay_channels_mutex);
	CPU_FOREACH(i) {		
		if ((buf = chan->buf[i]))
			relay_switch_subbuf(buf, 0);
	}
	mutex_unlock(&relay_channels_mutex);
}

extern void relay_subbufs_consumed(struct rchan *chan,
								   unsigned int cpu,
								   size_t  subbufs_consumed) {
	printf("%s\n", __func__);

	struct rchan_buf *buf;

	if (!chan || cpu >= mp_ncpus)
		return;

	buf = chan->buf[cpu];
	if (!buf || subbufs_consumed > chan->n_subbufs)
		return;

	if (subbufs_consumed > buf->subbufs_produced - buf->subbufs_consumed)
		buf->subbufs_consumed = buf->subbufs_produced;
	else
		buf->subbufs_consumed += subbufs_consumed;
}


extern int relay_buf_full(struct rchan_buf *buf) {
	printf("%s\n", __func__);
	size_t ready = buf->subbufs_produced - buf->subbufs_consumed;
	return (ready >= buf->chan->n_subbufs) ? 1 : 0;
}

static int relay_buf_empty(struct rchan_buf *buf) {
	printf("%s\n", __func__);
	return (buf->subbufs_produced - buf->subbufs_consumed) ? 0 : 1;
}

extern size_t relay_switch_subbuf(struct rchan_buf *buf,
								  size_t length) {
	printf("%s\n", __func__);
	void *old, *new;
	size_t old_subbuf, new_subbuf;

	if (unlikely(length > buf->chan->subbuf_size))
		goto toobig;

	if (buf->offset != buf->chan->subbuf_size + 1) {
		buf->prev_padding = buf->chan->subbuf_size - buf->offset;
		old_subbuf = buf->subbufs_produced % buf->chan->n_subbufs;
		buf->padding[old_subbuf] = buf->prev_padding;
		buf->subbufs_produced++;
		if (buf->dentry)
			i_size_write(d_inode(buf->dentry), buf->chan->subbuf_size - buf->padding[old_subbuf]);
		else
			buf->early_bytes += buf->chan->subbuf_size -
					    buf->padding[old_subbuf];
		smp_mb();
		if (waitqueue_active(&buf->read_wait)) {
			printf("%s: Wake up readers here (deferred to avoid deadlock)\n", __func__);
			// FIXME!
			/*
			 * Calling wake_up_interruptible() from here
			 * will deadlock if we happen to be logging
			 * from the scheduler (trying to re-grab
			 * rq->lock), so defer it.
			 */
			irq_work_queue(&buf->wakeup_work);
		}
	}

	old = buf->data;
	new_subbuf = buf->subbufs_produced % buf->chan->n_subbufs;
	new = (char*)buf->start + new_subbuf * buf->chan->subbuf_size;
	buf->offset = 0;
	if (!buf->chan->cb->subbuf_start(buf, new, old, buf->prev_padding)) {
		buf->offset = buf->chan->subbuf_size + 1;
		return 0;
	}
	buf->data = new;
	buf->padding[new_subbuf] = 0;

	if (unlikely(length + buf->offset > buf->chan->subbuf_size))
		goto toobig;

	return length;

toobig:
	buf->chan->last_toobig = length;
	return 0;
}





static int relay_file_open(struct inode *inode, struct file *filp) {

	printf("%s: inode=%p, filp=%p, inode->i_private=%p\n", __func__, inode, filp, inode->i_private);

	struct rchan_buf *buf = inode->i_private;
	kref_get(&buf->kref);

	struct seq_file *sf = malloc(sizeof(struct seq_file), M_KMALLOC, M_WAITOK|M_ZERO);
	sf->private = buf;
	filp->private_data = sf;

	return nonseekable_open(inode, filp);
}

/* /\** */
/*  *	relay_file_mmap - mmap file op for relay files */
/*  *	@filp: the file */
/*  *	@vma: the vma describing what to map */
/*  * */
/*  *	Calls upon relay_mmap_buf() to map the file into user space. */
/*  *\/ */
/* static int relay_file_mmap(struct file *filp, struct vm_area_struct *vma) */
/* { */
/* 	struct rchan_buf *buf = filp->private_data; */
/* 	return relay_mmap_buf(buf, vma); */
/* } */


static unsigned int relay_file_poll(struct file *filp, poll_table *wait) {
	printf("%s\n", __func__);
	unsigned int mask = 0;
	
	struct seq_file *sf = filp->private_data;
	struct rchan_buf *buf = sf->private;

	if (buf->finalized)
		return POLLERR;

	if (filp->f_mode & FMODE_READ) {
		poll_wait(filp, &buf->read_wait, wait);
		if (!relay_buf_empty(buf))
			mask |= POLLIN | POLLRDNORM;
	}

	return mask;
}

static int relay_file_release(struct inode *inode, struct file *filp) {

	printf("%s\n", __func__);

	struct seq_file *sf = filp->private_data;
	struct rchan_buf *buf = sf->private;
	
	filp->private_data = NULL;
	free(sf, M_KMALLOC);
	
	kref_put(&buf->kref, relay_remove_buf);
	return 0;
}

static void relay_file_read_consume(struct rchan_buf *buf,
									size_t read_pos,
									size_t bytes_consumed) {
	size_t subbuf_size = buf->chan->subbuf_size;
	size_t n_subbufs = buf->chan->n_subbufs;
	size_t read_subbuf;

	if (buf->subbufs_produced == buf->subbufs_consumed &&
	    buf->offset == buf->bytes_consumed)
		return;

	if (buf->bytes_consumed + bytes_consumed > subbuf_size) {
		relay_subbufs_consumed(buf->chan, buf->cpu, 1);
		buf->bytes_consumed = 0;
	}

	buf->bytes_consumed += bytes_consumed;
	if (!read_pos)
		read_subbuf = buf->subbufs_consumed % n_subbufs;
	else
		read_subbuf = read_pos / buf->chan->subbuf_size;
	if (buf->bytes_consumed + buf->padding[read_subbuf] == subbuf_size) {
		if ((read_subbuf == buf->subbufs_produced % n_subbufs) &&
		    (buf->offset == subbuf_size))
			return;
		relay_subbufs_consumed(buf->chan, buf->cpu, 1);
		buf->bytes_consumed = 0;
	}
}

static int relay_file_read_avail(struct rchan_buf *buf, size_t read_pos) {

	printf("%s: called with read_pos %zu\n", __func__, read_pos);

	size_t subbuf_size = buf->chan->subbuf_size;
	size_t n_subbufs = buf->chan->n_subbufs;
	size_t produced = buf->subbufs_produced;
	size_t consumed = buf->subbufs_consumed;

	relay_file_read_consume(buf, read_pos, 0);

	consumed = buf->subbufs_consumed;

	if (unlikely(buf->offset > subbuf_size)) {
		if (produced == consumed) {
			return 0;
		}
		return 1;
	}

	if (unlikely(produced - consumed >= n_subbufs)) {
		consumed = produced - n_subbufs + 1;
		buf->subbufs_consumed = consumed;
		buf->bytes_consumed = 0;
	}

	produced = (produced % n_subbufs) * subbuf_size + buf->offset;
	consumed = (consumed % n_subbufs) * subbuf_size + buf->bytes_consumed;

	if (consumed > produced)
		produced += n_subbufs * subbuf_size;

	if (consumed == produced) {
		if (buf->offset == subbuf_size && buf->subbufs_produced > buf->subbufs_consumed)
			return 1;
		return 0;
	}

	return 1;
}

static size_t relay_file_read_subbuf_avail(size_t read_pos,
										   struct rchan_buf *buf) {
	size_t padding, avail = 0;
	size_t read_subbuf, read_offset, write_subbuf, write_offset;
	size_t subbuf_size = buf->chan->subbuf_size;

	write_subbuf = ((char*)buf->data - (char*)buf->start) / subbuf_size;
	write_offset = buf->offset > subbuf_size ? subbuf_size : buf->offset;
	read_subbuf = read_pos / subbuf_size;
	read_offset = read_pos % subbuf_size;
	padding = buf->padding[read_subbuf];

	if (read_subbuf == write_subbuf) {
		if (read_offset + padding < write_offset)
			avail = write_offset - (read_offset + padding);
	} else
		avail = (subbuf_size - padding) - read_offset;

	return avail;
}

static size_t relay_file_read_start_pos(size_t read_pos,
										struct rchan_buf *buf) {
	size_t read_subbuf, padding, padding_start, padding_end;
	size_t subbuf_size = buf->chan->subbuf_size;
	size_t n_subbufs = buf->chan->n_subbufs;
	size_t consumed = buf->subbufs_consumed % n_subbufs;

	if (!read_pos)
		read_pos = consumed * subbuf_size + buf->bytes_consumed;
	read_subbuf = read_pos / subbuf_size;
	padding = buf->padding[read_subbuf];
	padding_start = (read_subbuf + 1) * subbuf_size - padding;
	padding_end = (read_subbuf + 1) * subbuf_size;
	if (read_pos >= padding_start && read_pos < padding_end) {
		read_subbuf = (read_subbuf + 1) % n_subbufs;
		read_pos = read_subbuf * subbuf_size;
	}

	return read_pos;
}

static size_t relay_file_read_end_pos(struct rchan_buf *buf,
									  size_t read_pos,
									  size_t count) {
	size_t read_subbuf, padding, end_pos;
	size_t subbuf_size = buf->chan->subbuf_size;
	size_t n_subbufs = buf->chan->n_subbufs;

	read_subbuf = read_pos / subbuf_size;
	padding = buf->padding[read_subbuf];
	if (read_pos % subbuf_size + count + padding == subbuf_size)
		end_pos = (read_subbuf + 1) * subbuf_size;
	else
		end_pos = read_pos + count;
	if (end_pos >= subbuf_size * n_subbufs)
		end_pos = 0;

	return end_pos;
}

static ssize_t relay_file_read(struct file *filp,
							   char __user *buffer,
							   size_t count,
							   loff_t *ppos) {

	printf("%s\n", __func__);

	struct seq_file *sf = filp->private_data;
	struct rchan_buf *buf = sf->private;
	
	size_t read_start, avail;
	size_t written = 0;
	int ret;

	if (!buf) {
		printf("%s: ERROR: buf = NULL\n", __func__);
		return 0;
	} else {
		/* printf("%s: rchan_buf = %p\n", __func__, buf); */
	}

	/* printf("%s: relay channel buffer content (start=%p):\n", __func__, buf->start); */
	/*		
	if (!buffer) {
		printf("%s: ERROR: buffer = NULL\n", __func__);
		return 0;
	} else {
		printf("%s: buffer = %p\n", __func__, buffer);
		}*/
	
	if (!count) {
		printf("%s: ERROR: count = 0\n", __func__);
		return 0;
	} else {
		/* printf("%s: count = %zu\n", __func__, count); */
	}

	
	// file already locked?
	
	/* VI_LOCK(file_inode(filp)); */
	do {
		/* printf("%s: top of do while\n", __func__); */

		void *from;

		if (!relay_file_read_avail(buf, *ppos)) {
			/* printf("%s: relay_file_read_avail() failed, breaking out\n", __func__); */
			break;
		}

		read_start = relay_file_read_start_pos(*ppos, buf);
		avail = relay_file_read_subbuf_avail(read_start, buf);
		if (!avail) {
			/* printf("%s: avail = 0, breaking out\n", __func__);		 */
			break;
		}
		avail = min(count, avail);
		from = (char*)buf->start + read_start;
		ret = avail;

		/* printf("%s: start=%lu, avail=%lu\n", __func__, read_start, avail); */
		/* printf("%s: relay channel buffer content (start=%p):\n", __func__, buf->start); */
		/* printf("%s: relay channel buffer content:\n", __func__); */
		/* char *b = (char*)from; */
		/* for(int i=0;i<avail;i++) { */
		/* 	printf("%c", b[i]); */
		/* } */
		/* printf("\n"); */

		seq_write(sf, from, avail);
		
		buffer += ret;
		written += ret;
		count -= ret;

		relay_file_read_consume(buf, read_start, ret);
		*ppos = relay_file_read_end_pos(buf, read_start, ret);
	} while (count);

	return written;
}



const struct file_operations relay_file_operations = {
	.open		= relay_file_open,
	.poll		= relay_file_poll,
	/* .mmap		= relay_file_mmap, // Not yet */
	.read		= relay_file_read,
	.llseek		= no_llseek,
	.release	= relay_file_release,
	/* .splice_read	= relay_file_splice_read, // Not yet */
};
