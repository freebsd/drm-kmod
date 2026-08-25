/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 */

extern struct vnodeop_desc vop_default_desc;
#include "vnode_if_typedef.h"
#include "vnode_if_newproto.h"
struct vop_islocked_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_islocked_desc;

int VOP_ISLOCKED_AP(struct vop_islocked_args *);
int VOP_ISLOCKED_APV(const struct vop_vector *vop, struct vop_islocked_args *);

static __inline int VOP_ISLOCKED(
	struct vnode *vp)
{
	struct vop_islocked_args a;

	a.a_gen.a_desc = &vop_islocked_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_islocked(&a));
	else
		return (VOP_ISLOCKED_APV(vp->v_op, &a));
#else
	return (VOP_ISLOCKED_APV(vp->v_op, &a));
#endif
}

struct vop_lookup_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
};

extern struct vnodeop_desc vop_lookup_desc;

int VOP_LOOKUP_AP(struct vop_lookup_args *);
int VOP_LOOKUP_APV(const struct vop_vector *vop, struct vop_lookup_args *);

static __inline int VOP_LOOKUP(
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp)
{
	struct vop_lookup_args a;

	a.a_gen.a_desc = &vop_lookup_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (dvp->v_op->vop_lookup(&a));
	else
		return (VOP_LOOKUP_APV(dvp->v_op, &a));
#else
	return (VOP_LOOKUP_APV(dvp->v_op, &a));
#endif
}

struct vop_cachedlookup_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
};

extern struct vnodeop_desc vop_cachedlookup_desc;

int VOP_CACHEDLOOKUP_AP(struct vop_cachedlookup_args *);
int VOP_CACHEDLOOKUP_APV(const struct vop_vector *vop, struct vop_cachedlookup_args *);

static __inline int VOP_CACHEDLOOKUP(
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp)
{
	struct vop_cachedlookup_args a;

	a.a_gen.a_desc = &vop_cachedlookup_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (dvp->v_op->vop_cachedlookup(&a));
	else
		return (VOP_CACHEDLOOKUP_APV(dvp->v_op, &a));
#else
	return (VOP_CACHEDLOOKUP_APV(dvp->v_op, &a));
#endif
}

struct vop_create_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};

extern struct vnodeop_desc vop_create_desc;

int VOP_CREATE_AP(struct vop_create_args *);
int VOP_CREATE_APV(const struct vop_vector *vop, struct vop_create_args *);

static __inline int VOP_CREATE(
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap)
{
	struct vop_create_args a;

	a.a_gen.a_desc = &vop_create_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	return (VOP_CREATE_APV(dvp->v_op, &a));
}

struct vop_whiteout_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct componentname *a_cnp;
	int a_flags;
};

extern struct vnodeop_desc vop_whiteout_desc;

int VOP_WHITEOUT_AP(struct vop_whiteout_args *);
int VOP_WHITEOUT_APV(const struct vop_vector *vop, struct vop_whiteout_args *);

static __inline int VOP_WHITEOUT(
	struct vnode *dvp,
	struct componentname *cnp,
	int flags)
{
	struct vop_whiteout_args a;

	a.a_gen.a_desc = &vop_whiteout_desc;
	a.a_dvp = dvp;
	a.a_cnp = cnp;
	a.a_flags = flags;
	return (VOP_WHITEOUT_APV(dvp->v_op, &a));
}

struct vop_mknod_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};

extern struct vnodeop_desc vop_mknod_desc;

int VOP_MKNOD_AP(struct vop_mknod_args *);
int VOP_MKNOD_APV(const struct vop_vector *vop, struct vop_mknod_args *);

static __inline int VOP_MKNOD(
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap)
{
	struct vop_mknod_args a;

	a.a_gen.a_desc = &vop_mknod_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	return (VOP_MKNOD_APV(dvp->v_op, &a));
}

struct vop_open_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_mode;
	struct ucred *a_cred;
	struct thread *a_td;
	struct file *a_fp;
};

extern struct vnodeop_desc vop_open_desc;

int VOP_OPEN_AP(struct vop_open_args *);
int VOP_OPEN_APV(const struct vop_vector *vop, struct vop_open_args *);

static __inline int VOP_OPEN(
	struct vnode *vp,
	int mode,
	struct ucred *cred,
	struct thread *td,
	struct file *fp)
{
	struct vop_open_args a;

	a.a_gen.a_desc = &vop_open_desc;
	a.a_vp = vp;
	a.a_mode = mode;
	a.a_cred = cred;
	a.a_td = td;
	a.a_fp = fp;
	return (VOP_OPEN_APV(vp->v_op, &a));
}

struct vop_close_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_fflag;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_close_desc;

int VOP_CLOSE_AP(struct vop_close_args *);
int VOP_CLOSE_APV(const struct vop_vector *vop, struct vop_close_args *);

static __inline int VOP_CLOSE(
	struct vnode *vp,
	int fflag,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_close_args a;

	a.a_gen.a_desc = &vop_close_desc;
	a.a_vp = vp;
	a.a_fflag = fflag;
	a.a_cred = cred;
	a.a_td = td;
	return (VOP_CLOSE_APV(vp->v_op, &a));
}

struct vop_fplookup_vexec_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_fplookup_vexec_desc;

int VOP_FPLOOKUP_VEXEC_AP(struct vop_fplookup_vexec_args *);
int VOP_FPLOOKUP_VEXEC_APV(const struct vop_vector *vop, struct vop_fplookup_vexec_args *);

static __inline int VOP_FPLOOKUP_VEXEC(
	struct vnode *vp,
	struct ucred *cred)
{
	struct vop_fplookup_vexec_args a;

	a.a_gen.a_desc = &vop_fplookup_vexec_desc;
	a.a_vp = vp;
	a.a_cred = cred;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_fplookup_vexec(&a));
	else
		return (VOP_FPLOOKUP_VEXEC_APV(vp->v_op, &a));
#else
	return (VOP_FPLOOKUP_VEXEC_APV(vp->v_op, &a));
#endif
}

struct vop_fplookup_symlink_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct cache_fpl *a_fpl;
};

extern struct vnodeop_desc vop_fplookup_symlink_desc;

int VOP_FPLOOKUP_SYMLINK_AP(struct vop_fplookup_symlink_args *);
int VOP_FPLOOKUP_SYMLINK_APV(const struct vop_vector *vop, struct vop_fplookup_symlink_args *);

static __inline int VOP_FPLOOKUP_SYMLINK(
	struct vnode *vp,
	struct cache_fpl *fpl)
{
	struct vop_fplookup_symlink_args a;

	a.a_gen.a_desc = &vop_fplookup_symlink_desc;
	a.a_vp = vp;
	a.a_fpl = fpl;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_fplookup_symlink(&a));
	else
		return (VOP_FPLOOKUP_SYMLINK_APV(vp->v_op, &a));
#else
	return (VOP_FPLOOKUP_SYMLINK_APV(vp->v_op, &a));
#endif
}

struct vop_access_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	accmode_t a_accmode;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_access_desc;

int VOP_ACCESS_AP(struct vop_access_args *);
int VOP_ACCESS_APV(const struct vop_vector *vop, struct vop_access_args *);

static __inline int VOP_ACCESS(
	struct vnode *vp,
	accmode_t accmode,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_access_args a;

	a.a_gen.a_desc = &vop_access_desc;
	a.a_vp = vp;
	a.a_accmode = accmode;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_access(&a));
	else
		return (VOP_ACCESS_APV(vp->v_op, &a));
#else
	return (VOP_ACCESS_APV(vp->v_op, &a));
#endif
}

struct vop_accessx_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	accmode_t a_accmode;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_accessx_desc;

int VOP_ACCESSX_AP(struct vop_accessx_args *);
int VOP_ACCESSX_APV(const struct vop_vector *vop, struct vop_accessx_args *);

static __inline int VOP_ACCESSX(
	struct vnode *vp,
	accmode_t accmode,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_accessx_args a;

	a.a_gen.a_desc = &vop_accessx_desc;
	a.a_vp = vp;
	a.a_accmode = accmode;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_accessx(&a));
	else
		return (VOP_ACCESSX_APV(vp->v_op, &a));
#else
	return (VOP_ACCESSX_APV(vp->v_op, &a));
#endif
}

struct vop_stat_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct stat *a_sb;
	struct ucred *a_active_cred;
	struct ucred *a_file_cred;
};

extern struct vnodeop_desc vop_stat_desc;

int VOP_STAT_AP(struct vop_stat_args *);
int VOP_STAT_APV(const struct vop_vector *vop, struct vop_stat_args *);

static __inline int VOP_STAT(
	struct vnode *vp,
	struct stat *sb,
	struct ucred *active_cred,
	struct ucred *file_cred)
{
	struct vop_stat_args a;

	a.a_gen.a_desc = &vop_stat_desc;
	a.a_vp = vp;
	a.a_sb = sb;
	a.a_active_cred = active_cred;
	a.a_file_cred = file_cred;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_stat(&a));
	else
		return (VOP_STAT_APV(vp->v_op, &a));
#else
	return (VOP_STAT_APV(vp->v_op, &a));
#endif
}

struct vop_getattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct vattr *a_vap;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_getattr_desc;

int VOP_GETATTR_AP(struct vop_getattr_args *);
int VOP_GETATTR_APV(const struct vop_vector *vop, struct vop_getattr_args *);

static __inline int VOP_GETATTR(
	struct vnode *vp,
	struct vattr *vap,
	struct ucred *cred)
{
	struct vop_getattr_args a;

	a.a_gen.a_desc = &vop_getattr_desc;
	a.a_vp = vp;
	a.a_vap = vap;
	a.a_cred = cred;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getattr(&a));
	else
		return (VOP_GETATTR_APV(vp->v_op, &a));
#else
	return (VOP_GETATTR_APV(vp->v_op, &a));
#endif
}

struct vop_setattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct vattr *a_vap;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_setattr_desc;

int VOP_SETATTR_AP(struct vop_setattr_args *);
int VOP_SETATTR_APV(const struct vop_vector *vop, struct vop_setattr_args *);

static __inline int VOP_SETATTR(
	struct vnode *vp,
	struct vattr *vap,
	struct ucred *cred)
{
	struct vop_setattr_args a;

	a.a_gen.a_desc = &vop_setattr_desc;
	a.a_vp = vp;
	a.a_vap = vap;
	a.a_cred = cred;
	return (VOP_SETATTR_APV(vp->v_op, &a));
}

struct vop_mmapped_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_mmapped_desc;

int VOP_MMAPPED_AP(struct vop_mmapped_args *);
int VOP_MMAPPED_APV(const struct vop_vector *vop, struct vop_mmapped_args *);

static __inline int VOP_MMAPPED(
	struct vnode *vp)
{
	struct vop_mmapped_args a;

	a.a_gen.a_desc = &vop_mmapped_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_mmapped(&a));
	else
		return (VOP_MMAPPED_APV(vp->v_op, &a));
#else
	return (VOP_MMAPPED_APV(vp->v_op, &a));
#endif
}

struct vop_read_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_read_desc;

int VOP_READ_AP(struct vop_read_args *);
int VOP_READ_APV(const struct vop_vector *vop, struct vop_read_args *);

static __inline int VOP_READ(
	struct vnode *vp,
	struct uio *uio,
	int ioflag,
	struct ucred *cred)
{
	struct vop_read_args a;

	a.a_gen.a_desc = &vop_read_desc;
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VOP_READ_APV(vp->v_op, &a));
}

struct vop_read_pgcache_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_read_pgcache_desc;

int VOP_READ_PGCACHE_AP(struct vop_read_pgcache_args *);
int VOP_READ_PGCACHE_APV(const struct vop_vector *vop, struct vop_read_pgcache_args *);

static __inline int VOP_READ_PGCACHE(
	struct vnode *vp,
	struct uio *uio,
	int ioflag,
	struct ucred *cred)
{
	struct vop_read_pgcache_args a;

	a.a_gen.a_desc = &vop_read_pgcache_desc;
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VOP_READ_PGCACHE_APV(vp->v_op, &a));
}

struct vop_write_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_write_desc;

int VOP_WRITE_AP(struct vop_write_args *);
int VOP_WRITE_APV(const struct vop_vector *vop, struct vop_write_args *);

static __inline int VOP_WRITE(
	struct vnode *vp,
	struct uio *uio,
	int ioflag,
	struct ucred *cred)
{
	struct vop_write_args a;

	a.a_gen.a_desc = &vop_write_desc;
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VOP_WRITE_APV(vp->v_op, &a));
}

struct vop_ioctl_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	u_long a_command;
	void *a_data;
	int a_fflag;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_ioctl_desc;

int VOP_IOCTL_AP(struct vop_ioctl_args *);
int VOP_IOCTL_APV(const struct vop_vector *vop, struct vop_ioctl_args *);

static __inline int VOP_IOCTL(
	struct vnode *vp,
	u_long command,
	void *data,
	int fflag,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_ioctl_args a;

	a.a_gen.a_desc = &vop_ioctl_desc;
	a.a_vp = vp;
	a.a_command = command;
	a.a_data = data;
	a.a_fflag = fflag;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_ioctl(&a));
	else
		return (VOP_IOCTL_APV(vp->v_op, &a));
#else
	return (VOP_IOCTL_APV(vp->v_op, &a));
#endif
}

struct vop_poll_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_events;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_poll_desc;

int VOP_POLL_AP(struct vop_poll_args *);
int VOP_POLL_APV(const struct vop_vector *vop, struct vop_poll_args *);

static __inline int VOP_POLL(
	struct vnode *vp,
	int events,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_poll_args a;

	a.a_gen.a_desc = &vop_poll_desc;
	a.a_vp = vp;
	a.a_events = events;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_poll(&a));
	else
		return (VOP_POLL_APV(vp->v_op, &a));
#else
	return (VOP_POLL_APV(vp->v_op, &a));
#endif
}

struct vop_kqfilter_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct knote *a_kn;
};

extern struct vnodeop_desc vop_kqfilter_desc;

int VOP_KQFILTER_AP(struct vop_kqfilter_args *);
int VOP_KQFILTER_APV(const struct vop_vector *vop, struct vop_kqfilter_args *);

static __inline int VOP_KQFILTER(
	struct vnode *vp,
	struct knote *kn)
{
	struct vop_kqfilter_args a;

	a.a_gen.a_desc = &vop_kqfilter_desc;
	a.a_vp = vp;
	a.a_kn = kn;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_kqfilter(&a));
	else
		return (VOP_KQFILTER_APV(vp->v_op, &a));
#else
	return (VOP_KQFILTER_APV(vp->v_op, &a));
#endif
}

struct vop_revoke_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_flags;
};

extern struct vnodeop_desc vop_revoke_desc;

int VOP_REVOKE_AP(struct vop_revoke_args *);
int VOP_REVOKE_APV(const struct vop_vector *vop, struct vop_revoke_args *);

static __inline int VOP_REVOKE(
	struct vnode *vp,
	int flags)
{
	struct vop_revoke_args a;

	a.a_gen.a_desc = &vop_revoke_desc;
	a.a_vp = vp;
	a.a_flags = flags;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_revoke(&a));
	else
		return (VOP_REVOKE_APV(vp->v_op, &a));
#else
	return (VOP_REVOKE_APV(vp->v_op, &a));
#endif
}

struct vop_fsync_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_waitfor;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_fsync_desc;

int VOP_FSYNC_AP(struct vop_fsync_args *);
int VOP_FSYNC_APV(const struct vop_vector *vop, struct vop_fsync_args *);

static __inline int VOP_FSYNC(
	struct vnode *vp,
	int waitfor,
	struct thread *td)
{
	struct vop_fsync_args a;

	a.a_gen.a_desc = &vop_fsync_desc;
	a.a_vp = vp;
	a.a_waitfor = waitfor;
	a.a_td = td;
	return (VOP_FSYNC_APV(vp->v_op, &a));
}

struct vop_remove_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};

extern struct vnodeop_desc vop_remove_desc;

int VOP_REMOVE_AP(struct vop_remove_args *);
int VOP_REMOVE_APV(const struct vop_vector *vop, struct vop_remove_args *);

static __inline int VOP_REMOVE(
	struct vnode *dvp,
	struct vnode *vp,
	struct componentname *cnp)
{
	struct vop_remove_args a;

	a.a_gen.a_desc = &vop_remove_desc;
	a.a_dvp = dvp;
	a.a_vp = vp;
	a.a_cnp = cnp;
	return (VOP_REMOVE_APV(dvp->v_op, &a));
}

struct vop_link_args {
	struct vop_generic_args a_gen;
	struct vnode *a_tdvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};

extern struct vnodeop_desc vop_link_desc;

int VOP_LINK_AP(struct vop_link_args *);
int VOP_LINK_APV(const struct vop_vector *vop, struct vop_link_args *);

static __inline int VOP_LINK(
	struct vnode *tdvp,
	struct vnode *vp,
	struct componentname *cnp)
{
	struct vop_link_args a;

	a.a_gen.a_desc = &vop_link_desc;
	a.a_tdvp = tdvp;
	a.a_vp = vp;
	a.a_cnp = cnp;
	return (VOP_LINK_APV(tdvp->v_op, &a));
}

struct vop_rename_args {
	struct vop_generic_args a_gen;
	struct vnode *a_fdvp;
	struct vnode *a_fvp;
	struct componentname *a_fcnp;
	struct vnode *a_tdvp;
	struct vnode *a_tvp;
	struct componentname *a_tcnp;
	u_int a_flags;
};

extern struct vnodeop_desc vop_rename_desc;

int VOP_RENAME_AP(struct vop_rename_args *);
int VOP_RENAME_APV(const struct vop_vector *vop, struct vop_rename_args *);

static __inline int VOP_RENAME(
	struct vnode *fdvp,
	struct vnode *fvp,
	struct componentname *fcnp,
	struct vnode *tdvp,
	struct vnode *tvp,
	struct componentname *tcnp,
	u_int flags)
{
	struct vop_rename_args a;

	a.a_gen.a_desc = &vop_rename_desc;
	a.a_fdvp = fdvp;
	a.a_fvp = fvp;
	a.a_fcnp = fcnp;
	a.a_tdvp = tdvp;
	a.a_tvp = tvp;
	a.a_tcnp = tcnp;
	a.a_flags = flags;
	return (VOP_RENAME_APV(fdvp->v_op, &a));
}

struct vop_mkdir_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};

extern struct vnodeop_desc vop_mkdir_desc;

int VOP_MKDIR_AP(struct vop_mkdir_args *);
int VOP_MKDIR_APV(const struct vop_vector *vop, struct vop_mkdir_args *);

static __inline int VOP_MKDIR(
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap)
{
	struct vop_mkdir_args a;

	a.a_gen.a_desc = &vop_mkdir_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	return (VOP_MKDIR_APV(dvp->v_op, &a));
}

struct vop_rmdir_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};

extern struct vnodeop_desc vop_rmdir_desc;

int VOP_RMDIR_AP(struct vop_rmdir_args *);
int VOP_RMDIR_APV(const struct vop_vector *vop, struct vop_rmdir_args *);

static __inline int VOP_RMDIR(
	struct vnode *dvp,
	struct vnode *vp,
	struct componentname *cnp)
{
	struct vop_rmdir_args a;

	a.a_gen.a_desc = &vop_rmdir_desc;
	a.a_dvp = dvp;
	a.a_vp = vp;
	a.a_cnp = cnp;
	return (VOP_RMDIR_APV(dvp->v_op, &a));
}

struct vop_symlink_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
	const char *a_target;
};

extern struct vnodeop_desc vop_symlink_desc;

int VOP_SYMLINK_AP(struct vop_symlink_args *);
int VOP_SYMLINK_APV(const struct vop_vector *vop, struct vop_symlink_args *);

static __inline int VOP_SYMLINK(
	struct vnode *dvp,
	struct vnode **vpp,
	struct componentname *cnp,
	struct vattr *vap,
	const char *target)
{
	struct vop_symlink_args a;

	a.a_gen.a_desc = &vop_symlink_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_cnp = cnp;
	a.a_vap = vap;
	a.a_target = target;
	return (VOP_SYMLINK_APV(dvp->v_op, &a));
}

struct vop_readdir_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct uio *a_uio;
	struct ucred *a_cred;
	int *a_eofflag;
	int *a_ncookies;
	uint64_t **a_cookies;
};

extern struct vnodeop_desc vop_readdir_desc;

int VOP_READDIR_AP(struct vop_readdir_args *);
int VOP_READDIR_APV(const struct vop_vector *vop, struct vop_readdir_args *);

static __inline int VOP_READDIR(
	struct vnode *vp,
	struct uio *uio,
	struct ucred *cred,
	int *eofflag,
	int *ncookies,
	uint64_t **cookies)
{
	struct vop_readdir_args a;

	a.a_gen.a_desc = &vop_readdir_desc;
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_cred = cred;
	a.a_eofflag = eofflag;
	a.a_ncookies = ncookies;
	a.a_cookies = cookies;
	return (VOP_READDIR_APV(vp->v_op, &a));
}

struct vop_readlink_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct uio *a_uio;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_readlink_desc;

int VOP_READLINK_AP(struct vop_readlink_args *);
int VOP_READLINK_APV(const struct vop_vector *vop, struct vop_readlink_args *);

static __inline int VOP_READLINK(
	struct vnode *vp,
	struct uio *uio,
	struct ucred *cred)
{
	struct vop_readlink_args a;

	a.a_gen.a_desc = &vop_readlink_desc;
	a.a_vp = vp;
	a.a_uio = uio;
	a.a_cred = cred;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_readlink(&a));
	else
		return (VOP_READLINK_APV(vp->v_op, &a));
#else
	return (VOP_READLINK_APV(vp->v_op, &a));
#endif
}

struct vop_inactive_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_inactive_desc;

int VOP_INACTIVE_AP(struct vop_inactive_args *);
int VOP_INACTIVE_APV(const struct vop_vector *vop, struct vop_inactive_args *);

static __inline int VOP_INACTIVE(
	struct vnode *vp)
{
	struct vop_inactive_args a;

	a.a_gen.a_desc = &vop_inactive_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_inactive(&a));
	else
		return (VOP_INACTIVE_APV(vp->v_op, &a));
#else
	return (VOP_INACTIVE_APV(vp->v_op, &a));
#endif
}

struct vop_need_inactive_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_need_inactive_desc;

int VOP_NEED_INACTIVE_AP(struct vop_need_inactive_args *);
int VOP_NEED_INACTIVE_APV(const struct vop_vector *vop, struct vop_need_inactive_args *);

static __inline int VOP_NEED_INACTIVE(
	struct vnode *vp)
{
	struct vop_need_inactive_args a;

	a.a_gen.a_desc = &vop_need_inactive_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_need_inactive(&a));
	else
		return (VOP_NEED_INACTIVE_APV(vp->v_op, &a));
#else
	return (VOP_NEED_INACTIVE_APV(vp->v_op, &a));
#endif
}

struct vop_reclaim_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_reclaim_desc;

int VOP_RECLAIM_AP(struct vop_reclaim_args *);
int VOP_RECLAIM_APV(const struct vop_vector *vop, struct vop_reclaim_args *);

static __inline int VOP_RECLAIM(
	struct vnode *vp)
{
	struct vop_reclaim_args a;

	a.a_gen.a_desc = &vop_reclaim_desc;
	a.a_vp = vp;
	return (VOP_RECLAIM_APV(vp->v_op, &a));
}

struct vop_lock1_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_flags;
	const char *a_file;
	int a_line;
};

extern struct vnodeop_desc vop_lock1_desc;

int VOP_LOCK1_AP(struct vop_lock1_args *);
int VOP_LOCK1_APV(const struct vop_vector *vop, struct vop_lock1_args *);

static __inline int VOP_LOCK1(
	struct vnode *vp,
	int flags,
	const char *file,
	int line)
{
	struct vop_lock1_args a;

	a.a_gen.a_desc = &vop_lock1_desc;
	a.a_vp = vp;
	a.a_flags = flags;
	a.a_file = file;
	a.a_line = line;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_lock1(&a));
	else
		return (VOP_LOCK1_APV(vp->v_op, &a));
#else
	return (VOP_LOCK1_APV(vp->v_op, &a));
#endif
}

struct vop_unlock_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_unlock_desc;

int VOP_UNLOCK_AP(struct vop_unlock_args *);
int VOP_UNLOCK_APV(const struct vop_vector *vop, struct vop_unlock_args *);

static __inline int VOP_UNLOCK(
	struct vnode *vp)
{
	struct vop_unlock_args a;

	a.a_gen.a_desc = &vop_unlock_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_unlock(&a));
	else
		return (VOP_UNLOCK_APV(vp->v_op, &a));
#else
	return (VOP_UNLOCK_APV(vp->v_op, &a));
#endif
}

struct vop_bmap_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	daddr_t a_bn;
	struct bufobj **a_bop;
	daddr_t *a_bnp;
	int *a_runp;
	int *a_runb;
};

extern struct vnodeop_desc vop_bmap_desc;

int VOP_BMAP_AP(struct vop_bmap_args *);
int VOP_BMAP_APV(const struct vop_vector *vop, struct vop_bmap_args *);

static __inline int VOP_BMAP(
	struct vnode *vp,
	daddr_t bn,
	struct bufobj **bop,
	daddr_t *bnp,
	int *runp,
	int *runb)
{
	struct vop_bmap_args a;

	a.a_gen.a_desc = &vop_bmap_desc;
	a.a_vp = vp;
	a.a_bn = bn;
	a.a_bop = bop;
	a.a_bnp = bnp;
	a.a_runp = runp;
	a.a_runb = runb;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_bmap(&a));
	else
		return (VOP_BMAP_APV(vp->v_op, &a));
#else
	return (VOP_BMAP_APV(vp->v_op, &a));
#endif
}

struct vop_strategy_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct buf *a_bp;
};

extern struct vnodeop_desc vop_strategy_desc;

int VOP_STRATEGY_AP(struct vop_strategy_args *);
int VOP_STRATEGY_APV(const struct vop_vector *vop, struct vop_strategy_args *);

static __inline int VOP_STRATEGY(
	struct vnode *vp,
	struct buf *bp)
{
	struct vop_strategy_args a;

	a.a_gen.a_desc = &vop_strategy_desc;
	a.a_vp = vp;
	a.a_bp = bp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_strategy(&a));
	else
		return (VOP_STRATEGY_APV(vp->v_op, &a));
#else
	return (VOP_STRATEGY_APV(vp->v_op, &a));
#endif
}

struct vop_getwritemount_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct mount **a_mpp;
};

extern struct vnodeop_desc vop_getwritemount_desc;

int VOP_GETWRITEMOUNT_AP(struct vop_getwritemount_args *);
int VOP_GETWRITEMOUNT_APV(const struct vop_vector *vop, struct vop_getwritemount_args *);

static __inline int VOP_GETWRITEMOUNT(
	struct vnode *vp,
	struct mount **mpp)
{
	struct vop_getwritemount_args a;

	a.a_gen.a_desc = &vop_getwritemount_desc;
	a.a_vp = vp;
	a.a_mpp = mpp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getwritemount(&a));
	else
		return (VOP_GETWRITEMOUNT_APV(vp->v_op, &a));
#else
	return (VOP_GETWRITEMOUNT_APV(vp->v_op, &a));
#endif
}

struct vop_getlowvnode_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct vnode **a_vplp;
	int a_flags;
};

extern struct vnodeop_desc vop_getlowvnode_desc;

int VOP_GETLOWVNODE_AP(struct vop_getlowvnode_args *);
int VOP_GETLOWVNODE_APV(const struct vop_vector *vop, struct vop_getlowvnode_args *);

static __inline int VOP_GETLOWVNODE(
	struct vnode *vp,
	struct vnode **vplp,
	int flags)
{
	struct vop_getlowvnode_args a;

	a.a_gen.a_desc = &vop_getlowvnode_desc;
	a.a_vp = vp;
	a.a_vplp = vplp;
	a.a_flags = flags;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getlowvnode(&a));
	else
		return (VOP_GETLOWVNODE_APV(vp->v_op, &a));
#else
	return (VOP_GETLOWVNODE_APV(vp->v_op, &a));
#endif
}

struct vop_print_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_print_desc;

int VOP_PRINT_AP(struct vop_print_args *);
int VOP_PRINT_APV(const struct vop_vector *vop, struct vop_print_args *);

static __inline int VOP_PRINT(
	struct vnode *vp)
{
	struct vop_print_args a;

	a.a_gen.a_desc = &vop_print_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_print(&a));
	else
		return (VOP_PRINT_APV(vp->v_op, &a));
#else
	return (VOP_PRINT_APV(vp->v_op, &a));
#endif
}

struct vop_pathconf_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_name;
	long *a_retval;
};

extern struct vnodeop_desc vop_pathconf_desc;

int VOP_PATHCONF_AP(struct vop_pathconf_args *);
int VOP_PATHCONF_APV(const struct vop_vector *vop, struct vop_pathconf_args *);

static __inline int VOP_PATHCONF(
	struct vnode *vp,
	int name,
	long *retval)
{
	struct vop_pathconf_args a;

	a.a_gen.a_desc = &vop_pathconf_desc;
	a.a_vp = vp;
	a.a_name = name;
	a.a_retval = retval;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_pathconf(&a));
	else
		return (VOP_PATHCONF_APV(vp->v_op, &a));
#else
	return (VOP_PATHCONF_APV(vp->v_op, &a));
#endif
}

struct vop_advlock_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	void *a_id;
	int a_op;
	struct flock *a_fl;
	int a_flags;
};

extern struct vnodeop_desc vop_advlock_desc;

int VOP_ADVLOCK_AP(struct vop_advlock_args *);
int VOP_ADVLOCK_APV(const struct vop_vector *vop, struct vop_advlock_args *);

static __inline int VOP_ADVLOCK(
	struct vnode *vp,
	void *id,
	int op,
	struct flock *fl,
	int flags)
{
	struct vop_advlock_args a;

	a.a_gen.a_desc = &vop_advlock_desc;
	a.a_vp = vp;
	a.a_id = id;
	a.a_op = op;
	a.a_fl = fl;
	a.a_flags = flags;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_advlock(&a));
	else
		return (VOP_ADVLOCK_APV(vp->v_op, &a));
#else
	return (VOP_ADVLOCK_APV(vp->v_op, &a));
#endif
}

struct vop_advlockasync_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	void *a_id;
	int a_op;
	struct flock *a_fl;
	int a_flags;
	struct task *a_task;
	void **a_cookiep;
};

extern struct vnodeop_desc vop_advlockasync_desc;

int VOP_ADVLOCKASYNC_AP(struct vop_advlockasync_args *);
int VOP_ADVLOCKASYNC_APV(const struct vop_vector *vop, struct vop_advlockasync_args *);

static __inline int VOP_ADVLOCKASYNC(
	struct vnode *vp,
	void *id,
	int op,
	struct flock *fl,
	int flags,
	struct task *task,
	void **cookiep)
{
	struct vop_advlockasync_args a;

	a.a_gen.a_desc = &vop_advlockasync_desc;
	a.a_vp = vp;
	a.a_id = id;
	a.a_op = op;
	a.a_fl = fl;
	a.a_flags = flags;
	a.a_task = task;
	a.a_cookiep = cookiep;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_advlockasync(&a));
	else
		return (VOP_ADVLOCKASYNC_APV(vp->v_op, &a));
#else
	return (VOP_ADVLOCKASYNC_APV(vp->v_op, &a));
#endif
}

struct vop_advlockpurge_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_advlockpurge_desc;

int VOP_ADVLOCKPURGE_AP(struct vop_advlockpurge_args *);
int VOP_ADVLOCKPURGE_APV(const struct vop_vector *vop, struct vop_advlockpurge_args *);

static __inline int VOP_ADVLOCKPURGE(
	struct vnode *vp)
{
	struct vop_advlockpurge_args a;

	a.a_gen.a_desc = &vop_advlockpurge_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_advlockpurge(&a));
	else
		return (VOP_ADVLOCKPURGE_APV(vp->v_op, &a));
#else
	return (VOP_ADVLOCKPURGE_APV(vp->v_op, &a));
#endif
}

struct vop_reallocblks_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct cluster_save *a_buflist;
};

extern struct vnodeop_desc vop_reallocblks_desc;

int VOP_REALLOCBLKS_AP(struct vop_reallocblks_args *);
int VOP_REALLOCBLKS_APV(const struct vop_vector *vop, struct vop_reallocblks_args *);

static __inline int VOP_REALLOCBLKS(
	struct vnode *vp,
	struct cluster_save *buflist)
{
	struct vop_reallocblks_args a;

	a.a_gen.a_desc = &vop_reallocblks_desc;
	a.a_vp = vp;
	a.a_buflist = buflist;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_reallocblks(&a));
	else
		return (VOP_REALLOCBLKS_APV(vp->v_op, &a));
#else
	return (VOP_REALLOCBLKS_APV(vp->v_op, &a));
#endif
}

struct vop_getpages_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	vm_page_t *a_m;
	int a_count;
	int *a_rbehind;
	int *a_rahead;
};

extern struct vnodeop_desc vop_getpages_desc;

int VOP_GETPAGES_AP(struct vop_getpages_args *);
int VOP_GETPAGES_APV(const struct vop_vector *vop, struct vop_getpages_args *);

static __inline int VOP_GETPAGES(
	struct vnode *vp,
	vm_page_t *m,
	int count,
	int *rbehind,
	int *rahead)
{
	struct vop_getpages_args a;

	a.a_gen.a_desc = &vop_getpages_desc;
	a.a_vp = vp;
	a.a_m = m;
	a.a_count = count;
	a.a_rbehind = rbehind;
	a.a_rahead = rahead;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getpages(&a));
	else
		return (VOP_GETPAGES_APV(vp->v_op, &a));
#else
	return (VOP_GETPAGES_APV(vp->v_op, &a));
#endif
}

struct vop_getpages_async_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	vm_page_t *a_m;
	int a_count;
	int *a_rbehind;
	int *a_rahead;
	vop_getpages_iodone_t *a_iodone;
	void *a_arg;
};

extern struct vnodeop_desc vop_getpages_async_desc;

int VOP_GETPAGES_ASYNC_AP(struct vop_getpages_async_args *);
int VOP_GETPAGES_ASYNC_APV(const struct vop_vector *vop, struct vop_getpages_async_args *);

static __inline int VOP_GETPAGES_ASYNC(
	struct vnode *vp,
	vm_page_t *m,
	int count,
	int *rbehind,
	int *rahead,
	vop_getpages_iodone_t *iodone,
	void *arg)
{
	struct vop_getpages_async_args a;

	a.a_gen.a_desc = &vop_getpages_async_desc;
	a.a_vp = vp;
	a.a_m = m;
	a.a_count = count;
	a.a_rbehind = rbehind;
	a.a_rahead = rahead;
	a.a_iodone = iodone;
	a.a_arg = arg;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getpages_async(&a));
	else
		return (VOP_GETPAGES_ASYNC_APV(vp->v_op, &a));
#else
	return (VOP_GETPAGES_ASYNC_APV(vp->v_op, &a));
#endif
}

struct vop_putpages_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	vm_page_t *a_m;
	int a_count;
	int a_sync;
	int *a_rtvals;
};

extern struct vnodeop_desc vop_putpages_desc;

int VOP_PUTPAGES_AP(struct vop_putpages_args *);
int VOP_PUTPAGES_APV(const struct vop_vector *vop, struct vop_putpages_args *);

static __inline int VOP_PUTPAGES(
	struct vnode *vp,
	vm_page_t *m,
	int count,
	int sync,
	int *rtvals)
{
	struct vop_putpages_args a;

	a.a_gen.a_desc = &vop_putpages_desc;
	a.a_vp = vp;
	a.a_m = m;
	a.a_count = count;
	a.a_sync = sync;
	a.a_rtvals = rtvals;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_putpages(&a));
	else
		return (VOP_PUTPAGES_APV(vp->v_op, &a));
#else
	return (VOP_PUTPAGES_APV(vp->v_op, &a));
#endif
}

struct vop_getacl_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	acl_type_t a_type;
	struct acl *a_aclp;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_getacl_desc;

int VOP_GETACL_AP(struct vop_getacl_args *);
int VOP_GETACL_APV(const struct vop_vector *vop, struct vop_getacl_args *);

static __inline int VOP_GETACL(
	struct vnode *vp,
	acl_type_t type,
	struct acl *aclp,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_getacl_args a;

	a.a_gen.a_desc = &vop_getacl_desc;
	a.a_vp = vp;
	a.a_type = type;
	a.a_aclp = aclp;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getacl(&a));
	else
		return (VOP_GETACL_APV(vp->v_op, &a));
#else
	return (VOP_GETACL_APV(vp->v_op, &a));
#endif
}

struct vop_setacl_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	acl_type_t a_type;
	struct acl *a_aclp;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_setacl_desc;

int VOP_SETACL_AP(struct vop_setacl_args *);
int VOP_SETACL_APV(const struct vop_vector *vop, struct vop_setacl_args *);

static __inline int VOP_SETACL(
	struct vnode *vp,
	acl_type_t type,
	struct acl *aclp,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_setacl_args a;

	a.a_gen.a_desc = &vop_setacl_desc;
	a.a_vp = vp;
	a.a_type = type;
	a.a_aclp = aclp;
	a.a_cred = cred;
	a.a_td = td;
	return (VOP_SETACL_APV(vp->v_op, &a));
}

struct vop_aclcheck_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	acl_type_t a_type;
	struct acl *a_aclp;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_aclcheck_desc;

int VOP_ACLCHECK_AP(struct vop_aclcheck_args *);
int VOP_ACLCHECK_APV(const struct vop_vector *vop, struct vop_aclcheck_args *);

static __inline int VOP_ACLCHECK(
	struct vnode *vp,
	acl_type_t type,
	struct acl *aclp,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_aclcheck_args a;

	a.a_gen.a_desc = &vop_aclcheck_desc;
	a.a_vp = vp;
	a.a_type = type;
	a.a_aclp = aclp;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_aclcheck(&a));
	else
		return (VOP_ACLCHECK_APV(vp->v_op, &a));
#else
	return (VOP_ACLCHECK_APV(vp->v_op, &a));
#endif
}

struct vop_closeextattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_commit;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_closeextattr_desc;

int VOP_CLOSEEXTATTR_AP(struct vop_closeextattr_args *);
int VOP_CLOSEEXTATTR_APV(const struct vop_vector *vop, struct vop_closeextattr_args *);

static __inline int VOP_CLOSEEXTATTR(
	struct vnode *vp,
	int commit,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_closeextattr_args a;

	a.a_gen.a_desc = &vop_closeextattr_desc;
	a.a_vp = vp;
	a.a_commit = commit;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_closeextattr(&a));
	else
		return (VOP_CLOSEEXTATTR_APV(vp->v_op, &a));
#else
	return (VOP_CLOSEEXTATTR_APV(vp->v_op, &a));
#endif
}

struct vop_getextattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_attrnamespace;
	const char *a_name;
	struct uio *a_uio;
	size_t *a_size;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_getextattr_desc;

int VOP_GETEXTATTR_AP(struct vop_getextattr_args *);
int VOP_GETEXTATTR_APV(const struct vop_vector *vop, struct vop_getextattr_args *);

static __inline int VOP_GETEXTATTR(
	struct vnode *vp,
	int attrnamespace,
	const char *name,
	struct uio *uio,
	size_t *size,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_getextattr_args a;

	a.a_gen.a_desc = &vop_getextattr_desc;
	a.a_vp = vp;
	a.a_attrnamespace = attrnamespace;
	a.a_name = name;
	a.a_uio = uio;
	a.a_size = size;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_getextattr(&a));
	else
		return (VOP_GETEXTATTR_APV(vp->v_op, &a));
#else
	return (VOP_GETEXTATTR_APV(vp->v_op, &a));
#endif
}

struct vop_listextattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_attrnamespace;
	struct uio *a_uio;
	size_t *a_size;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_listextattr_desc;

int VOP_LISTEXTATTR_AP(struct vop_listextattr_args *);
int VOP_LISTEXTATTR_APV(const struct vop_vector *vop, struct vop_listextattr_args *);

static __inline int VOP_LISTEXTATTR(
	struct vnode *vp,
	int attrnamespace,
	struct uio *uio,
	size_t *size,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_listextattr_args a;

	a.a_gen.a_desc = &vop_listextattr_desc;
	a.a_vp = vp;
	a.a_attrnamespace = attrnamespace;
	a.a_uio = uio;
	a.a_size = size;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_listextattr(&a));
	else
		return (VOP_LISTEXTATTR_APV(vp->v_op, &a));
#else
	return (VOP_LISTEXTATTR_APV(vp->v_op, &a));
#endif
}

struct vop_openextattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_openextattr_desc;

int VOP_OPENEXTATTR_AP(struct vop_openextattr_args *);
int VOP_OPENEXTATTR_APV(const struct vop_vector *vop, struct vop_openextattr_args *);

static __inline int VOP_OPENEXTATTR(
	struct vnode *vp,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_openextattr_args a;

	a.a_gen.a_desc = &vop_openextattr_desc;
	a.a_vp = vp;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_openextattr(&a));
	else
		return (VOP_OPENEXTATTR_APV(vp->v_op, &a));
#else
	return (VOP_OPENEXTATTR_APV(vp->v_op, &a));
#endif
}

struct vop_deleteextattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_attrnamespace;
	const char *a_name;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_deleteextattr_desc;

int VOP_DELETEEXTATTR_AP(struct vop_deleteextattr_args *);
int VOP_DELETEEXTATTR_APV(const struct vop_vector *vop, struct vop_deleteextattr_args *);

static __inline int VOP_DELETEEXTATTR(
	struct vnode *vp,
	int attrnamespace,
	const char *name,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_deleteextattr_args a;

	a.a_gen.a_desc = &vop_deleteextattr_desc;
	a.a_vp = vp;
	a.a_attrnamespace = attrnamespace;
	a.a_name = name;
	a.a_cred = cred;
	a.a_td = td;
	return (VOP_DELETEEXTATTR_APV(vp->v_op, &a));
}

struct vop_setextattr_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_attrnamespace;
	const char *a_name;
	struct uio *a_uio;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_setextattr_desc;

int VOP_SETEXTATTR_AP(struct vop_setextattr_args *);
int VOP_SETEXTATTR_APV(const struct vop_vector *vop, struct vop_setextattr_args *);

static __inline int VOP_SETEXTATTR(
	struct vnode *vp,
	int attrnamespace,
	const char *name,
	struct uio *uio,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_setextattr_args a;

	a.a_gen.a_desc = &vop_setextattr_desc;
	a.a_vp = vp;
	a.a_attrnamespace = attrnamespace;
	a.a_name = name;
	a.a_uio = uio;
	a.a_cred = cred;
	a.a_td = td;
	return (VOP_SETEXTATTR_APV(vp->v_op, &a));
}

struct vop_setlabel_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct label *a_label;
	struct ucred *a_cred;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_setlabel_desc;

int VOP_SETLABEL_AP(struct vop_setlabel_args *);
int VOP_SETLABEL_APV(const struct vop_vector *vop, struct vop_setlabel_args *);

static __inline int VOP_SETLABEL(
	struct vnode *vp,
	struct label *label,
	struct ucred *cred,
	struct thread *td)
{
	struct vop_setlabel_args a;

	a.a_gen.a_desc = &vop_setlabel_desc;
	a.a_vp = vp;
	a.a_label = label;
	a.a_cred = cred;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_setlabel(&a));
	else
		return (VOP_SETLABEL_APV(vp->v_op, &a));
#else
	return (VOP_SETLABEL_APV(vp->v_op, &a));
#endif
}

struct vop_vptofh_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct fid *a_fhp;
};

extern struct vnodeop_desc vop_vptofh_desc;

int VOP_VPTOFH_AP(struct vop_vptofh_args *);
int VOP_VPTOFH_APV(const struct vop_vector *vop, struct vop_vptofh_args *);

static __inline int VOP_VPTOFH(
	struct vnode *vp,
	struct fid *fhp)
{
	struct vop_vptofh_args a;

	a.a_gen.a_desc = &vop_vptofh_desc;
	a.a_vp = vp;
	a.a_fhp = fhp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_vptofh(&a));
	else
		return (VOP_VPTOFH_APV(vp->v_op, &a));
#else
	return (VOP_VPTOFH_APV(vp->v_op, &a));
#endif
}

struct vop_vptocnp_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct vnode **a_vpp;
	char *a_buf;
	size_t *a_buflen;
};

extern struct vnodeop_desc vop_vptocnp_desc;

int VOP_VPTOCNP_AP(struct vop_vptocnp_args *);
int VOP_VPTOCNP_APV(const struct vop_vector *vop, struct vop_vptocnp_args *);

static __inline int VOP_VPTOCNP(
	struct vnode *vp,
	struct vnode **vpp,
	char *buf,
	size_t *buflen)
{
	struct vop_vptocnp_args a;

	a.a_gen.a_desc = &vop_vptocnp_desc;
	a.a_vp = vp;
	a.a_vpp = vpp;
	a.a_buf = buf;
	a.a_buflen = buflen;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_vptocnp(&a));
	else
		return (VOP_VPTOCNP_APV(vp->v_op, &a));
#else
	return (VOP_VPTOCNP_APV(vp->v_op, &a));
#endif
}

struct vop_allocate_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	off_t *a_offset;
	off_t *a_len;
	int a_ioflag;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_allocate_desc;

int VOP_ALLOCATE_AP(struct vop_allocate_args *);
int VOP_ALLOCATE_APV(const struct vop_vector *vop, struct vop_allocate_args *);

static __inline int VOP_ALLOCATE(
	struct vnode *vp,
	off_t *offset,
	off_t *len,
	int ioflag,
	struct ucred *cred)
{
	struct vop_allocate_args a;

	a.a_gen.a_desc = &vop_allocate_desc;
	a.a_vp = vp;
	a.a_offset = offset;
	a.a_len = len;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VOP_ALLOCATE_APV(vp->v_op, &a));
}

struct vop_advise_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	off_t a_start;
	off_t a_end;
	int a_advice;
};

extern struct vnodeop_desc vop_advise_desc;

int VOP_ADVISE_AP(struct vop_advise_args *);
int VOP_ADVISE_APV(const struct vop_vector *vop, struct vop_advise_args *);

static __inline int VOP_ADVISE(
	struct vnode *vp,
	off_t start,
	off_t end,
	int advice)
{
	struct vop_advise_args a;

	a.a_gen.a_desc = &vop_advise_desc;
	a.a_vp = vp;
	a.a_start = start;
	a.a_end = end;
	a.a_advice = advice;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_advise(&a));
	else
		return (VOP_ADVISE_APV(vp->v_op, &a));
#else
	return (VOP_ADVISE_APV(vp->v_op, &a));
#endif
}

struct vop_unp_bind_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct unpcb *a_unpcb;
};

extern struct vnodeop_desc vop_unp_bind_desc;

int VOP_UNP_BIND_AP(struct vop_unp_bind_args *);
int VOP_UNP_BIND_APV(const struct vop_vector *vop, struct vop_unp_bind_args *);

static __inline int VOP_UNP_BIND(
	struct vnode *vp,
	struct unpcb *unpcb)
{
	struct vop_unp_bind_args a;

	a.a_gen.a_desc = &vop_unp_bind_desc;
	a.a_vp = vp;
	a.a_unpcb = unpcb;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_unp_bind(&a));
	else
		return (VOP_UNP_BIND_APV(vp->v_op, &a));
#else
	return (VOP_UNP_BIND_APV(vp->v_op, &a));
#endif
}

struct vop_unp_connect_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct unpcb **a_unpcb;
};

extern struct vnodeop_desc vop_unp_connect_desc;

int VOP_UNP_CONNECT_AP(struct vop_unp_connect_args *);
int VOP_UNP_CONNECT_APV(const struct vop_vector *vop, struct vop_unp_connect_args *);

static __inline int VOP_UNP_CONNECT(
	struct vnode *vp,
	struct unpcb **unpcb)
{
	struct vop_unp_connect_args a;

	a.a_gen.a_desc = &vop_unp_connect_desc;
	a.a_vp = vp;
	a.a_unpcb = unpcb;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_unp_connect(&a));
	else
		return (VOP_UNP_CONNECT_APV(vp->v_op, &a));
#else
	return (VOP_UNP_CONNECT_APV(vp->v_op, &a));
#endif
}

struct vop_unp_detach_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_unp_detach_desc;

int VOP_UNP_DETACH_AP(struct vop_unp_detach_args *);
int VOP_UNP_DETACH_APV(const struct vop_vector *vop, struct vop_unp_detach_args *);

static __inline int VOP_UNP_DETACH(
	struct vnode *vp)
{
	struct vop_unp_detach_args a;

	a.a_gen.a_desc = &vop_unp_detach_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_unp_detach(&a));
	else
		return (VOP_UNP_DETACH_APV(vp->v_op, &a));
#else
	return (VOP_UNP_DETACH_APV(vp->v_op, &a));
#endif
}

struct vop_is_text_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_is_text_desc;

int VOP_IS_TEXT_AP(struct vop_is_text_args *);
int VOP_IS_TEXT_APV(const struct vop_vector *vop, struct vop_is_text_args *);

static __inline int VOP_IS_TEXT(
	struct vnode *vp)
{
	struct vop_is_text_args a;

	a.a_gen.a_desc = &vop_is_text_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_is_text(&a));
	else
		return (VOP_IS_TEXT_APV(vp->v_op, &a));
#else
	return (VOP_IS_TEXT_APV(vp->v_op, &a));
#endif
}

struct vop_set_text_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_set_text_desc;

int VOP_SET_TEXT_AP(struct vop_set_text_args *);
int VOP_SET_TEXT_APV(const struct vop_vector *vop, struct vop_set_text_args *);

static __inline int VOP_SET_TEXT(
	struct vnode *vp)
{
	struct vop_set_text_args a;

	a.a_gen.a_desc = &vop_set_text_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_set_text(&a));
	else
		return (VOP_SET_TEXT_APV(vp->v_op, &a));
#else
	return (VOP_SET_TEXT_APV(vp->v_op, &a));
#endif
}

struct vop_unset_text_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_unset_text_desc;

int VOP_UNSET_TEXT_AP(struct vop_unset_text_args *);
int VOP_UNSET_TEXT_APV(const struct vop_vector *vop, struct vop_unset_text_args *);

static __inline int VOP_UNSET_TEXT(
	struct vnode *vp)
{
	struct vop_unset_text_args a;

	a.a_gen.a_desc = &vop_unset_text_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_unset_text(&a));
	else
		return (VOP_UNSET_TEXT_APV(vp->v_op, &a));
#else
	return (VOP_UNSET_TEXT_APV(vp->v_op, &a));
#endif
}

struct vop_add_writecount_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	int a_inc;
};

extern struct vnodeop_desc vop_add_writecount_desc;

int VOP_ADD_WRITECOUNT_AP(struct vop_add_writecount_args *);
int VOP_ADD_WRITECOUNT_APV(const struct vop_vector *vop, struct vop_add_writecount_args *);

static __inline int VOP_ADD_WRITECOUNT(
	struct vnode *vp,
	int inc)
{
	struct vop_add_writecount_args a;

	a.a_gen.a_desc = &vop_add_writecount_desc;
	a.a_vp = vp;
	a.a_inc = inc;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_add_writecount(&a));
	else
		return (VOP_ADD_WRITECOUNT_APV(vp->v_op, &a));
#else
	return (VOP_ADD_WRITECOUNT_APV(vp->v_op, &a));
#endif
}

struct vop_fdatasync_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_fdatasync_desc;

int VOP_FDATASYNC_AP(struct vop_fdatasync_args *);
int VOP_FDATASYNC_APV(const struct vop_vector *vop, struct vop_fdatasync_args *);

static __inline int VOP_FDATASYNC(
	struct vnode *vp,
	struct thread *td)
{
	struct vop_fdatasync_args a;

	a.a_gen.a_desc = &vop_fdatasync_desc;
	a.a_vp = vp;
	a.a_td = td;
	return (VOP_FDATASYNC_APV(vp->v_op, &a));
}

struct vop_copy_file_range_args {
	struct vop_generic_args a_gen;
	struct vnode *a_invp;
	off_t *a_inoffp;
	struct vnode *a_outvp;
	off_t *a_outoffp;
	size_t *a_lenp;
	unsigned int a_flags;
	struct ucred *a_incred;
	struct ucred *a_outcred;
	struct thread *a_fsizetd;
};

extern struct vnodeop_desc vop_copy_file_range_desc;

int VOP_COPY_FILE_RANGE_AP(struct vop_copy_file_range_args *);
int VOP_COPY_FILE_RANGE_APV(const struct vop_vector *vop, struct vop_copy_file_range_args *);

static __inline int VOP_COPY_FILE_RANGE(
	struct vnode *invp,
	off_t *inoffp,
	struct vnode *outvp,
	off_t *outoffp,
	size_t *lenp,
	unsigned int flags,
	struct ucred *incred,
	struct ucred *outcred,
	struct thread *fsizetd)
{
	struct vop_copy_file_range_args a;

	a.a_gen.a_desc = &vop_copy_file_range_desc;
	a.a_invp = invp;
	a.a_inoffp = inoffp;
	a.a_outvp = outvp;
	a.a_outoffp = outoffp;
	a.a_lenp = lenp;
	a.a_flags = flags;
	a.a_incred = incred;
	a.a_outcred = outcred;
	a.a_fsizetd = fsizetd;
	return (VOP_COPY_FILE_RANGE_APV(invp->v_op, &a));
}

struct vop_vput_pair_args {
	struct vop_generic_args a_gen;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	bool a_unlock_vp;
};

extern struct vnodeop_desc vop_vput_pair_desc;

int VOP_VPUT_PAIR_AP(struct vop_vput_pair_args *);
int VOP_VPUT_PAIR_APV(const struct vop_vector *vop, struct vop_vput_pair_args *);

static __inline int VOP_VPUT_PAIR(
	struct vnode *dvp,
	struct vnode **vpp,
	bool unlock_vp)
{
	struct vop_vput_pair_args a;

	a.a_gen.a_desc = &vop_vput_pair_desc;
	a.a_dvp = dvp;
	a.a_vpp = vpp;
	a.a_unlock_vp = unlock_vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (dvp->v_op->vop_vput_pair(&a));
	else
		return (VOP_VPUT_PAIR_APV(dvp->v_op, &a));
#else
	return (VOP_VPUT_PAIR_APV(dvp->v_op, &a));
#endif
}

struct vop_deallocate_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	off_t *a_offset;
	off_t *a_len;
	int a_flags;
	int a_ioflag;
	struct ucred *a_cred;
};

extern struct vnodeop_desc vop_deallocate_desc;

int VOP_DEALLOCATE_AP(struct vop_deallocate_args *);
int VOP_DEALLOCATE_APV(const struct vop_vector *vop, struct vop_deallocate_args *);

static __inline int VOP_DEALLOCATE(
	struct vnode *vp,
	off_t *offset,
	off_t *len,
	int flags,
	int ioflag,
	struct ucred *cred)
{
	struct vop_deallocate_args a;

	a.a_gen.a_desc = &vop_deallocate_desc;
	a.a_vp = vp;
	a.a_offset = offset;
	a.a_len = len;
	a.a_flags = flags;
	a.a_ioflag = ioflag;
	a.a_cred = cred;
	return (VOP_DEALLOCATE_APV(vp->v_op, &a));
}

struct vop_inotify_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct vnode *a_dvp;
	struct componentname *a_cnp;
	int a_event;
	uint32_t a_cookie;
};

extern struct vnodeop_desc vop_inotify_desc;

int VOP_INOTIFY_AP(struct vop_inotify_args *);
int VOP_INOTIFY_APV(const struct vop_vector *vop, struct vop_inotify_args *);

static __inline int VOP_INOTIFY(
	struct vnode *vp,
	struct vnode *dvp,
	struct componentname *cnp,
	int event,
	uint32_t cookie)
{
	struct vop_inotify_args a;

	a.a_gen.a_desc = &vop_inotify_desc;
	a.a_vp = vp;
	a.a_dvp = dvp;
	a.a_cnp = cnp;
	a.a_event = event;
	a.a_cookie = cookie;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_inotify(&a));
	else
		return (VOP_INOTIFY_APV(vp->v_op, &a));
#else
	return (VOP_INOTIFY_APV(vp->v_op, &a));
#endif
}

struct vop_inotify_add_watch_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
	struct inotify_softc *a_sc;
	uint32_t a_mask;
	uint32_t *a_wdp;
	struct thread *a_td;
};

extern struct vnodeop_desc vop_inotify_add_watch_desc;

int VOP_INOTIFY_ADD_WATCH_AP(struct vop_inotify_add_watch_args *);
int VOP_INOTIFY_ADD_WATCH_APV(const struct vop_vector *vop, struct vop_inotify_add_watch_args *);

static __inline int VOP_INOTIFY_ADD_WATCH(
	struct vnode *vp,
	struct inotify_softc *sc,
	uint32_t mask,
	uint32_t *wdp,
	struct thread *td)
{
	struct vop_inotify_add_watch_args a;

	a.a_gen.a_desc = &vop_inotify_add_watch_desc;
	a.a_vp = vp;
	a.a_sc = sc;
	a.a_mask = mask;
	a.a_wdp = wdp;
	a.a_td = td;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_inotify_add_watch(&a));
	else
		return (VOP_INOTIFY_ADD_WATCH_APV(vp->v_op, &a));
#else
	return (VOP_INOTIFY_ADD_WATCH_APV(vp->v_op, &a));
#endif
}

struct vop_delayed_setsize_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_delayed_setsize_desc;

int VOP_DELAYED_SETSIZE_AP(struct vop_delayed_setsize_args *);
int VOP_DELAYED_SETSIZE_APV(const struct vop_vector *vop, struct vop_delayed_setsize_args *);

static __inline int VOP_DELAYED_SETSIZE(
	struct vnode *vp)
{
	struct vop_delayed_setsize_args a;

	a.a_gen.a_desc = &vop_delayed_setsize_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_delayed_setsize(&a));
	else
		return (VOP_DELAYED_SETSIZE_APV(vp->v_op, &a));
#else
	return (VOP_DELAYED_SETSIZE_APV(vp->v_op, &a));
#endif
}

struct vop_spare1_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_spare1_desc;

int VOP_SPARE1_AP(struct vop_spare1_args *);
int VOP_SPARE1_APV(const struct vop_vector *vop, struct vop_spare1_args *);

static __inline int VOP_SPARE1(
	struct vnode *vp)
{
	struct vop_spare1_args a;

	a.a_gen.a_desc = &vop_spare1_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_spare1(&a));
	else
		return (VOP_SPARE1_APV(vp->v_op, &a));
#else
	return (VOP_SPARE1_APV(vp->v_op, &a));
#endif
}

struct vop_spare2_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_spare2_desc;

int VOP_SPARE2_AP(struct vop_spare2_args *);
int VOP_SPARE2_APV(const struct vop_vector *vop, struct vop_spare2_args *);

static __inline int VOP_SPARE2(
	struct vnode *vp)
{
	struct vop_spare2_args a;

	a.a_gen.a_desc = &vop_spare2_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_spare2(&a));
	else
		return (VOP_SPARE2_APV(vp->v_op, &a));
#else
	return (VOP_SPARE2_APV(vp->v_op, &a));
#endif
}

struct vop_spare3_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_spare3_desc;

int VOP_SPARE3_AP(struct vop_spare3_args *);
int VOP_SPARE3_APV(const struct vop_vector *vop, struct vop_spare3_args *);

static __inline int VOP_SPARE3(
	struct vnode *vp)
{
	struct vop_spare3_args a;

	a.a_gen.a_desc = &vop_spare3_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_spare3(&a));
	else
		return (VOP_SPARE3_APV(vp->v_op, &a));
#else
	return (VOP_SPARE3_APV(vp->v_op, &a));
#endif
}

struct vop_spare4_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_spare4_desc;

int VOP_SPARE4_AP(struct vop_spare4_args *);
int VOP_SPARE4_APV(const struct vop_vector *vop, struct vop_spare4_args *);

static __inline int VOP_SPARE4(
	struct vnode *vp)
{
	struct vop_spare4_args a;

	a.a_gen.a_desc = &vop_spare4_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_spare4(&a));
	else
		return (VOP_SPARE4_APV(vp->v_op, &a));
#else
	return (VOP_SPARE4_APV(vp->v_op, &a));
#endif
}

struct vop_spare5_args {
	struct vop_generic_args a_gen;
	struct vnode *a_vp;
};

extern struct vnodeop_desc vop_spare5_desc;

int VOP_SPARE5_AP(struct vop_spare5_args *);
int VOP_SPARE5_APV(const struct vop_vector *vop, struct vop_spare5_args *);

static __inline int VOP_SPARE5(
	struct vnode *vp)
{
	struct vop_spare5_args a;

	a.a_gen.a_desc = &vop_spare5_desc;
	a.a_vp = vp;

#if !defined(INVARIANTS) && !defined(KTR)
	if (!SDT_PROBES_ENABLED())
		return (vp->v_op->vop_spare5(&a));
	else
		return (VOP_SPARE5_APV(vp->v_op, &a));
#else
	return (VOP_SPARE5_APV(vp->v_op, &a));
#endif
}

void vfs_vector_op_register(struct vop_vector *orig_vop);
