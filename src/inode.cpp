#include <inode.h>
#include <stmpfs_error.h>
#include <debug.h>
#include <cerrno>

/** @file
 *
 * This file implements inode and relevant operations
 */

inode_t::~inode_t() = default;
inode_t::inode_t() = default;
