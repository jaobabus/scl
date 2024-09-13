#include "example01.hpp"

#include <scl/parse.h>
#include <tokencolors.h>

#include <iostream>




int Example1::run()
{
    std::string buffer{"cmd arg -fF --long-flag=value \"DStr\" 'SStr' `PStr` $VAR"};

    const char* it = buffer.c_str();
    SHLParseState state{0};
    while (shl_get_token(state) != SHLT_StateError
           and shl_get_token(state) != SHLT_Eof)
    {
        if (it == (buffer.c_str() + buffer.size() + 1))
            break; // throw std::runtime_error("Error: Parser reach out of bound");
        SHLParseResult res = shl_parse_next(state, *it);
        std::cout << shlt_get_color(shl_get_token(res.state)) << *it << " - " << shl_str_token(shl_get_token(res.state)) << "\e[0m" << "\n";
        state = res.state;
        ++it;
    }
    std::cout << std::endl;

    return 0;
}

BaseExample::Info Example1::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
