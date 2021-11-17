#include <buffer.h>
#include <iostream>

int main()
{
    buffer_t buffer;
    buffer.write("123\n", 4, 0);
    std::cout << buffer.get_sha256sum() << std::endl;

    return buffer.get_sha256sum() != "181210f8f9c779c26da1d9b2075bde0127302ee0e3fca38c9a83f5b1dd8e5d3b";
}
