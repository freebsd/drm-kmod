/*
 * Header file for dma buffer sharing framework.
 *
 * Copyright(C) 2011 Linaro Limited. All rights reserved.
 * Author: Sumit Semwal <sumit.semwal@ti.com>
 *
 * Many thanks to linaro-mm-sig list, and specially
 * Arnd Bergmann <arnd@arndb.de>, Rob Clark <rob@ti.com> and
 * Daniel Vetter <daniel@ffwll.ch> for their support in creation and
 * refining of this idea.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LINUX_GPLV2_DMA_BUF_H_
#define _LINUX_GPLV2_DMA_BUF_H_

#include <linux/iosys-map.h>
#include <linux/file.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <linux/list.h>
#include <linux/dma-mapping.h>
#include <linux/fs.h>
#include <linux/dma-fence.h>
#include <linux/wait.h>
#include <linux/module.h>

struct device;
struct dma_buf;
struct dma_buf_attachment;
struct dma_buf_export_info;

int dma_buf_fd(struct dma_buf *dmabuf, int flags);
struct dma_buf *dma_buf_get(int fd);
void dma_buf_put(struct dma_buf *db);

struct dma_buf *dma_buf_export(const struct dma_buf_export_info *exp_info);

struct dma_buf_export_info {
	const char *exp_name;
	struct module *owner;
	const struct dma_buf_ops *ops;
	size_t size;
	int flags;
	struct dma_resv *resv;
	void *priv;
};

#define DEFINE_DMA_BUF_EXPORT_INFO(a)	\
	struct dma_buf_export_info a = { .exp_name = KBUILD_MODNAME, \
					 .owner = THIS_MODULE }

struct dma_buf_ops {
	bool cache_sgt_mapping;

	int (*attach)(struct dma_buf *, struct dma_buf_attachment *);

	void (*detach)(struct dma_buf *, struct dma_buf_attachment *);

	int (*pin)(struct dma_buf_attachment *attach);

	void (*unpin)(struct dma_buf_attachment *attach);

	/* For {map,unmap}_dma_buf below, any specific buffer attributes
	 * required should get added to device_dma_parameters accessible
	 * via dev->dma_params.
	 */
	struct sg_table * (*map_dma_buf)(struct dma_buf_attachment *,
						enum dma_data_direction);
	void (*unmap_dma_buf)(struct dma_buf_attachment *,
						struct sg_table *,
						enum dma_data_direction);
	/* TODO: Add try_map_dma_buf version, to return immed with -EBUSY
	 * if the call would block.
	 */

	/* after final dma_buf_put() */
	void (*release)(struct dma_buf *);

	int (*begin_cpu_access)(struct dma_buf *, enum dma_data_direction);
	int (*end_cpu_access)(struct dma_buf *, enum dma_data_direction);

	void *(*map_atomic)(struct dma_buf *, unsigned long);
	void (*unmap_atomic)(struct dma_buf *, unsigned long, void *);

	void *(*map)(struct dma_buf *, unsigned long);
	void (*unmap)(struct dma_buf *, unsigned long, void *);
	
	int (*mmap)(struct dma_buf *, struct vm_area_struct *vma);

	int (*vmap)(struct dma_buf *dmabuf, struct iosys_map *map);
	void (*vunmap)(struct dma_buf *dmabuf, struct iosys_map *map);
};

#undef file
struct dma_buf {
	size_t size;
	struct file *linux_file; /* Native struct file, not struct linux_file */
	struct list_head attachments;
	const struct dma_buf_ops *ops;
	/* mutex to serialize list manipulation, attach/detach and vmap/unmap */
	struct mutex lock;
	unsigned vmapping_counter;
	struct iosys_map vmap_ptr;
	const char *exp_name;
	struct module *owner;
	struct list_head list_node;
	void *priv;
	struct dma_resv *resv;

	/* poll support */
	wait_queue_head_t poll;

	struct dma_buf_poll_cb_t {
		struct dma_fence_cb cb;
		wait_queue_head_t *poll;

		unsigned long active;
	} cb_excl, cb_shared;
};

struct dma_buf_attachment {
	struct dma_buf *dmabuf;
	struct device *dev;
	struct list_head node;
	struct sg_table *sgt;
	enum dma_data_direction dir;
	bool peer2peer;
	const struct dma_buf_attach_ops *importer_ops;
	void *importer_priv;
	void *priv;
};

struct dma_buf_attach_ops {
	bool allow_peer2peer;
	void (*move_notify)(struct dma_buf_attachment *attach);
};

#define file linux_file
static inline void
get_dma_buf(struct dma_buf *dmabuf)
{
	/*
	 * As of r350199 fhold changed from macro to a function returning
	 * true/false at success/failure to avoid overflow.
	 */
#ifdef fhold
	fhold(dmabuf->file);
#else
	while(!fhold(dmabuf->file)) {
		pause("fhold", hz);
	}
#endif
}


struct dma_buf_attachment *dma_buf_attach(struct dma_buf *, struct device *);
struct dma_buf_attachment *dma_buf_dynamic_attach(struct dma_buf *,
    struct device *, const struct dma_buf_attach_ops *, void *);
void dma_buf_detach(struct dma_buf *, struct dma_buf_attachment *);
int dma_buf_pin(struct dma_buf_attachment *);
void dma_buf_unpin(struct dma_buf_attachment *);

struct sg_table *dma_buf_map_attachment(struct dma_buf_attachment *,
					enum dma_data_direction);
struct sg_table *dma_buf_map_attachment_unlocked(struct dma_buf_attachment *,
    enum dma_data_direction);
void dma_buf_unmap_attachment(struct dma_buf_attachment *, struct sg_table *,
				enum dma_data_direction);
void dma_buf_unmap_attachment_unlocked(struct dma_buf_attachment *,
    struct sg_table *, enum dma_data_direction);
void dma_buf_move_notify(struct dma_buf *);
int dma_buf_vmap(struct dma_buf *dmabuf, struct iosys_map *map);
void dma_buf_vunmap(struct dma_buf *dmabuf, struct iosys_map *map);

#endif /* _LINUX_GPLV2_DMA_BUF_H_ */
