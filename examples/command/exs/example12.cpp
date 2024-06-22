#include "example12.hpp"

#include <iostream>




int Example12::run()
{
    std::cout << "Hello from example 1!" << std::endl;

    return 0;
}

BaseExample::Info Example12::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
