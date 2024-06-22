#include "example01.hpp"

#include <iostream>




int Example1::run()
{
    std::cout << "Hello from example 1!" << std::endl;

    return 0;
}

BaseExample::Info Example1::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
