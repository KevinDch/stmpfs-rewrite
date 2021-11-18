#ifndef STMPFS_REWRITE_PATHNAME_T_H
#define STMPFS_REWRITE_PATHNAME_T_H

/** @file
 *
 * This file defines pathname resolver
 */

#include <string>
#include <vector>

typedef std::vector < std::string > pathname_t;

/// STMPFS pathname Entry
class stmpfs_pathname_t
{
private:
    pathname_t pathname;

public:
    /// create an entry link
    /// @param pathname entry
    explicit stmpfs_pathname_t(std::string pathname);

    /* support for C++11 range loop */
    typedef pathname_t::iterator iterator;
    [[nodiscard]] iterator begin()  { return pathname.begin(); }
    [[nodiscard]] iterator end()    { return pathname.end(); }

    /// return last element and pop_back
    std::string target_name();

    size_t size() { return pathname.size(); }
};

#endif //STMPFS_REWRITE_PATHNAME_T_H
