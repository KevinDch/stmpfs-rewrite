#ifndef STMPFS_REWRITE_DEBUG_H
#define STMPFS_REWRITE_DEBUG_H

/** @file
 *
 * This file defines operations for runtime debug support
 */

#ifdef CMAKE_BUILD_DEBUG

#include <iostream>
#include <execinfo.h>
#include <sys/xattr.h>
#include <sys/sysinfo.h>
#include <string>

/// current executable pathname, filled by main()
/// if not filled, addr2line in OBTAIN_STACK_FRAME will not be available
extern const char * __path_to_executable;

/// check if addr2line command is available, filled by __check_addr2line()
extern bool __is_addr2line_available;

/// check addr2line availability
/// you have to run this before you can use addr2line in OBTAIN_STACK_FRAME
void __check_addr2line();

/// execute a command and get its output
/// @param cmd command for SHELL
/// @return SHELL output
std::string __exec_cmd(const std::string& cmd);

/// get address in stack frame output
/// @param input raw stack frame input
/// @return address of stack frame
std::string __get_addr(const std::string& input);

/// clean addr2line output, remove "?? ??:0"
/// @param input addr2line raw output
/// @return useful information provided by addr2line, or "(no specific information)" if not
std::string __clean_addr2line_output(const std::string& input);


# define BACKTRACE_SZ 64    /* Max stack frame size */
/// Automatically obtain stack frame
# define OBTAIN_STACK_FRAME                             \
{                                                       \
    void *__array[BACKTRACE_SZ];                        \
    int __size, __i;                                    \
    char **__strings;                                   \
    __size = backtrace (__array, BACKTRACE_SZ);         \
    __strings = backtrace_symbols (__array, __size);    \
                                                        \
    if (__strings != nullptr)                           \
    {                                                   \
        std::cerr   << std::endl                        \
                    << "Obtained stack frame(s):"       \
                    << std::endl;                       \
        for (__i = 0; __i < __size; __i++)              \
        {                                               \
            std::cerr << "\t" << __strings[__i];        \
            if (__path_to_executable && __is_addr2line_available)                       \
            {                                                                           \
                std::cerr << " " << __clean_addr2line_output(                           \
                                    __exec_cmd(std::string("addr2line a -f -p -e ")     \
                                        + __path_to_executable + " "                    \
                                        + __get_addr(__strings[__i])                    \
                                    )                   \
                                );                      \
            }                                           \
                                                        \
            std::cerr << "\n";                          \
        }                                               \
        free (__strings);                               \
    }                                                   \
} __asm__("nop") /* suppress IDE "empty statement" warning */


# define FUNCTION_INFO                                  \
{                                                       \
    std::cerr << "\nFrom " << __FILE__ << ":"           \
              << __LINE__ << ": "                       \
              << __FUNCTION__ << ":";                   \
} __asm__("nop") /* suppress IDE "empty statement" warning */


#else // CMAKE_BUILD_DEBUG

# define OBTAIN_STACK_FRAME __asm__("nop") /* suppress IDE "empty statement" warning */
# define FUNCTION_INFO      __asm__("nop") /* suppress IDE "empty statement" warning */

#endif // CMAKE_BUILD_DEBUG


#endif //STMPFS_REWRITE_DEBUG_H
