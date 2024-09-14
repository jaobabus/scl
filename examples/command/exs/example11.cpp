#include "example11.hpp"
#include "scl/consolebuffer.h"

#include <vector>
#include <scl/parse.h>
#include <scl/inplace.h>
#include <scl/command.hpp>
#include <scl/executor.h>
#include <tokencolors.h>
#include <coprintf.hpp>

#include <termio.h>
#include <iostream>
#include <thread>




namespace
{



class ConsoleExecutor;
ConsoleExecutor* executor;

class ConsoleExecutor
{
public:
    struct CommandHandle
    {
        const SCLCommandDescriptor* descriptor;
        const char* name;
    };

public:
    ConsoleExecutor(CommandHandle* handlers, size_t count)
        : _handlers(handlers), _size(count)
    {
        static uint8_t colors[16] = {15, 15, 196, 15, 148, 64, 198, 208, 253, 15, 15, 15, 15, 196, 196, 196};
        _ctx = sclcb_make_context(new char[128], 128,
                                  this, on_write,
                                  +[](void*, void* d, size_t s) { executor->on_command(d, s); },
                                  colors);
    }

    static constexpr SCLAllocator system_alloc = { malloc, +[](void* p, size_t) { free(p); } };

public:
    static void on_write(void*, const void* data, size_t size)
    {
        fwrite(data, size, 1, stdout);
    }

    void on_command(void* buffer, size_t size)
    {
        shli_parse_inplace(buffer, size);
        auto first = shli_parse_data(buffer);
        auto next = first;
        shli_next_token(&next);
        while (next.token == SHLT_Whitespace)
            shli_next_token(&next);
        for (size_t i = 0; i < _size; i++)
        {
            auto& hnd = _handlers[i];
            if (hnd.name == std::string((char*)first.data, (char*)first.data + first.size))
            {
                auto sz = size - ((char*)next.data - (char*)buffer);
                auto err = scl_execute_inplace(hnd.descriptor, &system_alloc, (char*)next.data, sz);
                if (err.error != 0)
                {
                    printf("Error %d in %d\n", (int)err.error, (int)err.token);
                }
                return;
            }
        }
        std::string name{(char*)first.data, (char*)first.data + first.size};
        printf("Error: command '%s' not found\n", name.c_str());
    }

    void on_char(char ch)
    {
        sclcb_on_char(&_ctx, ch);
    }

private:
    CommandHandle* _handlers;
    size_t _size;
    SCLConsoleBufferContext _ctx;

};

constexpr SCLError StringArgErrorToken = SCLE_UserErrorsStart;


class StringArg : public TypedArgument<StringArg, StringView>
{
public:
    SCLError parse(type& value, SHLITokenInfo token) const noexcept
    {
        value = StringView{(const char*)token.data, token.size};
        return SCLE_NoError;
    }

};


class EchoCommand : public TypedCommand<EchoCommand, StringArg>
{
public:
    static const uint8_t execute(const StringView& sw) noexcept
    {
        fwrite(sw.data(), sw.size(), 1, stdout);
        fwrite("\n", 1, 1, stdout);
        return SCLE_NoError;
    }

    constexpr static const void** sc_opaques = nullptr;
    constexpr static SCLCommandDescriptorWithName<5> sc_descriptor{sc_descriptor_base, {"echo"}};
};

template<typename Command>
constexpr ConsoleExecutor::CommandHandle default_handle(const char* name)
{
    return ConsoleExecutor::CommandHandle{&Command::sc_descriptor.base, name};
}

}


int Example11::run()
{
    coprintf("~~~~~~ Start ~~~~~~");
    termios tios;
    tcgetattr(fileno(stdin), &tios);
    tios.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(fileno(stdin), 0, &tios);
    ConsoleExecutor::CommandHandle handlers[] = {
        default_handle<EchoCommand>("echo")
    };
    ConsoleExecutor buffer(handlers, sizeof(handlers) / sizeof(handlers[0]));
    executor = &buffer;
    while (true)
    {
        int ch = getchar();
        if (ch == -1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        buffer.on_char((uint8_t)ch);
    }

    return 0;
}

BaseExample::Info Example11::get_info() const
{
    Info info;
    info.block_terminal = true;
    // other options...
    return info;
}
