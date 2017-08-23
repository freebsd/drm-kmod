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

MALLOC_DEFINE(M_DFSINT, "debugfsint", "Linux debugfs internal");

static struct pfs_node *debugfs_root;

struct dentry_meta {
	struct dentry dm_dnode;
	const struct file_operations *dm_fops;
	void *dm_data;
	umode_t dm_mode;
};

static int
debugfs_attr(PFS_ATTR_ARGS)
{
	struct dentry_meta *dm;

	dm = pn->pn_data;

	vap->va_mode = dm->dm_mode;
	return (0);
}

static int
debugfs_destroy(PFS_DESTROY_ARGS)
{
	struct dentry_meta *d;

	d = pn->pn_data;
	free(d, M_DFSINT);
	return (0);
}

static int
debugfs_fill(PFS_FILL_ARGS)
{
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
debugfs_create_file(const char *name, umode_t mode,
		    struct dentry *parent, void *data,
		    const struct file_operations *fops)
{
	struct dentry_meta *dm;
	struct dentry *dnode;
	struct pfs_node *pnode;
	int flags;

	dm = malloc(sizeof(*dm), M_DFSINT, M_NOWAIT | M_ZERO);
	if (dm == NULL)
		return (NULL);
	dnode = &dm->dm_dnode;
	dm->dm_fops = fops;
	dm->dm_data = data;
	dm->dm_mode = mode;
	if (parent != NULL)
		pnode = parent->d_pfs_node;
	else
		pnode = debugfs_root;
	
	flags = fops->write ? PFS_RDWR : PFS_RD;
	dnode->d_pfs_node = pfs_create_file(pnode, name, debugfs_fill,
	    debugfs_attr, NULL, debugfs_destroy, flags);
	dnode->d_pfs_node->pn_data = dm;

	return (dnode);
}

struct dentry *
debugfs_create_dir(const char *name, struct dentry *parent)
{
	struct dentry_meta *dm;
	struct dentry *dnode;
	struct pfs_node *pnode;

	dm = malloc(sizeof(struct dentry), M_DFSINT, M_NOWAIT|M_ZERO);
	if (dm == NULL)
		return (NULL);
	dnode = &dm->dm_dnode;
	dm->dm_mode = 0700;
	if (parent != NULL)
		pnode = parent->d_pfs_node;
	else
		pnode = debugfs_root;

	dnode->d_pfs_node = pfs_create_dir(pnode, name, debugfs_attr, NULL, debugfs_destroy, PFS_RD);
	dnode->d_pfs_node->pn_data = dm;
	return (dnode);
}

void
debugfs_remove(struct dentry *dentry)
{
	;
}

void
debugfs_remove_recursive(struct dentry *dentry)
{
	;
}


static int
debugfs_init(PFS_INIT_ARGS)
{

	debugfs_root = pi->pi_root;
	return (0);
}

static int
debugfs_uninit(PFS_INIT_ARGS)
{
	return (0);
}

PSEUDOFS(debugfs, 1, PR_ALLOW_MOUNT_LINSYSFS);
MODULE_DEPEND(debugfs, linuxkpi, 1, 1, 1);
MODULE_DEPEND(debugfs, linuxkpi_gplv2, 1, 1, 1);
