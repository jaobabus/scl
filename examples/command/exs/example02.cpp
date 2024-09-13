#include "example02.hpp"

#include <scl/parse.h>
#include <tokencolors.h>

#include <iostream>




int Example2::run()
{
    //std::string buffer{"cmd arg -fF --long-flag=value \"DStr\\x99\" 'SStr\\012' `PStr` $VAR"};
    std::string buffer{"\"DStr\\x99\" 'SStr\\012'"};

    const char* it = buffer.c_str();
    SHLParseState state{0};
    while (shl_get_token(state) != SHLT_StateError
           and shl_get_token(state) != SHLT_Eof)
    {
        if (it == (buffer.c_str() + buffer.size() + 1))
            throw std::runtime_error("Error: Parser reach out of bound");
        SHLParseResult res = shl_parse_next(state, *it);
        std::cout << shlt_get_color(shl_get_token(res.state)) << *it << "\e[0m";
        state = res.state;
        ++it;
    }
    std::cout << std::endl;

    return 0;
}

BaseExample::Info Example2::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
