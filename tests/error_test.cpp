#include <stmpfs_error.h>
#include <iostream>

int main()
{
    try {
        throw stmpfs_error_t(STMPFS_EXT_LIB_ERR);
    }
    catch (std::exception & err)
    {
        std::cout << err.what() << std::endl;
        return 0;
    }
}
