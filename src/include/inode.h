#ifndef STMPFS_REWRITE_INODE_H
#define STMPFS_REWRITE_INODE_H

#include <buffer.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>

class inode_t
{
private:

    buffer_t buffer;

    std::map < uint32_t, std::vector < inode_t * > > children;

    std::string name;

    void create_snapshot_for_children(uint32_t version);
    void delete_snapshot_for_children(uint32_t version);

public:

    /// init children if inode is a dir
    void init_as_dir()
        { children.emplace(0, std::vector < inode_t * >()); }

    struct stat fs_stat { }; // file/dir stat, publicly changeable

    void snapshot(uint32_t version);

    void delete_snapshot(uint32_t version);

    inode_t * get_inode_by_name(const std::string& _name, uint32_t version = 0);

    uint32_t snapshout_link_count = 1;

    void new_child(const std::string& _name);

    void delete_child(const std::string& _name);

    void replicate_snapshot_0(const std::string& _name);

    /// resize the buffer
    /// @param new_size new size of buffer
    /// @return none
    void resize(size_t new_size) { buffer.resize(new_size); }

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
