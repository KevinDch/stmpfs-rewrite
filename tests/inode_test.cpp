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
        root.fs_stat.st_mode = S_IFDIR;
        root.init_as_dir();

        root.new_child("file1");
        root.new_child("file2");

        auto file2 = root.get_inode_by_name("file2");
        file2->write("123", 3, 0);

        root.new_child("dir1");
        auto dir1 = root.get_inode_by_name("dir1");
        dir1->fs_stat.st_mode = S_IFDIR;
        dir1->init_as_dir();
        dir1->new_child("file3");

        root.snapshot(1);
        root.delete_child("file2");

        root.replicate_snapshot_0("file1");
        root.get_inode_by_name("file1")->write("345", 3, 0);

        auto inode = root.get_inode_by_name("file2", 1);
        std::cout << inode->get_sha256sum() << std::endl;

        root.delete_snapshot(1);
    }
    catch (std::exception & err)
    {
        std::cout << err.what() << std::endl;
        return 0;
    }
}
