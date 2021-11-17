#include <debug.h>
#include <iostream>

/** @file
 *
 * This file handles test for runtime debug support
 */

void func2()
{
    FUNCTION_INFO;
    OBTAIN_STACK_FRAME;
}

void func1()
{
    FUNCTION_INFO;
    OBTAIN_STACK_FRAME;

    func2();
}

int main(int argc, char ** argv)
{
    __path_to_executable = *argv;
    __check_addr2line();

    FUNCTION_INFO;
    OBTAIN_STACK_FRAME;

    func1();
}
