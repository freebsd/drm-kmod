
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/queue.h>
#include <sys/blist.h>
#include <sys/conf.h>
#include <sys/exec.h>
#include <sys/filedesc.h>
#include <sys/kernel.h>
#include <sys/linker.h>
#include <sys/malloc.h>
#include <sys/mount.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/resourcevar.h>
#include <sys/sbuf.h>
#include <sys/smp.h>
#include <sys/socket.h>
#include <sys/vnode.h>
#include <sys/bus.h>
#include <sys/pciio.h>

#include <dev/pci/pcivar.h>
#include <dev/pci/pcireg.h>

#include <net/if.h>

#include <vm/vm.h>
#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_param.h>
#include <vm/vm_object.h>
#include <vm/swap_pager.h>

#include <machine/bus.h>

#include <compat/linux/linux_ioctl.h>
#include <compat/linux/linux_mib.h>
#include <compat/linux/linux_util.h>
#include <fs/pseudofs/pseudofs.h>

#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/compat.h>

#include <linux/anon_inodefs.h>

MALLOC_DEFINE(M_AFSINT, "anon_inodefsint", "Linux anon_inode internal");

static struct pfs_node *anon_inodefs_root;

struct dentry_meta {
	struct dentry dm_dnode;
	const struct file_operations *dm_fops;
	void *dm_data;
	umode_t dm_mode;
};

static int
anon_inodefs_attr(PFS_ATTR_ARGS)
{
	printf("%s: \n",__func__);
	struct dentry_meta *dm;

	dm = pn->pn_data;

	vap->va_mode = dm->dm_mode;
	return (0);
}

static int
anon_inodefs_destroy(PFS_DESTROY_ARGS)
{
	printf("%s: \n",__func__);
	struct dentry_meta *d;

	d = pn->pn_data;
	free(d, M_AFSINT);
	return (0);
}

static int
anon_inodefs_fill(PFS_FILL_ARGS)
{
	printf("%s: \n",__func__);
	return 0;

	struct dentry_meta *d;
	struct linux_file lf;
	struct seq_file *sf;
	struct vnode vn;
	void *buf;
	int rc;
	size_t len;
	off_t off;

	d = pn->pn_data;

	if ((rc = linux_set_current_flags(curthread, M_NOWAIT)))
		return (rc);
	vn.v_data = d->dm_data;
	buf = uio->uio_iov[0].iov_base;
	len = min(uio->uio_iov[0].iov_len, uio->uio_resid);
	off = 0;
	lf.private_data = NULL;
	rc = d->dm_fops->open(&vn, &lf);
	if (rc < 0) {
#ifdef INVARIANTS
		printf("open failed with %d\n", rc);
#endif
		return (-rc);
	}
	sf = lf.private_data;
	sf->buf = sb;
	if (uio->uio_rw == UIO_READ)
		rc = d->dm_fops->read(&lf, NULL, len, &off);
	else
		rc = d->dm_fops->write(&lf, buf, len, &off);
	if (d->dm_fops->release)
		d->dm_fops->release(&vn, &lf);
	else
		single_release(&vn, &lf);
	
	if (rc < 0) {
#ifdef INVARIANTS
		printf("read/write return %d\n", rc);
#endif
		return (-rc);
	}
	return (0);
}

struct dentry *
anon_inodefs_create_file(const char *name, umode_t mode,
						 struct dentry *parent, void *data,
						 const struct file_operations *fops)
{
	printf("%s: name = %s\n",__func__, name);
	
	struct pfs_node *p = pfs_find_node(anon_inodefs_root, name);
	if (p) {
		printf("%s: ERROR: node with name = %s already exists\n",__func__, name);
		return NULL;
	}
	
	struct dentry_meta *dm;
	struct dentry *dnode;
	struct pfs_node *pnode;
	int flags;

	dm = malloc(sizeof(*dm), M_AFSINT, M_NOWAIT | M_ZERO);
	if (dm == NULL)
		return (NULL);
	dnode = &dm->dm_dnode;
	dm->dm_fops = fops;
	dm->dm_data = data;
	dm->dm_mode = mode;
	if (parent != NULL)
		pnode = parent->d_pfs_node;
	else
		pnode = anon_inodefs_root;
	
	flags = 0; //fops->write ? PFS_RDWR : PFS_RD;
	dnode->d_pfs_node = pfs_create_file(pnode, name, anon_inodefs_fill,
	    anon_inodefs_attr, NULL, anon_inodefs_destroy, flags);
	dnode->d_pfs_node->pn_data = dm;

	return (dnode);
}
/*
struct dentry *
anon_inodefs_create_dir(const char *name, struct dentry *parent)
{
	printf("%s: \n",__func__);
	struct dentry_meta *dm;
	struct dentry *dnode;
	struct pfs_node *pnode;

	dm = malloc(sizeof(*dm), M_AFSINT, M_NOWAIT | M_ZERO);
	if (dm == NULL)
		return (NULL);
	dnode = &dm->dm_dnode;
	dm->dm_mode = 0700;
	if (parent != NULL)
		pnode = parent->d_pfs_node;
	else
		pnode = anon_inodefs_root;

	dnode->d_pfs_node = pfs_create_dir(pnode, name, anon_inodefs_attr, NULL, anon_inodefs_destroy, PFS_RD);
	dnode->d_pfs_node->pn_data = dm;
	return (dnode);
}
*/
void
anon_inodefs_remove(struct dentry *dentry)
{
	printf("%s: \n",__func__);

	struct pfs_node *parent = dentry->d_pfs_node->pn_parent;
	if (parent)
		mtx_lock(&parent->pn_mutex);
	pfs_destroy(dentry->d_pfs_node);
	if (parent)
		mtx_unlock(&parent->pn_mutex);
}
/*
void
anon_inodefs_remove_recursive(struct dentry *dentry)
{
	printf("%s: \n",__func__);
	pfs_destroy(dentry->d_pfs_node);
}
*/

static int
anon_inodefs_init(PFS_INIT_ARGS)
{
	printf("%s: \n",__func__);

	anon_inodefs_root = pi->pi_root;
	return (0);
}

static int
anon_inodefs_uninit(PFS_INIT_ARGS)
{
	printf("%s: \n",__func__);
	return (0);
}

PSEUDOFS(anon_inodefs, 1, VFCF_JAIL);
