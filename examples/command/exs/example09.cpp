#include "example09.hpp"

#if 0

#include "/home/jabus/repos/projects/shlexer/ch32v003-scl-demo/src/cli.hpp"
#include "/home/jabus/repos/projects/shlexer/ch32v003-scl-demo/src/cli.cpp"

// #include <coprintf.hpp>
#include <tokencolors.h>
#include <vector>

#include <termio.h>
#include <iostream>
#include <thread>

#include <iostream>


constexpr SCLError StringArgErrorToken = SCLE_UserErrorsStart;
constexpr SCLError IntArgInvalidChar = SCLError(SCLE_UserErrorsStart + 1);

void uwrite(const char* d, size_t s)
{
    fwrite(d, s, 1, stdout);
}

class StringArg : public TypedArgument<StringArg, StringView>
{
public:
    static uint8_t parse(type& value, SHLITokenInfo token) noexcept
    {
        value = StringView{(const char*)token.data, token.size};
        return SCLE_NoError;
    }

};


class EchoCommand : public TypedCommand<EchoCommand, StringArg>
{
public:
    static uint8_t execute(const StringView& sw) noexcept
    {
        uwrite(sw.data(), sw.size());
        uwrite("\r\n", 2);
        return SCLE_NoError;
    }

    constexpr static const void** sc_opaques = nullptr;
    constexpr static SCLCommandDescriptorWithName<5> sc_descriptor{sc_descriptor_base, {"echo"}};
};


class IntArg : public TypedArgument<IntArg, int32_t>
{
public:
    uint8_t parse(type& value, SHLITokenInfo token) const noexcept
    {
        const char* it = (char*)token.data, * end = it + token.size;
        if (*it == '-')
            it++;
        value = 0;
        while (it != end)
        {
            value *= 10;
            if (*it >= '0' and *it <= '9')
                value += *it - '0';
            else
                return IntArgInvalidChar;
            ++it;
        }
        if (((char*)token.data)[0] == '-')
            value = -value;
        return SCLE_NoError;
    }

};


class AddCommand : public TypedCommand<AddCommand, IntArg, IntArg>
{
public:
    static uint8_t execute(const int& a, const int& b) noexcept
    {
        char buf[10];
        int r = a + b;
        bool neg = false;
        uint8_t it = 7;
        if (r < 0)
            r = -r, neg = true;
        while (r != 0)
        {
            buf[it] = '0' + r % 10;
            r /= 10;
            --it;
        }
        if (neg)
            buf[it--] = '-';
        buf[8] = '\r';
        buf[9] = '\n';
        uwrite(buf + it + 1, 9 - it);
        return SCLE_NoError;
    }

    constexpr static const void** sc_opaques = nullptr;
    constexpr static SCLCommandDescriptorWithName<4> sc_descriptor{sc_descriptor_base, {"add"}};
};


int Example9::run()
{
    ConsoleExecutor::CommandHandle handlers[] = {
        default_handle<EchoCommand>("echo"),
        default_handle<AddCommand>("add"),
    };

    using Tuple = MyTuple<IntArg::type, IntArg::type>;
    using elem0 = tuple_element<0, Tuple>;
    using elem1 = tuple_element<1, Tuple>;

    auto i = elem0::index;
    auto o = elem0::offset;
    auto i1 = elem1::index;
    auto o1 = elem1::offset;

    return 0;
}

BaseExample::Info Example9::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}

#endif

int Example9::run()
{

    return 0;
}

BaseExample::Info Example9::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
