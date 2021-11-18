#include <inode.h>
#include <stmpfs_error.h>
#include <debug.h>
#include <cerrno>
#include <fuse_ops.h>

/** @file
 *
 * This file implements inode and relevant operations
 */


void inode_t::add_child(const std::string &child_name)
{
    auto it = children.find(child_name);
    if (it != children.end())
    {
        FUNCTION_INFO;
        OBTAIN_STACK_FRAME;

        errno = EEXIST; // File exists (POSIX.1-2001).
        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }

    children.emplace(child_name, inode_t());
}

inode_t & inode_t::get_inode_by_name(const std::string& child_name)
{
    auto it = children.find(child_name);

    if (it == children.end())
    {
        FUNCTION_INFO;
        OBTAIN_STACK_FRAME;

        errno = ENOENT; // No such file or directory (POSIX.1-2001).
        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }

    return it->second;
}

void inode_t::delete_child(const std::string & child_name)
{
    auto it = children.find(child_name);
    if (it == children.end())
    {
        FUNCTION_INFO;
        OBTAIN_STACK_FRAME;

        errno = ENOENT; // No such file or directory (POSIX.1-2001).
        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }

    children.erase(child_name);
}

inode_t &pathname_to_inode(stmpfs_pathname_t &pathname, inode_t &root)
{
    inode_t * cur_dir = &root;

    for (const auto& path : pathname)
    {
        cur_dir = &cur_dir->get_inode_by_name(path);
    }

    return *cur_dir;
}
