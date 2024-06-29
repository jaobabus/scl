#include "example06.hpp"

#include <vector>
#include <scl/parse.h>
#include <scl/inplace.h>
#include <scl/executor.h>
#include <tokencolors.h>

#include <iostream>


SCLExecuteError my_cmd_execute(const void* opaque,
                               void* obj_buffer,
                               const uint16_t* flags,
                               size_t size)
{
    auto& arg = *(const StringView*)obj_buffer;
    fwrite(arg.data(), 1, arg.size(), stdout);
    fwrite("\n", 1, 1, stdout);
    SCLExecuteError error;
    error.error = 0;
    return error;
}

SCLError my_arg_parse(const void* opaque,
                      void* obj,
                      SHLITokenInfo token)
{
    *(StringView*)obj = StringView{(const char*)token.data, (size_t)token.size};
    return SCLE_NoError;
}

const
SCLArgument my_cmd_arguments[] =
{
    {
        +[] (void* arg) { new(arg) StringView; },
        +[] (void* arg) { ((StringView*)arg)->~StringView(); },
        my_arg_parse,
        nullptr,
        sizeof(StringView),
        0
    }
};

void* my_cmd_arguments_opaque[] =
{
    nullptr
};

SCLCommand my_command_desc =
{
    &my_cmd_execute,
    +[](const void*, const char* name, size_t size) -> uint8_t { return std::string(name, name + size) == "echo"; },
    my_cmd_arguments,
    sizeof(my_cmd_arguments) / sizeof(my_cmd_arguments[0])
};



int Example6::run()
{
    std::string buffer = "echo 'Hello world!'";
    shli_parse_inplace(&buffer[0], buffer.size());

    scl_execute_inplace(nullptr, &my_command_desc, my_cmd_arguments_opaque, &buffer[0], buffer.size());

    return 0;
}

BaseExample::Info Example6::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
