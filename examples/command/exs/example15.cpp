#include "example15.hpp"

#include <iostream>


#include <scl/command.hpp>



int Example15::run()
{
    return 0;
}

BaseExample::Info Example15::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
