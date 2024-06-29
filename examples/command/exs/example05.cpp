#include "example05.hpp"

#include <vector>
#include <scl/parse.h>
#include <scl/inplace.h>
#include <tokencolors.h>

#include <iostream>




int Example5::run()
{
    std::cout << "..." << std::endl;

    return 0;
}

BaseExample::Info Example5::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
