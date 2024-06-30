#include <scl/executor.h>

#include <stdlib.h>

#define true 1
#define false 0
#define not !
#define and &&
#define or ||

typedef uint8_t bool;



static
bool is_argument(uint8_t token)
{
    return token == SHLT_Argument
            or token == SHLT_StringSQ
            or token == SHLT_StringDQ;
}

static
void* alloc_arguments_buffer(const SCLCommand* cmd, const SCLAllocator* alloc)
{
    size_t size = 1;
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        size_t sz = cmd->arguments[i].obj_offset + cmd->arguments[i].obj_size;
        if (sz > size)
            size = sz;
    }
    void* buffer = alloc->alloc(size);
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        cmd->arguments[i].construct((char*)buffer + cmd->arguments[i].obj_offset);
    }
    return buffer;
}

static
void free_arguments_buffer(const SCLCommand* cmd, const SCLAllocator* alloc, void* buffer)
{
    size_t size = 1;
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        cmd->arguments[i].destruct((char*)buffer + cmd->arguments[i].obj_offset);
        size_t sz = cmd->arguments[i].obj_offset + cmd->arguments[i].obj_size;
        if (sz > size)
            size = sz;
    }
    alloc->release(buffer, size);
}

static
SCLExecuteError parse_arguments(SHLITokenInfo token,
                                const void* opaque,
                                const SCLCommand* cmd,
                                void** argument_opaque_table,
                                void* arguments,
                                char* cmdline, size_t size)
{
    uint16_t flags_buffer[16];
    uint8_t flags_top = 0;
    uint8_t token_index = 1, arg_index = 0;
    while (token.token != SHLT_EOF
           and token.token != SHLT_StateError)
    {
        while (token.token == SHLT_Whitespace)
        {
            token = shli_next_token(token);
            token_index++;
        }
        if (token.token == SHLT_Flag
            or token.token == SHLT_LongFlag)
        {
            if (flags_top >= sizeof(flags_buffer) / sizeof(flags_buffer[0]))
            {
                SCLExecuteError error;
                error.token = token_index;
                error.error = SCLE_MemoryLimitError;
                return error;
            }
            flags_buffer[flags_top++] = (char*)token.head - cmdline;
            SHLITokenInfo next = shli_next_token(token);
            if (next.token == SHLT_FlagEquals)
            {
                token = shli_next_token(next);
                token_index += 2;
            }
        }
        else if (is_argument(token.token))
        {
            void* arg = (char*)arguments + cmd->arguments[arg_index].obj_offset;
            SCLError err = cmd->arguments[arg_index].parse(argument_opaque_table[arg_index], arg, token);
            if (err != SCLE_NoError)
            {
                SCLExecuteError error;
                error.token = token_index;
                error.error = err;
                return error;
            }
            arg_index++;
        }
        else
        {
            SCLExecuteError error;
            error.token = token_index;
            error.error = SCLE_NotImplemented;
            return error;
        }
        token = shli_next_token(token);
        token_index++;
    }
    return cmd->execute(opaque, arguments, flags_buffer, flags_top);
}

SCLExecuteError scl_execute_inplace(const void* opaque,
                                    const SCLCommand* cmd,
                                    void** argument_opaque_table,
                                    const SCLAllocator* alloc,
                                    char* cmdline, size_t size)
{
    SHLITokenInfo token = shli_parse_data(cmdline);
    void* arguments = alloc_arguments_buffer(cmd, alloc);
    SCLExecuteError error = parse_arguments(token, opaque, cmd, argument_opaque_table, arguments, cmdline, size);
    free_arguments_buffer(cmd, alloc, arguments);
    return error;
}
