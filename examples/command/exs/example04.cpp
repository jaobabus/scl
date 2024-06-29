#include "example04.hpp"

#include <vector>
#include <scl/parse.h>
#include <scl/inplace.h>
#include <tokencolors.h>

#include <termio.h>
#include <iostream>




int Example4::run()
{
    termios tios;
    tcgetattr(fileno(stdin), &tios);
    tios.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(fileno(stdin), 0, &tios);
    while (true) {
        std::vector<SHLParseState> context_list;
        std::string buffer;
        while (true) {
            char ch = getchar();
            if (ch == '\x7F') {
                if (buffer.empty())
                    continue;
                buffer.pop_back();
                context_list.pop_back();
                printf("\033[1D \033[1D");
            }
            else if (ch == '\n') {
                printf("\n");
                break;
            }
            else {
                const char* color = "\033[0m";
                auto pr = shl_parse_next(buffer.empty() ? SHLParseState{} : context_list.back(), ch);
                buffer.push_back(ch);
                context_list.push_back(pr.state);
                color = shlt_get_color(shl_get_token(context_list.back()));
                printf("%s%c\033[0m", color, ch);
            }
        }
    }
    return 0;
}

BaseExample::Info Example4::get_info() const
{
    Info info;
    info.block_terminal = true;
    // other options...
    return info;
}
