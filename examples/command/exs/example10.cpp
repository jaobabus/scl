#include "example10.hpp"

#include <scl/parse.h>
#include <scl/inplace.h>
#include <scl/command.hpp>
#include <scl/executor.h>
#include <scl/consolebuffer.h>
#include <tokencolors.h>
#include <coprintf.hpp>

#include "arguments.hpp"

#include <termio.h>
#include <thread>



namespace
{


SCLC_DeclareCommand(echo,
                    StringView sv,
                    int a,
                    StringView sv2)
{
    fwrite(sv.data(), sv.size(), 1, stdout);
    fwrite("\n", 1, 1, stdout);
    return SCLE_NoError;
}

const SCLCommandDescriptor* commands[] =
{
    &command_echo.sc_descriptor.base,
};

}


int Example10::run()
{
    coprintf("~~~~~~ Start ~~~~~~");
    termios tios;
    tcgetattr(fileno(stdin), &tios);
    tios.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(fileno(stdin), 0, &tios);

    auto size = get_max_object_alloc(commands, 1);

    auto write = +[](void*, const void* data, size_t size)
    {
        fwrite(data, size, 1, stdout);
    };

    auto execute = +[](void*, void* cmd, size_t size)
    {
        auto alloc = SCLAllocator{malloc, +[](void* d, size_t) { free(d); }};
        auto err = scl_find_and_execute_inplace(commands, sizeof(commands) / sizeof(commands[0]),
                                                &alloc, (char*)cmd, size);
        if (err.error) {
            fprintf(stderr, "Error %d at %d\n", err.error, err.token);
        }
    };

    char cmd_buffer[128];
    SCLConsoleBufferContext buffer = sclcb_make_context(
                cmd_buffer, sizeof(cmd_buffer),
                nullptr,
                write,
                execute,
                (const uint8_t*)"\x0F\x0F\xC4\x0F\x94\x40\xC6\xD0\xFD\x0F\x0F\x0F\x0F\xC4\xC4\xC4"
    );

    while (true)
    {
        int ch = getchar();
        if (ch == -1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        sclcb_on_char(&buffer, (uint8_t)ch);
    }

    return 0;
}

BaseExample::Info Example10::get_info() const
{
    Info info;
    info.block_terminal = true;
    // other options...
    return info;
}
