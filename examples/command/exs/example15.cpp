#include "example15.hpp"

#include <iostream>




int Example15::run()
{
    std::cout << "Hello from example 1!" << std::endl;

    return 0;
}

BaseExample::Info Example15::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
