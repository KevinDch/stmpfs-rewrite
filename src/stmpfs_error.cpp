/** @file
 *
 * This file implements error class and relevant operations
 */

#include <stmpfs_error.h>

const char *stmpfs_error_t::what() const noexcept
{
    switch (error_code)
    {
        case STMPFS_EXT_LIB_ERR:
            return STMPFS_PREFIX "External library error";

        default:
            return STMPFS_PREFIX "Unknown";
    }
}
