#include "example06.hpp"

#include <scl/executor.h>
#include <scl/inplace.h>
#include <scl/parse.h>
#include <scl/utils.hpp>
#include <tokencolors.h>


SCLExecuteError my_cmd_execute(void* obj_buffer,
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

uint8_t my_arg_parse(const void* opaque,
                      void* obj,
                      SHLITokenInfo token)
{
    *(StringView*)obj = StringView{(const char*)token.data, (size_t)token.size};
    return SCLE_NoError;
}

const
SCLArgumentDescriptor my_cmd_arguments[] =
{
    {
        +[] (void* arg) { ((StringView*)arg)->~StringView(); },
        my_arg_parse,
        nullptr,
        sizeof(StringView),
        0
    }
};

const SCLArgumentDescriptor* my_cmd_arguments_ptr[] = {&my_cmd_arguments[0]};

void* my_cmd_arguments_opaque[] = {nullptr};

SCLCommandDescriptor my_command_desc = {&my_cmd_execute,
                                     my_cmd_arguments_ptr,
                                     my_cmd_arguments_opaque,
                                     sizeof(my_cmd_arguments) / sizeof(my_cmd_arguments[0])};

int Example6::run()
{
    std::string buffer = "echo 'Hello world!'";
    shli_parse_inplace(&buffer[0], buffer.size());

    SCLAllocator alloc = {malloc, +[](void* p, size_t) { free(p); }};
    scl_execute_inplace(&my_command_desc, &alloc, &buffer[0], buffer.size());

    return 0;
}

BaseExample::Info Example6::get_info() const
{
    Info info;
    info.block_terminal = false;
    // other options...
    return info;
}
