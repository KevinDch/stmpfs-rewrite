#include <buffer.h>
#include <sha256sum.h>

/**
 * @file this file defines a simple buffer **/

void buffer_t::resize(size_t new_size)
{
    if (new_size > data.size())
    {
        size_t append = new_size - data.size();
        auto it = data.end();
        data.insert(it, append, 0);
    }
    else if (new_size < data.size())
    {
        size_t ndelete = data.size() - new_size;

        if (ndelete > 1)
        {
            auto it = data.begin();
            data.erase(it, it + (long)ndelete);
        }
        else
        {
            data.pop_back();
        }
    }
}

size_t buffer_t::read(char *buffer, size_t length, off_t offset)
{
    // offset is beyond data bank, return 0
    if (offset > data.size())
    {
        return 0;
    }

    // size is too big, shrink length
    if (offset + length > data.size())
    {
        length = data.size() - offset;
    }

    // read data
    for (u_int64_t i = 0; i < length; i++)
    {
        buffer[i] = data[offset + i];
    }

    // return read length
    return length;
}

size_t buffer_t::write(const char *buffer, size_t length, off_t offset)
{
    // if data bank is too small, resize it
    if (size() < offset + length)
    {
        resize(offset + length);
    }

    // write data
    for (u_int64_t i = 0; i < length; i++)
    {
        data[offset + i] = buffer[i];
    }

    // return write length
    return length;
}

std::string buffer_t::to_string()
{
    std::string ret;
    for (auto i : data)
    {
        ret += i;
    }

    return ret;
}

std::string buffer_t::get_sha256sum()
{
    return sha256(to_string());
}
