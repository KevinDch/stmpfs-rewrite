#ifndef STMPFS_REWRITE_INODE_H
#define STMPFS_REWRITE_INODE_H

/** @file
 *
 * This file defines inode and relevant operations
 */

#include <buffer.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <pathname_t.h>
#include <stmpfs_error.h>

class inode_t;

/// get inode by name
inode_t & pathname_to_inode(stmpfs_pathname_t & pathname, inode_t & root);

class inode_t
{
private:

    buffer_t buffer;
    std::map < std::string, inode_t > children;

public:

    inode_t() noexcept = default;

    [[nodiscard]] const std::map < std::string, inode_t > & get_children()
        { return children; }

    /// file/dir stat, publicly changeable
    struct stat fs_stat { };

    /// extended attributes
    std::map < std::string, std::string > xattr;

    /// add child in children. ERROR will be thrown if child_name already
    /// exists in children
    /// @param child_name child name
    /// @return none
    void add_child(const std::string & child_name);

    /// delete child in children. ERROR will be thrown if child_name doesn't
    /// exist in children
    /// @param child_name child name
    /// @return none
    void delete_child(const std::string & child_name);

    /// find a child in children. ERROR will be thrown if child_name doesn't
    /// exist in children
    /// @param child_name child name
    /// @return reference to inode
    inode_t & get_inode_by_name(const std::string& child_name);

    /// resize the buffer
    /// @param new_size new size of buffer
    /// @return none
    void truncate(size_t new_size)
        {   buffer.resize(new_size);
            fs_stat.st_size = (off64_t)new_size;
        }

    /// read from buffer
    /// @param buffer output buffer
    /// @param length read length
    /// @param offset read offset
    /// @return the actual read length
    off64_t read(char * _buffer, size_t length, off_t offset)
        { return (off64_t)buffer.read(_buffer, length, offset); }

    /// write to buffer
    /// @param buffer input buffer
    /// @param length write length
    /// @param offset write offset
    /// @return the actual write length
    off64_t write(const char * _buffer, size_t length, off_t offset)
        {   auto new_size = (off64_t)buffer.write(_buffer, length, offset);
            fs_stat.st_size = size();
            return new_size;
        }

    /// get current data size
    /// @param none
    /// @return size of current data bank
    off64_t size() { return (off64_t)buffer.size(); }

    off64_t child_size() { return (off64_t)children.size(); }

    /// get sha256sum of current data bank
    /// @param none
    /// @return sha256sum of current data bank
    std::string get_sha256sum() { return buffer.get_sha256sum(); }

    friend inode_t & pathname_to_inode(stmpfs_pathname_t & pathname, inode_t & root);
};


#endif //STMPFS_REWRITE_INODE_H
