#include "example07.hpp"

#include <vector>
#include <scl/parse.h>
#include <scl/inplace.h>
#include <scl/command.hpp>
#include <scl/executor.h>
#include <tokencolors.h>
#include <coprintf.hpp>

#include "arguments.hpp"

#include <termio.h>
#include <iostream>
#include <thread>



class ConsoleBuffer
{
public:
    ConsoleBuffer()
        : _cursor(0), _escape(0) {}

    virtual void on_command(char* buffer, size_t size) = 0;

    template<typename T>
    void safe_redraw(T cb)
    {
        printf("\e[?25l\e7");
        cb();
        printf("\e8\e[?25h");
    }

    void on_char(char ch)
    {
        if (_escape)
        {
            switch (_escape)
            {
            case 1:
                if (ch == '[')
                    _escape = 2;
                else
                    _escape = 0, coprintf("Esc error unknown 1lvl esc '%c' (%d)", ch, int(ch));;
                break;
            case 2:
                switch (ch)
                {
                case 'A':
                    break;
                case 'B':
                    break;
                case 'C':
                    if (_cursor < _buffer.size())
                    {
                        _cursor++;
                        printf("\e[1C");
                    }
                    break;
                case 'D':
                    if (_cursor > 0)
                    {
                        _cursor--;
                        printf("\e[1D");
                    }
                    break;
                case '3':
                    coprintf("DEL");
                    if (_cursor < _buffer.size())
                    {
                        printf("\e[1D");
                        _buffer.erase(_buffer.begin() + _cursor, _buffer.begin() + _cursor + 1);
                        safe_redraw([&]
                        {
                            fwrite(_buffer.c_str() + _cursor, _buffer.size() - _cursor, 1, stdout);
                            fwrite(" ", 1, 1, stdout);
                        });
                    }
                    _escape = 3;
                    break;
                default:
                    coprintf("Esc error unknown 2lvl esc '%c' (%d)", ch, int(ch));
                }
                if (_escape == 2)
                    _escape = 0;
                break;
            case 3:
                coprintf("Esc del end lvl 3: '%c' (%d)", ch, int(ch));
                _escape = 0;
                break;
            default:
                coprintf("Esc error lvl %d", _escape);
                _escape = 0;
            }
        }
        else if (ch == '\x7F')
        {
            coprintf("BS");
            if (_cursor == 0)
                return;
            _cursor--;
            printf("\e[1D");
            _buffer.erase(_buffer.begin() + _cursor, _buffer.begin() + _cursor + 1);
            safe_redraw([&]
            {
                fwrite(_buffer.c_str() + _cursor, _buffer.size() - _cursor, 1, stdout);
                fwrite(" ", 1, 1, stdout);
            });
        }
        else if (ch == '\e')
        {
            coprintf("ESC");
            _escape = 1;
        }
        else if (ch == '\n')
        {
            coprintf("Command '%s'", _buffer.c_str());
            printf("\n");
            on_command(&_buffer[0], _buffer.size());
            _buffer = "";
            _cursor = 0;
        }
        else
        {
            coprintf("Add '%c' (%d)",
                     ch, int((uint8_t)ch));
            _buffer.insert(_buffer.begin() + _cursor, ch);
            safe_redraw([&]
            {
                fwrite(_buffer.c_str() + _cursor, _buffer.size() - _cursor, 1, stdout);
            });
            _cursor++;
            printf("\e[1C");
        }
        coprintf("cur:%+2d, buf:'%s', e:%d\n",
                 _cursor,
                 _buffer.c_str(),
                 _escape);
    }

private:
    std::string _buffer;
    size_t _cursor;
    int _escape;

};


class ConsoleExecutor : public ConsoleBuffer
{
public:
    struct CommandHandle
    {
        const SCLCommandDescriptor* descriptor;
        void* opaque;
        void** arguments_opaque_table;
    };

public:
    void register_command(CommandHandle hnd)
    {
        _handlers.push_back(hnd);
    }

public:
    void on_command(char* buffer, size_t size) override
    {
        shli_parse_inplace(buffer, size);
        auto first = shli_parse_data(buffer);
        auto next = shli_next_token(first);
        while (next.token == SHLT_Whitespace)
            next = shli_next_token(next);
        for (auto& hnd : _handlers)
        {/*
            if (hnd.descriptor->is_command(hnd.opaque, (const char*)first.data, first.size))
            {
                auto sz = size - ((char*)next.data - buffer);
                auto err = scl_execute_inplace(hnd.opaque, hnd.descriptor, hnd.arguments_opaque_table, (char*)next.data, sz);
                if (err.error != 0)
                {
                    printf("Error %d in %d\n", (int)err.error, (int)err.token);
                }
                return;
            }*/
        }
        std::string name{(char*)first.data, (char*)first.data + first.size};
        printf("Error: command '%s' not found\n", name.c_str());
    }

private:
    std::vector<CommandHandle> _handlers;

};

template<typename Command>
ConsoleExecutor::CommandHandle default_handle(Command* cmd, void** args_opaque_table)
{
    return ConsoleExecutor::CommandHandle
    {
        &Command::sc_descriptor,
        cmd,
        args_opaque_table
    };
}


class EchoCommand : public TypedCommand<EchoCommand, StringArg>
{
public:
    SCLError execute(const StringView& sw) const noexcept
    {
        fwrite(sw.data(), sw.size(), 1, stdout);
        fwrite("\n", 1, 1, stdout);
        return SCLE_NoError;
    }
};


int Example7::run()
{
    termios tios;
    tcgetattr(fileno(stdin), &tios);
    tios.c_lflag &= ~ECHO & ~ICANON;
    tcsetattr(fileno(stdin), 0, &tios);
    ConsoleExecutor buffer;
    void* opaques[] = { new StringArg{} };
    // buffer.register_command(default_handle<EchoCommand>(new EchoCommand(), opaques));
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

BaseExample::Info Example7::get_info() const
{
    Info info;
    info.block_terminal = true;
    // other options...
    return info;
}
