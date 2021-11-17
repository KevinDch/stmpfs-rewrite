#ifndef STMPFS_REWRITE_BUFFER_H
#define STMPFS_REWRITE_BUFFER_H

/**
 * @file this file defines a simple buffer with sha256sum check ability **/

#include <vector>
#include <string>

/// a simple buffer with sha256sum check ability
class buffer_t
{
private:
    std::vector < char > data;

public:

    /// resize the buffer
    /// @param new_size new size of buffer
    /// @return none
    void resize(size_t new_size);

    /// emplace a pointer
    /// @param _pointer pointer
    /// @return none
    template < class Type >
    void emplace_pointer(Type * _pointer);

    /// get a pointer
    /// @param _pointer pointer buffer
    /// @return none
    template < class Type >
    void get_pointer(Type * & _pointer);

    /// read from buffer
    /// @param buffer output buffer
    /// @param length read length
    /// @param offset read offset
    /// @return the actual read length
    size_t read(char * buffer, size_t length, off_t offset);

    /// write to buffer
    /// @param buffer input buffer
    /// @param length write length
    /// @param offset write offset
    /// @return the actual write length
    size_t write(const char * buffer, size_t length, off_t offset);

    /// get current data size
    /// @param none
    /// @return size of current data bank
    size_t size() { return data.size(); }

    /// get sha256sum of current data bank
    /// @param none
    /// @return sha256sum of current data bank
    std::string get_sha256sum();

    /// convert current data bank to std::string
    /// @param none
    /// @return std::string of current data bank
    std::string to_string();
};

template<class Type>
void buffer_t::emplace_pointer(Type *_pointer)
{
    write((char*)&_pointer, sizeof(Type*), 0);
}

template<class Type>
void buffer_t::get_pointer(Type *&_pointer)
{
    read((char*)&_pointer, sizeof(Type*), 0);
}

#endif //STMPFS_REWRITE_BUFFER_H
