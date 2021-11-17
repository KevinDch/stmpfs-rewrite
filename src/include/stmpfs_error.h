#ifndef STMPFS_REWRITE_ERROR_H
#define STMPFS_REWRITE_ERROR_H

/** @file
 *
 * This file defines runtime error class and relevant operations
 */

#include <string>
#include <utility>
#include <cstring>

#define STMPFS_EXT_LIB_ERR      0xA0000001      /* External library error */

/// Filesystem Error Type
class stmpfs_error_t : public std::exception
{
private:
    uint32_t error_code;
    error_t _errno;

public:
    /// Generate a error with error code
    /** @param _code Your error code **/
    explicit stmpfs_error_t(unsigned int _code) noexcept : error_code(_code), _errno(errno) {}

    /// Return explanation of current error
    [[nodiscard]] const char * what() const noexcept override;

    /// Return the explanation of errno snapshoted when the current error is generated
    [[nodiscard]] const char * what_errno() const noexcept { return strerror(_errno); };

    /// Return the errno snapshoted when the current error is generated
    [[nodiscard]] error_t my_errno() const noexcept { return _errno; }

    /// Return the error code of current error
    [[nodiscard]] unsigned int my_errcode() const noexcept { return error_code; }
};

#endif //STMPFS_REWRITE_ERROR_H
