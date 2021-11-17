#include <pathname_t.h>
#include <iostream>

int main()
{
    stmpfs_pathname_t pathname("/tmp/tmp/tmp");
    for (const auto& i : pathname)
    {
        std::cout << i << std::endl;
    }
}
