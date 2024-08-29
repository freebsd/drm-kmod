/*-
 * Copyright (c) 2016 Matt Macy (mmacy@nextbsd.org)
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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>
#include <sys/proc.h>
#include <sys/sglist.h>
#include <sys/sleepqueue.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/bus.h>
#include <sys/fcntl.h>
#include <sys/file.h>
#include <sys/filio.h>
#include <sys/unistd.h>
#include <sys/capsicum.h>

#include <vm/vm.h>
#include <vm/pmap.h>

#include <machine/stdarg.h>

#include <linux/list.h>
#include <linux/dma-buf.h>
#include <linux/dma-resv.h>

#include <uapi/linux/dma-buf.h>

#undef file
#undef fget

#ifdef NOT_YET
#define	CONFIG_DMABUF_MOVE_NOTIFY
#endif

struct db_list {
	struct list_head head;
	struct sx lock;
};

static struct db_list db_list;
MALLOC_DEFINE(M_DMABUF, "dmabuf", "dmabuf allocator");

static fo_close_t dma_buf_close;
static fo_stat_t dma_buf_stat;
static fo_fill_kinfo_t dma_buf_fill_kinfo;
static fo_mmap_t dma_buf_mmap_fileops;
static fo_poll_t dma_buf_poll;
static fo_seek_t dma_buf_seek;
static fo_ioctl_t dma_buf_ioctl;

struct fileops dma_buf_fileops  = {
	.fo_close = dma_buf_close,
	.fo_stat = dma_buf_stat,
	.fo_fill_kinfo = dma_buf_fill_kinfo,
	.fo_mmap = dma_buf_mmap_fileops,
	.fo_poll = dma_buf_poll,
	.fo_seek = dma_buf_seek,
	.fo_ioctl = dma_buf_ioctl,
	.fo_flags = DFLAG_PASSABLE|DFLAG_SEEKABLE,
};

/* To upstream */
#ifndef DTYPE_DMABUF
#define	DTYPE_DMABUF	100
#endif

#define fp_is_db(fp) ((fp)->f_ops == &dma_buf_fileops)

static int
dma_buf_close(struct file *fp, struct thread *td)
{
	struct dma_buf *db;

	if (!fp_is_db(fp))
		return (EINVAL);

	db = fp->f_data;

	MPASS(db->cb_shared.active == 0);
	MPASS(db->cb_excl.active == 0);

	/* release DMA buffer */
	db->ops->release(db);

	sx_xlock(&db_list.lock);
	list_del(&db->list_node);
	sx_xunlock(&db_list.lock);

	if (db->resv == (struct dma_resv *)&db[1])
		dma_resv_fini(db->resv);

	free(db, M_DMABUF);
	return (0);
}

static int
dma_buf_stat(struct file *fp, struct stat *sb,
	     struct ucred *active_cred __unused)
{

	/* XXX need to define flags for st_mode */
	return (0);
}


static int
dma_buf_fill_kinfo(struct file *fp, struct kinfo_file *kif,
		   struct filedesc *fdp __unused)
{

	/* XXX pass this on to stat */
	return (0);
}

static int
dma_buf_mmap_fileops(struct file *fp, vm_map_t map, vm_offset_t *addr,
	     vm_size_t size, vm_prot_t prot, vm_prot_t cap_maxprot,
	     int flags, vm_ooffset_t foff, struct thread *td)
{
	struct dma_buf *db;
	struct vm_area_struct vma;

	if (!fp_is_db(fp))
		return (EINVAL);

	db = fp->f_data;
	if (foff + size > db->size)
		return (EINVAL);

	vma.vm_start = *addr;
	vma.vm_end = *addr + size;
	vma.vm_pgoff = foff;
	/* XXX do we need to fill in / propagate other flags? */
	
	return (-db->ops->mmap(db, &vma));
}

static int
dma_buf_seek(struct file *fp, off_t offset, int whence, struct thread *td)
{
	struct dma_buf *db;
	off_t base;
	
	if (!fp_is_db(fp))
		return (EINVAL);

	db = fp->f_data;

	if (offset != 0)
		return (EINVAL);
	
	if (whence == SEEK_END)
		base = db->size;
	else if (whence == SEEK_SET)
		base = 0;
	else
		return (EINVAL);

	td->td_retval[0] = fp->f_offset = base;
	return (0);
}

static int
dma_buf_poll(struct file *fp, int events,
	     struct ucred *active_cred, struct thread *td)
{
	// TODO: implement this when we need it (mostly used for cross-device fence sync)
	return (ENOTSUP);
}


static int
dma_buf_begin_cpu_access(struct dma_buf *db, enum dma_data_direction dir)
{

	MPASS(db != NULL);
	if (db->ops->begin_cpu_access)
		return (db->ops->begin_cpu_access(db, dir));
	return (0);
}

static int
dma_buf_end_cpu_access(struct dma_buf *db, enum dma_data_direction dir)
{
	MPASS(db != NULL);
	if (db->ops->end_cpu_access)
		return (db->ops->end_cpu_access(db, dir));
	return (0);
}

static int
dma_buf_ioctl(struct file *fp, u_long com, void *data,
	      struct ucred *active_cred, struct thread *td)
{
	struct dma_buf *db;
	struct dma_buf_sync *sync;
	enum dma_data_direction dir;
	int rc;

	if (!fp_is_db(fp))
		return (EINVAL);

	db = fp->f_data;
	sync = data;
	rc = 0;

	switch (com) {
	case DMA_BUF_IOCTL_SYNC:
		if (sync->flags & ~DMA_BUF_SYNC_VALID_FLAGS_MASK)
			return (EINVAL);

		switch (sync->flags & DMA_BUF_SYNC_RW) {
		case DMA_BUF_SYNC_READ:
			dir = DMA_FROM_DEVICE;
			break;
		case DMA_BUF_SYNC_WRITE:
			dir = DMA_TO_DEVICE;
			break;
		case DMA_BUF_SYNC_RW:
			dir = DMA_BIDIRECTIONAL;
			break;
		default:
			return (EINVAL);
		}
		if (sync->flags & DMA_BUF_SYNC_END)
			rc = dma_buf_end_cpu_access(db, dir);
		else
			rc = dma_buf_begin_cpu_access(db, dir);
		return (-rc);
	default:
		return (ENOTTY);
	}
	/* UNREACHED */
	return (0);
}


struct dma_buf_attachment *
dma_buf_dynamic_attach(struct dma_buf *db, struct device *dev,
    const struct dma_buf_attach_ops *iops, void *ipriv)
{
	struct dma_buf_attachment *dba;
	struct sg_table *sgt;
	int rc;

	MPASS(db != NULL);
	MPASS(dev != NULL);
	MPASS(iops == NULL || iops->move_notify != NULL);
	if (db == NULL || dev == NULL ||
	    (iops != NULL && iops->move_notify == NULL))
		return (ERR_PTR(-EINVAL));

	if ((dba = malloc(sizeof(*dba), M_DMABUF, M_NOWAIT|M_ZERO)) == NULL)
		return (ERR_PTR(-ENOMEM));
	
	dba->dev = dev;
	dba->dmabuf = db;
	if (iops != NULL)
		dba->peer2peer = iops->allow_peer2peer;
	dba->importer_ops = iops;
	dba->importer_priv = ipriv;

	if (db->ops->attach) {
		if ((rc = db->ops->attach(db, dba)) != 0) {
			free(dba, M_DMABUF);
			return (ERR_PTR(rc));
		}
	}
	dma_resv_lock(db->resv, NULL);
	list_add(&dba->node, &db->attachments);
	dma_resv_unlock(db->resv);

	if ((iops != NULL) != (db->ops->pin != NULL)) {
		if (dba->dmabuf->ops->pin != NULL) {
			dma_resv_lock(dba->dmabuf->resv, NULL);
			rc = dma_buf_pin(dba);
			if (rc != 0) {
				dma_resv_unlock(dba->dmabuf->resv);
				return (ERR_PTR(rc));
			}
		}
		sgt = db->ops->map_dma_buf(dba, DMA_BIDIRECTIONAL);
		if (sgt == NULL)
			sgt = ERR_PTR(-ENOMEM);
		if (IS_ERR(sgt) && dba->dmabuf->ops->pin != NULL)
			dma_buf_unpin(dba);
		if (dba->dmabuf->ops->pin != NULL)
			dma_resv_unlock(dba->dmabuf->resv);
		if (IS_ERR(sgt)) {
			dma_buf_detach(db, dba);
			return (ERR_CAST(sgt));
		}
		dba->sgt = sgt;
		dba->dir = DMA_BIDIRECTIONAL;
	}

	return (dba);
}

struct dma_buf_attachment *
dma_buf_attach(struct dma_buf *db, struct device *dev)
{
	return (dma_buf_dynamic_attach(db, dev, NULL, NULL));
}

void
dma_buf_detach(struct dma_buf *db, struct dma_buf_attachment *dba)
{
	MPASS(db != NULL);
	MPASS(dba != NULL);
	
	if (db == NULL || dba == NULL)
		return;

	if (dba->sgt != NULL) {
		if (dba->dmabuf->ops->pin != NULL)
			dma_resv_lock(dba->dmabuf->resv, NULL);
		db->ops->unmap_dma_buf(dba, dba->sgt, dba->dir);
		if (dba->dmabuf->ops->pin != NULL) {
			dma_buf_unpin(dba);
			dma_resv_unlock(dba->dmabuf->resv);
		}
	}

	dma_resv_lock(db->resv, NULL);
	list_del(&dba->node);
	dma_resv_unlock(db->resv);
	if (db->ops->detach)
		db->ops->detach(db, dba);
	free(dba, M_DMABUF);
}

int
dma_buf_pin(struct dma_buf_attachment *dba)
{
	dma_resv_assert_held(dba->dmabuf->resv);
	return (dba->dmabuf->ops->pin != NULL ? dba->dmabuf->ops->pin(dba) :0);
}

void
dma_buf_unpin(struct dma_buf_attachment *dba)
{
	dma_resv_assert_held(dba->dmabuf->resv);
	if (dba->dmabuf->ops->unpin != NULL)
		dba->dmabuf->ops->unpin(dba);
}

struct dma_buf *
dma_buf_get(int fd)
{
	struct file *fp;
	int rc;
	cap_rights_t rights;

	CAP_ALL(&rights);
	if ((rc = fget(curthread, fd, &rights, &fp)) != 0)
		return (ERR_PTR(-rc));

	if (!fp_is_db(fp)) {
		fdrop(fp, curthread);
		return (ERR_PTR(-EINVAL));
	}
	return (fp->f_data);
}

void
dma_buf_put(struct dma_buf *db)
{

	MPASS(db != NULL);
	MPASS(db->linux_file != NULL);

	fdrop(db->linux_file, curthread);
}

int
dma_buf_fd(struct dma_buf *db, int flags)
{
	int err, fd;

	if (db == NULL || db->linux_file == NULL)
		return (-EINVAL);

	/* XXX ignore flags for now */
	if ((err = finstall(curthread, db->linux_file, &fd, 0, NULL)) != 0)
		return (err);

	/* drop extra reference */
	fdrop(db->linux_file, curthread);

	return (fd);
}

struct dma_buf *
dma_buf_export(const struct dma_buf_export_info *exp_info)
{
	const struct dma_buf_ops *ops = exp_info->ops;
	struct dma_buf *db;
	struct file *fp;
	struct dma_resv *ro;
	int size, err;

	MPASS(!(ops->cache_sgt_mapping &&
		(ops->pin != NULL || ops->unpin != NULL)));
	MPASS((ops->pin == NULL) == (ops->unpin == NULL));

	ro = exp_info->resv;
	size = (ro == NULL) ? sizeof(*db) + sizeof(*ro) : sizeof(*db) + 1;
	db = malloc(size, M_DMABUF, M_NOWAIT|M_ZERO);
	err = 0;
	if (db == NULL)
		return (ERR_PTR(-ENOENT));

	db->priv = exp_info->priv;
	db->ops = ops;
	db->size = exp_info->size;
	db->exp_name = exp_info->exp_name;
	db->owner = exp_info->owner;
	init_waitqueue_head(&db->poll);
	db->cb_excl.poll = db->cb_shared.poll = &db->poll;
	db->cb_excl.active = db->cb_shared.active = 0;

	if (ro == NULL) {
		ro = (struct dma_resv *)&db[1];
		dma_resv_init(ro);
	}
	db->resv = ro;
	if ((err = falloc_noinstall(curthread, &fp)) != 0)
		goto err;

	finit(fp, 0, DTYPE_DMABUF, db, &dma_buf_fileops);

	db->linux_file = fp;
	mutex_init(&db->lock);
	INIT_LIST_HEAD(&db->attachments);

	sx_xlock(&db_list.lock);
	list_add(&db->list_node, &db_list.head);
	sx_xunlock(&db_list.lock);

	return (db);
err:	
	free(db, M_DMABUF);
	return (ERR_PTR(-err));
}

struct sg_table *
dma_buf_map_attachment(struct dma_buf_attachment *dba, enum dma_data_direction dir)
{
	struct sg_table *sgt;
	int rc;

	MPASS(dba != NULL);
	MPASS(dba->dmabuf != NULL);
		
	if (dba == NULL || dba->dmabuf == NULL)
		return (ERR_PTR(-EINVAL));

	if (dba->importer_ops != NULL)
		dma_resv_assert_held(dba->dmabuf->resv);

	if (dba->sgt != NULL) {
		if (dba->dir != dir && dba->dir != DMA_BIDIRECTIONAL)
			return (ERR_PTR(-EBUSY));
		return (dba->sgt);
	}

	if (dba->dmabuf->ops->pin != NULL) {
		dma_resv_assert_held(dba->dmabuf->resv);
#ifndef CONFIG_DMABUF_MOVE_NOTIFY
		rc = dma_buf_pin(dba);
		if (rc != 0)
			return ERR_PTR(rc);
#endif
	}

	sgt = dba->dmabuf->ops->map_dma_buf(dba, dir);
	if (sgt == NULL)
		return (ERR_PTR(-ENOMEM));

#ifndef CONFIG_DMABUF_MOVE_NOTIFY
	if (IS_ERR(sgt) && dba->dmabuf->ops->pin != NULL)
		dma_buf_unpin(dba);
#endif

	if (!IS_ERR(sgt) && dba->dmabuf->ops->cache_sgt_mapping) {
		dba->sgt = sgt;
		dba->dir = dir;
	}

	return (sgt);
}

struct sg_table *
dma_buf_map_attachment_unlocked(struct dma_buf_attachment *dba,
    enum dma_data_direction dir)
{
	struct sg_table *sgt;

	if (dba == NULL || dba->dmabuf == NULL)
		return ERR_PTR(-EINVAL);

	dma_resv_lock(dba->dmabuf->resv, NULL);
	sgt = dma_buf_map_attachment(dba, dir);
	dma_resv_unlock(dba->dmabuf->resv);

	return (sgt);
}

void
dma_buf_unmap_attachment(struct dma_buf_attachment *dba,
			 struct sg_table *sg_table,
			 enum dma_data_direction dir)
{
	MPASS(dba != NULL);
	MPASS(dba->dmabuf != NULL);
	MPASS(sg_table != NULL);

	if (dba == NULL || dba->dmabuf == NULL || sg_table == NULL)
		return;

	if (dba->importer_ops != NULL)
		dma_resv_assert_held(dba->dmabuf->resv);
	if (dba->sgt == sg_table)
		return;
	if (dba->dmabuf->ops->pin != NULL)
		dma_resv_assert_held(dba->dmabuf->resv);

	dba->dmabuf->ops->unmap_dma_buf(dba, sg_table, dir);

#ifndef CONFIG_DMABUF_MOVE_NOTIFY
	if (dba->dmabuf->ops->pin != NULL)
		dma_buf_unpin(dba);
#endif
}

void
dma_buf_unmap_attachment_unlocked(struct dma_buf_attachment *dba,
    struct sg_table *sgt, enum dma_data_direction dir)
{
	if (dba == NULL || dba->dmabuf == NULL || sgt == NULL)
		return;

	dma_resv_lock(dba->dmabuf->resv, NULL);
	dma_buf_unmap_attachment(dba, sgt, dir);
	dma_resv_unlock(dba->dmabuf->resv);
}

void
dma_buf_move_notify(struct dma_buf *db)
{
	struct dma_buf_attachment *dba;

	dma_resv_assert_held(db->resv);

	list_for_each_entry(dba, &db->attachments, node)
		if (dba->importer_ops != NULL &&
		    dba->importer_ops->move_notify != NULL)
			dba->importer_ops->move_notify(dba);
}

int
dma_buf_vmap(struct dma_buf *dmabuf, struct iosys_map *map)
{
	struct iosys_map ptr;
	int ret = 0;

	iosys_map_clear(map);

	if (WARN_ON(!dmabuf))
		return -EINVAL;

	if (!dmabuf->ops->vmap)
		return -EINVAL;

	mutex_lock(&dmabuf->lock);
	if (dmabuf->vmapping_counter) {
		dmabuf->vmapping_counter++;
		BUG_ON(iosys_map_is_null(&dmabuf->vmap_ptr));
		*map = dmabuf->vmap_ptr;
		goto out_unlock;
	}

	BUG_ON(iosys_map_is_set(&dmabuf->vmap_ptr));

	ret = dmabuf->ops->vmap(dmabuf, &ptr);
	if (WARN_ON_ONCE(ret))
		goto out_unlock;

	dmabuf->vmap_ptr = ptr;
	dmabuf->vmapping_counter = 1;

	*map = dmabuf->vmap_ptr;

out_unlock:
	mutex_unlock(&dmabuf->lock);
	return ret;
}

void dma_buf_vunmap(struct dma_buf *dmabuf, struct iosys_map *map)
{
	if (WARN_ON(!dmabuf))
		return;

	BUG_ON(iosys_map_is_null(&dmabuf->vmap_ptr));
	BUG_ON(dmabuf->vmapping_counter == 0);
	BUG_ON(!iosys_map_is_equal(&dmabuf->vmap_ptr, map));

	mutex_lock(&dmabuf->lock);
	if (--dmabuf->vmapping_counter == 0) {
		if (dmabuf->ops->vunmap)
			dmabuf->ops->vunmap(dmabuf, map);
		iosys_map_clear(&dmabuf->vmap_ptr);
	}
	mutex_unlock(&dmabuf->lock);
}

static void
dma_buf_init(void *arg __unused)
{
	sx_init(&db_list.lock, "db_list_lock");
	INIT_LIST_HEAD(&db_list.head);
}

static void
dma_buf_uninit(void *arg __unused)
{
	sx_destroy(&db_list.lock);
}

SYSINIT(dma_buf, SI_SUB_DRIVERS, SI_ORDER_SECOND, dma_buf_init, NULL);
SYSUNINIT(dma_buf, SI_SUB_DRIVERS, SI_ORDER_SECOND, dma_buf_uninit, NULL);
