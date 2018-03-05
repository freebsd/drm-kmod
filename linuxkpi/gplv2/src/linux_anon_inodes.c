// FreeBSD
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/types.h>
#include <sys/namei.h>
#include <sys/vnode.h>
#include <sys/libkern.h>

// Linux
#include <linux/anon_inodes.h>
#include <linux/anon_inodefs.h>
#include <linux/file.h>
#include <linux/fs.h>


struct file *
anon_inode_getfile(const char *name,
    const struct file_operations *fops,
    void *priv, int flags) {

	/* LINUX:

	struct qstr this;
	struct path path;
	struct file *file;

	if (IS_ERR(anon_inode_inode))
		return ERR_PTR(-ENODEV);

	if (fops->owner && !try_module_get(fops->owner))
		return ERR_PTR(-ENOENT);

	 * Link the inode to a directory entry by creating a unique name
	 * using the inode sequence number.

	file = ERR_PTR(-ENOMEM);
	this.name = name;
	this.len = strlen(name);
	this.hash = 0;
	path.dentry = d_alloc_pseudo(anon_inode_mnt->mnt_sb, &this);
	if (!path.dentry)
		goto err_module;

	path.mnt = mntget(anon_inode_mnt);

	 * We know the anon_inode inode count is always greater than zero,
	 * so ihold() is safe.

	ihold(anon_inode_inode);

	d_instantiate(path.dentry, anon_inode_inode);

	file = alloc_file(&path, OPEN_FMODE(flags), fops);
	if (IS_ERR(file))
		goto err_dput;
	file->f_mapping = anon_inode_inode->i_mapping;

	file->f_flags = flags & (O_ACCMODE | O_NONBLOCK);
	file->private_data = priv;

	return file;

err_dput:
	path_put(&path);
err_module:
	module_put(fops->owner);
	return file;
	 */
		
	printf("%s: creating anon_inode name = %s\n", __func__, name);

	struct linux_file *file;
	umode_t mode = S_IFREG;// | S_IRUGO;

	/* Create file descriptor */
	file = alloc_file(FMODE_READ, fops);
	if (IS_ERR(file)) {
		printf("%s: ERROR: alloc_file\n", __func__);
		return NULL;
	}

	/* 
	   Could still fail but good enough for now...
	 */
	uint32_t rnd = arc4random();
	char *unique_name = malloc(strlen(name) + 10, M_KMALLOC, M_WAITOK);
	snprintf(unique_name, strlen(name) + 10, "%s-%08x", name, rnd);
	
	printf("%s: created unique name = %s\n", __func__, unique_name);

	struct dentry *d = anon_inodefs_create_file(unique_name, mode,
	    NULL, NULL, NULL);
	free(unique_name, M_KMALLOC);	
	if (!d) {
		printf("%s: ERROR: anon_inodefs_create_file\n", __func__);
		return NULL;
	}

	file->f_flags = 0;//flags & (O_ACCMODE | O_NONBLOCK);
	file->private_data = priv;
	file->f_dentry = d;
		
	return file;
}

/*
// Only used by i915_perf.c so far (which is disabled)..
int
anon_inode_getfd(const char *name, const struct file_operations *fops,
				 void *priv, int flags) {
	printf("%s: name = %s\n", __func__, name);
	int error, fd;
	struct linux_file *file;

	error = get_unused_fd_flags(flags);
	if (error < 0)
		return error;
	fd = error;

	file = anon_inode_getfile(name, fops, priv, flags);
	if (IS_ERR(file)) {
		error = PTR_ERR(file);
		goto err_put_unused_fd;
	}
	fd_install(fd, file);

	return fd;

err_put_unused_fd:
	put_unused_fd(fd);
	return error;
}
*/
