#include <inode.h>
#include <iostream>
#include <debug.h>

/** @file
 *
 * This file defines test for inode_t
 */

int main(int argc, char ** argv)
{
    try
    {
        __path_to_executable = *argv;
        __check_addr2line();

        inode_t root;
        root.resize(4);
        root.write("123\n", 4, 0);
        std::cout << root.get_sha256sum() << std::endl;

        return root.get_sha256sum() != "181210f8f9c779c26da1d9b2075bde0127302ee0e3fca38c9a83f5b1dd8e5d3b";
    }
    catch (std::exception & err)
    {
        std::cout << err.what() << std::endl;
        return 0;
    }
}
