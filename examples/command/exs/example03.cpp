#include "example03.hpp"

#include <vector>
#include <scl/parse.h>
#include <scl/inplace.h>
#include <tokencolors.h>

#include <iostream>




int Example3::run()
{
    std::string buffer{"cmd arg -fF --long-flag=value \"DStr\" 'SStr' `PStr` $VAR"};

    std::vector<void*> tokens;

    const char* it = buffer.c_str();
    SHLParseState state{0};
    SHLInplaceContext ctx = shli_make_context(&buffer[0], SHLT_None);
    while (shl_get_token(state) != SHLT_StateError
           and shl_get_token(state) != SHLT_Eof)
    {
        if (it == (buffer.c_str() + buffer.size() + 1))
            break;
//            throw std::runtime_error("Error: Parser reach out of bound");
        SHLParseResult res = shl_parse_next(state, *it);
        {
            auto s = (res.parsed ? std::string(1, res.parsed) : "\\0");
            printf("'%s':\t%2d, %s\n", s.c_str(), res.state, shl_str_token(shl_get_token(res.state)));
        }
        if (shl_get_token(res.state) != shl_get_token(state))
        {
            shli_end(&ctx);
            shli_reuse_context(&ctx, shl_get_token(res.state));
            if (shl_get_token(res.state) != SHLT_None)
                tokens.push_back(ctx.head);
        }
        if (res.parsed)
            shli_next(&ctx, res.parsed);
        state = res.state;
        ++it;
    }
    shli_end(&ctx);

    auto print_token = [](auto& token)
    {
        SHLITokenInfo info = shli_parse_data(token);
        printf("%s%s: ",
               shlt_get_color((SHLToken)info.token),
               shl_str_token((SHLToken)info.token));
        switch (info.data_type)
        {
        case SHLI_CVT_DTE_11:
            printf("%02hhx\n",
                   *(const uint8_t*)info.head);
            break;

        case SHLI_CVT_DTE_12: {
            printf("%02hhx ",
                   *(const uint8_t*)info.head);
            std::string buf{(const char*)info.data, (const char*)info.data + info.size};
            printf("%s\n",
                   buf.c_str());
        }   break;

        case SHLI_CVT_DTE_21:
            printf("%02hhx %02hhx ",
                   ((const char*)info.head)[0],
                   ((const char*)info.head)[1]);

        case SHLI_CVT_DTE_3: {
            std::string buf{(const char*)info.data, (const char*)info.data + info.size};
            printf("%s\n",
                   buf.c_str());
        }   break;
        }
    };

    const char* marker_color = "\033[38;5;69m";
    const char* size_color = "\033[38;5;210m";
    for (auto& token : tokens)
    {
        print_token(token);
    }

    std::cout << "\n\n";
    auto token = shli_parse_data(buffer.data());
    while (token.token != SHLT_Eof)
    {
        print_token(token.head);
        token = shli_next_token(token);
    }

    return 0;
}

BaseExample::Info Example3::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
