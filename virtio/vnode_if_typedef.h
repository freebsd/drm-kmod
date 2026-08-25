/*
 * This file is @generated automatically.
 * Do not modify anything in here by hand.
 */


struct vop_islocked_args;
typedef int vop_islocked_t(struct vop_islocked_args *);

struct vop_lookup_args;
typedef int vop_lookup_t(struct vop_lookup_args *);

struct vop_cachedlookup_args;
typedef int vop_cachedlookup_t(struct vop_cachedlookup_args *);

struct vop_create_args;
typedef int vop_create_t(struct vop_create_args *);

struct vop_whiteout_args;
typedef int vop_whiteout_t(struct vop_whiteout_args *);

struct vop_mknod_args;
typedef int vop_mknod_t(struct vop_mknod_args *);

struct vop_open_args;
typedef int vop_open_t(struct vop_open_args *);

struct vop_close_args;
typedef int vop_close_t(struct vop_close_args *);

struct vop_fplookup_vexec_args;
typedef int vop_fplookup_vexec_t(struct vop_fplookup_vexec_args *);

struct vop_fplookup_symlink_args;
typedef int vop_fplookup_symlink_t(struct vop_fplookup_symlink_args *);

struct vop_access_args;
typedef int vop_access_t(struct vop_access_args *);

struct vop_accessx_args;
typedef int vop_accessx_t(struct vop_accessx_args *);

struct vop_stat_args;
typedef int vop_stat_t(struct vop_stat_args *);

struct vop_getattr_args;
typedef int vop_getattr_t(struct vop_getattr_args *);

struct vop_setattr_args;
typedef int vop_setattr_t(struct vop_setattr_args *);

struct vop_mmapped_args;
typedef int vop_mmapped_t(struct vop_mmapped_args *);

struct vop_read_args;
typedef int vop_read_t(struct vop_read_args *);

struct vop_read_pgcache_args;
typedef int vop_read_pgcache_t(struct vop_read_pgcache_args *);

struct vop_write_args;
typedef int vop_write_t(struct vop_write_args *);

struct vop_ioctl_args;
typedef int vop_ioctl_t(struct vop_ioctl_args *);

struct vop_poll_args;
typedef int vop_poll_t(struct vop_poll_args *);

struct vop_kqfilter_args;
typedef int vop_kqfilter_t(struct vop_kqfilter_args *);

struct vop_revoke_args;
typedef int vop_revoke_t(struct vop_revoke_args *);

struct vop_fsync_args;
typedef int vop_fsync_t(struct vop_fsync_args *);

struct vop_remove_args;
typedef int vop_remove_t(struct vop_remove_args *);

struct vop_link_args;
typedef int vop_link_t(struct vop_link_args *);

struct vop_rename_args;
typedef int vop_rename_t(struct vop_rename_args *);

struct vop_mkdir_args;
typedef int vop_mkdir_t(struct vop_mkdir_args *);

struct vop_rmdir_args;
typedef int vop_rmdir_t(struct vop_rmdir_args *);

struct vop_symlink_args;
typedef int vop_symlink_t(struct vop_symlink_args *);

struct vop_readdir_args;
typedef int vop_readdir_t(struct vop_readdir_args *);

struct vop_readlink_args;
typedef int vop_readlink_t(struct vop_readlink_args *);

struct vop_inactive_args;
typedef int vop_inactive_t(struct vop_inactive_args *);

struct vop_need_inactive_args;
typedef int vop_need_inactive_t(struct vop_need_inactive_args *);

struct vop_reclaim_args;
typedef int vop_reclaim_t(struct vop_reclaim_args *);

struct vop_lock1_args;
typedef int vop_lock1_t(struct vop_lock1_args *);

struct vop_unlock_args;
typedef int vop_unlock_t(struct vop_unlock_args *);

struct vop_bmap_args;
typedef int vop_bmap_t(struct vop_bmap_args *);

struct vop_strategy_args;
typedef int vop_strategy_t(struct vop_strategy_args *);

struct vop_getwritemount_args;
typedef int vop_getwritemount_t(struct vop_getwritemount_args *);

struct vop_getlowvnode_args;
typedef int vop_getlowvnode_t(struct vop_getlowvnode_args *);

struct vop_print_args;
typedef int vop_print_t(struct vop_print_args *);

struct vop_pathconf_args;
typedef int vop_pathconf_t(struct vop_pathconf_args *);

struct vop_advlock_args;
typedef int vop_advlock_t(struct vop_advlock_args *);

struct vop_advlockasync_args;
typedef int vop_advlockasync_t(struct vop_advlockasync_args *);

struct vop_advlockpurge_args;
typedef int vop_advlockpurge_t(struct vop_advlockpurge_args *);

struct vop_reallocblks_args;
typedef int vop_reallocblks_t(struct vop_reallocblks_args *);

struct vop_getpages_args;
typedef int vop_getpages_t(struct vop_getpages_args *);

struct vop_getpages_async_args;
typedef int vop_getpages_async_t(struct vop_getpages_async_args *);

struct vop_putpages_args;
typedef int vop_putpages_t(struct vop_putpages_args *);

struct vop_getacl_args;
typedef int vop_getacl_t(struct vop_getacl_args *);

struct vop_setacl_args;
typedef int vop_setacl_t(struct vop_setacl_args *);

struct vop_aclcheck_args;
typedef int vop_aclcheck_t(struct vop_aclcheck_args *);

struct vop_closeextattr_args;
typedef int vop_closeextattr_t(struct vop_closeextattr_args *);

struct vop_getextattr_args;
typedef int vop_getextattr_t(struct vop_getextattr_args *);

struct vop_listextattr_args;
typedef int vop_listextattr_t(struct vop_listextattr_args *);

struct vop_openextattr_args;
typedef int vop_openextattr_t(struct vop_openextattr_args *);

struct vop_deleteextattr_args;
typedef int vop_deleteextattr_t(struct vop_deleteextattr_args *);

struct vop_setextattr_args;
typedef int vop_setextattr_t(struct vop_setextattr_args *);

struct vop_setlabel_args;
typedef int vop_setlabel_t(struct vop_setlabel_args *);

struct vop_vptofh_args;
typedef int vop_vptofh_t(struct vop_vptofh_args *);

struct vop_vptocnp_args;
typedef int vop_vptocnp_t(struct vop_vptocnp_args *);

struct vop_allocate_args;
typedef int vop_allocate_t(struct vop_allocate_args *);

struct vop_advise_args;
typedef int vop_advise_t(struct vop_advise_args *);

struct vop_unp_bind_args;
typedef int vop_unp_bind_t(struct vop_unp_bind_args *);

struct vop_unp_connect_args;
typedef int vop_unp_connect_t(struct vop_unp_connect_args *);

struct vop_unp_detach_args;
typedef int vop_unp_detach_t(struct vop_unp_detach_args *);

struct vop_is_text_args;
typedef int vop_is_text_t(struct vop_is_text_args *);

struct vop_set_text_args;
typedef int vop_set_text_t(struct vop_set_text_args *);

struct vop_unset_text_args;
typedef int vop_unset_text_t(struct vop_unset_text_args *);

struct vop_add_writecount_args;
typedef int vop_add_writecount_t(struct vop_add_writecount_args *);

struct vop_fdatasync_args;
typedef int vop_fdatasync_t(struct vop_fdatasync_args *);

struct vop_copy_file_range_args;
typedef int vop_copy_file_range_t(struct vop_copy_file_range_args *);

struct vop_vput_pair_args;
typedef int vop_vput_pair_t(struct vop_vput_pair_args *);

struct vop_deallocate_args;
typedef int vop_deallocate_t(struct vop_deallocate_args *);

struct vop_inotify_args;
typedef int vop_inotify_t(struct vop_inotify_args *);

struct vop_inotify_add_watch_args;
typedef int vop_inotify_add_watch_t(struct vop_inotify_add_watch_args *);

struct vop_delayed_setsize_args;
typedef int vop_delayed_setsize_t(struct vop_delayed_setsize_args *);

struct vop_spare1_args;
typedef int vop_spare1_t(struct vop_spare1_args *);

struct vop_spare2_args;
typedef int vop_spare2_t(struct vop_spare2_args *);

struct vop_spare3_args;
typedef int vop_spare3_t(struct vop_spare3_args *);

struct vop_spare4_args;
typedef int vop_spare4_t(struct vop_spare4_args *);

struct vop_spare5_args;
typedef int vop_spare5_t(struct vop_spare5_args *);

