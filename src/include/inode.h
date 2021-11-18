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

class inode_t
{
private:

    buffer_t buffer;
    std::map < std::string, inode_t > children;

public:

    /// file/dir stat, publicly changeable
    struct stat fs_stat { };

    /// resize the buffer
    /// @param new_size new size of buffer
    /// @return none
    void resize(size_t new_size)
        {   buffer.resize(new_size);
            fs_stat.st_size = (off64_t)new_size;
        }

    /// read from buffer
    /// @param buffer output buffer
    /// @param length read length
    /// @param offset read offset
    /// @return the actual read length
    size_t read(char * _buffer, size_t length, off_t offset)
        { return buffer.read(_buffer, length, offset); }

    /// write to buffer
    /// @param buffer input buffer
    /// @param length write length
    /// @param offset write offset
    /// @return the actual write length
    size_t write(const char * _buffer, size_t length, off_t offset)
        { return buffer.write(_buffer, length, offset); }

    /// get current data size
    /// @param none
    /// @return size of current data bank
    size_t size() { return buffer.size(); }

    /// get sha256sum of current data bank
    /// @param none
    /// @return sha256sum of current data bank
    std::string get_sha256sum() { return buffer.get_sha256sum(); }

    inode_t();

    ~inode_t();
};


#endif //STMPFS_REWRITE_INODE_H
