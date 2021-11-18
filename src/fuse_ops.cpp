#include <fuse_ops.h>
#include <debug.h>
#include <pathname_t.h>
#include <cstring>

#define OUTPUT_PATHNAME std::cerr << "(PATHNAME=" << path << ")" << std::endl

/** @file
 *
 * This file defines the implements methods for FUSE APIs
 */

/// filesystem root
inode_t filesystem_root;

int do_getattr (const char *path, struct stat *stbuf)
{
    try
    {
        FUNCTION_INFO;
        OUTPUT_PATHNAME;

        stmpfs_pathname_t vpath(path);
        auto & inode = pathname_to_inode(vpath, filesystem_root);
        *stbuf = inode.fs_stat;

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_readdir (const char *path,
                void *buffer,
                fuse_fill_dir_t filler,
                off_t,
                struct fuse_file_info *)
{
    try
    {
        FUNCTION_INFO;
        OUTPUT_PATHNAME;

        stmpfs_pathname_t vpath(path);

        filler(buffer, ".", nullptr, 0);  // Current Directory
        filler(buffer, "..", nullptr, 0); // Parent Directory

        // normal read
        auto &inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_atim = current_time();

        for (auto & i: inode.get_children())
        {
            filler(buffer, i.first.c_str(), nullptr, 0);
        }

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_open (const char * path, struct fuse_file_info *)
{
    try
    {
        FUNCTION_INFO;
        OUTPUT_PATHNAME;

        stmpfs_pathname_t vpath(path);

        inode_t & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_atim = current_time();

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_mkdir (const char * path, mode_t mode)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        // get target name
        std::string tag_name = vpath.target_name();

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.add_child(tag_name);

        inode_t & new_inode = inode.get_inode_by_name(tag_name);
        auto cur_time = current_time();
        new_inode.fs_stat.st_mode = mode | S_IFDIR;
        new_inode.fs_stat.st_atim = cur_time;
        new_inode.fs_stat.st_ctim = cur_time;
        new_inode.fs_stat.st_mtim = cur_time;

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_chmod (const char * path, mode_t mode)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto &inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_mode = mode;

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_chown (const char * path, uid_t uid, gid_t gid)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_uid = uid;
        inode.fs_stat.st_gid = gid;

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_create (const char * path, mode_t mode, struct fuse_file_info *)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        // get target name
        std::string tag_name = vpath.target_name();

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.add_child(tag_name);

        inode_t & new_inode = inode.get_inode_by_name(tag_name);

        // fill info
        auto cur_time = current_time();
        new_inode.fs_stat.st_mode = mode;
        new_inode.fs_stat.st_nlink = 1;
        new_inode.fs_stat.st_atim = cur_time;
        new_inode.fs_stat.st_ctim = cur_time;
        new_inode.fs_stat.st_mtim = cur_time;

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_flush (const char * path, struct fuse_file_info *)
{
    FUNCTION_INFO;
    return 0;
}

int do_release (const char * path, struct fuse_file_info *)
{
    FUNCTION_INFO;
    return 0;
}


int do_read (const char *path,
             char *buffer,
             size_t size,
             off_t offset,
             struct fuse_file_info *)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_atim = current_time();
        return (int)inode.read(buffer, size, offset);
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_write (const char * path, const char * buffer, size_t size, off_t offset,
              struct fuse_file_info *)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_ctim = current_time();
        return (int)inode.write(buffer, size, offset);
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_utimens (const char * path, const struct timespec tv[2])
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_atim = tv[0];
        inode.fs_stat.st_mtim = tv[1];

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_unlink (const char * path)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        // attempt to delete root
        if (vpath.size() == 0)
        {
            return -EISDIR; // Is a directory (POSIX.1-2001).
        }

        std::string tag_name = vpath.target_name();

        auto & inode = pathname_to_inode(vpath, filesystem_root);
//        auto * target_inode = inode.find_in_dentry(tag_name);

        inode.delete_child(tag_name);
//        if (target_inode->fs_stat.st_nlink == 1)
//        {
//            inode.del_dentry(tag_name);
//        }
//        else
//        {
//            target_inode->fs_stat.st_nlink -= 1;
//        }

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_rmdir (const char * path)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        /// attempt to delete root
        if (vpath.size() == 0)
        {
            return -EBUSY;  // Device or resource busy (POSIX.1-2001).
        }

        std::string tag_name = vpath.target_name();

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        auto * target_inode = &inode.get_inode_by_name(tag_name);

        if (!(target_inode->fs_stat.st_mode & S_IFDIR))
        {
            return -ENOTDIR; // Not a directory (POSIX.1-2001).
        }

        if (target_inode->child_size())
        {
            return -ENOTEMPTY; // Directory not empty (POSIX.1-2001).
        }

        // remove directory
        inode.delete_child(tag_name);

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_mknod (const char * path, mode_t mode, dev_t device)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        std::string tag_name = vpath.target_name();

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.add_child(tag_name);

        inode_t & new_inode = inode.get_inode_by_name(tag_name);

        // fill up info
        auto cur_time = current_time();
        new_inode.fs_stat.st_mode = mode;
        new_inode.fs_stat.st_nlink = 1;
        new_inode.fs_stat.st_atim = cur_time;
        new_inode.fs_stat.st_ctim = cur_time;
        new_inode.fs_stat.st_mtim = cur_time;
        new_inode.fs_stat.st_dev = device;

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_symlink (const char * path, const char * linkname)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        // get target name
        std::string tag_name = vpath.target_name();

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.add_child(tag_name);
        inode_t & new_inode = inode.get_inode_by_name(tag_name);

        // fill up info
        auto cur_time = current_time();
        new_inode.fs_stat.st_mode = S_IFLNK | 0755;
        new_inode.fs_stat.st_nlink = 1;
        new_inode.fs_stat.st_atim = cur_time;
        new_inode.fs_stat.st_ctim = cur_time;
        new_inode.fs_stat.st_mtim = cur_time;
        new_inode.write(linkname, strlen(linkname), 0);

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_readlink (const char * path, char * buffer, size_t size)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);
        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_atim = current_time();
        inode.read(buffer, size, 0);

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_truncate (const char * path, off_t size)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        inode.fs_stat.st_size = size;
        inode.truncate(size);

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_fallocate(const char * path, int mode, off_t offset, off_t length, struct fuse_file_info *)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);

        // fill up info
        auto cur_time = current_time();
        inode.fs_stat.st_mode = mode | S_IFREG;
        inode.fs_stat.st_nlink = 1;
        inode.fs_stat.st_ctim = cur_time;
        inode.fs_stat.st_size = offset + length;
        inode.truncate(offset + length);

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_fsync (const char * path, int, struct fuse_file_info *)
{
    FUNCTION_INFO;
    return 0;
}

int do_releasedir (const char * path, struct fuse_file_info *)
{
    FUNCTION_INFO;
    return 0;
}

int do_fsyncdir (const char * path, int, struct fuse_file_info *)
{
    FUNCTION_INFO;
    return 0;
}

void inode_setxattr(inode_t & inode, const std::string& name, const char * value, size_t size)
{
    std::string buff;
    for (uint64_t i = 0; i < size; i++)
    {
        buff += value[i];
    }

    inode.xattr[name] = buff;
}

int do_setxattr (const char * path, const char * name, const char * value, size_t size, int flag)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        if (flag == XATTR_CREATE)
        {
            if (inode.xattr.find(name) != inode.xattr.end())
            {
                return -EEXIST;
            }

            inode_setxattr(inode, name, value, size);
            return 0;
        }
        else if (flag == XATTR_REPLACE)
        {
            if (inode.xattr.find(name) == inode.xattr.end())
            {
                return -ENODATA;
            }

            inode_setxattr(inode, name, value, size);
            return 0;
        }
        else
        {
            inode_setxattr(inode, name, value, size);
        }

        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_getxattr (const char * path, const char * name, char * value, size_t size)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        auto it = inode.xattr.find(name);
        if (it == inode.xattr.end())
        {
            return -ENODATA;
        }

        if (size == 0 && value == nullptr)
        {
            return (int)it->second.size();
        }

        if (size < it->second.size())
        {
            return -ERANGE;
        }

        for (uint64_t i = 0; i < it->second.size(); i++)
        {
            value[i] = it->second.at(i);
        }

        return (int)it->second.size();
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/// copy str to list buffer
uint64_t copy_to_list(char * list, uint64_t copy_len, const std::string& str)
{
    uint64_t actual_len = MIN(copy_len, str.size());
    for (uint64_t i = 0; i < actual_len; i++)
    {
        list[i] = str.at(i);
    }

    list[actual_len] = 0;
    return actual_len + 1;
}

int do_listxattr (const char * path, char * list, size_t list_size)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        uint64_t list_actual_size = 0, write_off = 0;
        auto xattr_itr = inode.xattr.begin();
        for (auto & i : inode.xattr)
        {
            list_actual_size += i.first.size() + 1;
        }

        if (list_size == 0 && list == nullptr)
        {
            return (int)list_actual_size;
        }

        if (list_size < list_actual_size)
        {
            return -ERANGE;
        }

        while (write_off < list_actual_size)
        {
            write_off += copy_to_list(list + write_off,
                                      list_actual_size - write_off,
                                      xattr_itr->first);
            xattr_itr++;
        }

        return (int)list_actual_size;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_removexattr (const char * path, const char * name)
{
    try
    {
        FUNCTION_INFO;

        stmpfs_pathname_t vpath(path);

        auto & inode = pathname_to_inode(vpath, filesystem_root);
        auto it = inode.xattr.find(name);
        if (it == inode.xattr.end())
        {
            return -ENODATA;
        }

        inode.xattr.erase(it);
        return 0;
    }
    catch (std::exception & error)
    {
        OBTAIN_STACK_FRAME;
        std::cerr << error.what() << " (errno=" << strerror(errno) << ")" << std::endl;
        return -errno;
    }
}

int do_statfs (const char * path, struct statvfs * statvfs)
{
    struct sysinfo _sysinfo{};
    sysinfo(&_sysinfo);

    struct statvfs _statvfs
            {
                    .f_bsize = 4096,
                    .f_frsize = 4096,
                    .f_blocks = _sysinfo.totalram / 4096,
                    // I don't really fucking know what is what, for some reason this works
                    .f_bfree = (_sysinfo.totalram - _sysinfo.freeram) / 4096,
                    .f_bavail = (_sysinfo.totalram - _sysinfo.freeram) / 4096,
                    .f_files = 4096,
                    .f_ffree = 4096,
                    .f_fsid = 1,
                    .f_namemax = 128,
            };

    *statvfs = _statvfs;

    return 0;
}
