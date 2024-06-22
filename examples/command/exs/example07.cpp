#include "example07.hpp"

#include <iostream>




int Example7::run()
{
    std::cout << "Hello from example 1!" << std::endl;

    return 0;
}

BaseExample::Info Example7::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
