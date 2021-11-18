#ifndef STMPFS_REWRITE_FUSE_OPS_H
#define STMPFS_REWRITE_FUSE_OPS_H

/** @file
 *
 * This file defines the corresponding methods for FUSE APIs
 */

#define FUSE_USE_VERSION 31
#include <inode.h>
#include <fuse.h>
#include <sys/types.h>
#include <inode.h>
#include <fuse_ops.h>
#include <execinfo.h>
#include <sys/xattr.h>
#include <sys/sysinfo.h>

extern inode_t filesystem_root;

int do_getattr  (const char * path, struct stat *stbuf);
int do_readlink (const char * path, char *, size_t);
int do_mknod    (const char * path, mode_t mode, dev_t device);
int do_mkdir    (const char * path, mode_t mode);
int do_unlink   (const char * path);
int do_rmdir    (const char * path);
int do_symlink  (const char * path, const char *);
int do_chmod    (const char * path, mode_t mode);
int do_chown    (const char * path, uid_t uid, gid_t gid);
int do_truncate (const char * path, off_t size);
int do_open     (const char * path, struct fuse_file_info * fi);
int do_read     (const char * path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi);
int do_write    (const char * path, const char * buffer, size_t size, off_t offset, struct fuse_file_info * fi);
int do_statfs   (const char * path, struct statvfs *);
int do_flush    (const char * path, struct fuse_file_info * fi);
int do_release  (const char * path, struct fuse_file_info * fi);
int do_fsync    (const char * path, int, struct fuse_file_info *);
int do_setxattr (const char * path, const char *, const char *, size_t, int);
int do_getxattr (const char * path, const char *, char *, size_t);
int do_listxattr (const char *path, char *, size_t);
int do_removexattr (const char *path, const char *);
int do_readdir  (const char * path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
int do_releasedir (const char *path, struct fuse_file_info *);
int do_fsyncdir (const char * path, int, struct fuse_file_info *);
int do_create   (const char * path, mode_t mode, struct fuse_file_info * fi);
int do_utimens  (const char * path, const struct timespec tv[2]);
int do_fallocate(const char * path, int mode, off_t offset, off_t length, struct fuse_file_info * fi);

#endif //STMPFS_REWRITE_FUSE_OPS_H
