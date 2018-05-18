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
anon_inode_getfile(const char *name, const struct file_operations *fops,
    void *priv, int flags)
{
	/*
	 * A new sync_file is created for each frame.
	 * TODO: Find a more optimized way of doing this??
	 */

	struct linux_file *file;
	umode_t mode = S_IFREG;
	int retries = 0;

	/* Create file descriptor */
	file = alloc_file(FMODE_READ, fops);
	if (IS_ERR(file)) {
		printf("%s: ERROR: alloc_file\n", __func__);
		return NULL;
	}

	/* Good enough for now? */
retry: ;
	uint32_t rnd = arc4random();
	char *unique_name = malloc(strlen(name) + 10, M_KMALLOC, M_WAITOK);
	snprintf(unique_name, strlen(name) + 10, "%s-%08x", name, rnd);

	struct dentry *d = anon_inodefs_create_file(unique_name, mode,
	    NULL, NULL, NULL);
	free(unique_name, M_KMALLOC);
	if (!d) {
		printf("%s: ERROR: Could not create anon_inodefs file\n",
		    __func__);
		if (retries > 10) {
			return NULL;
		}
		retries++;
		goto retry;
	}

	file->f_flags = 0;
	file->private_data = priv;
	file->f_dentry = d;

	return file;
}

void
anon_inode_release(struct file *file)
{
	anon_inodefs_remove(file->f_dentry);
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
