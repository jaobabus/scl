#include <scl/executor.h>
#include <scl/inplace.h>
#include <scl/calt.h>
#include <string.h>



SCL_CPP_ANON_NAMESPACE_START


SCL_STATIC_API_BEFORE_TYPE
bool is_argument(uint8_t token)
{
    return token == SHLT_Word
            or token == SHLT_String
            or token == SHLT_String;
}

SCL_STATIC_API_BEFORE_TYPE
void* alloc_arguments_buffer(const SCLCommandDescriptor* cmd, const SCLAllocator* alloc)
{
    size_t size = 1;
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        size_t sz = cmd->arguments[i]->obj_offset + cmd->arguments[i]->obj_size;
        if (sz > size)
            size = sz;
    }
    void* buffer = alloc->alloc(size);
    return buffer;
}

SCL_STATIC_API_BEFORE_TYPE
void free_arguments_buffer(const SCLCommandDescriptor* cmd, const SCLAllocator* alloc, void* buffer)
{
    size_t size = 1;
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        if (cmd->arguments[i]->destruct)
            cmd->arguments[i]->destruct((char*)buffer + cmd->arguments[i]->obj_offset);
        size_t sz = cmd->arguments[i]->obj_offset + cmd->arguments[i]->obj_size;
        if (sz > size)
            size = sz;
    }
    alloc->release(buffer, size);
}

SCL_STATIC_API_BEFORE_TYPE
SCLExecuteError parse_arguments(const SCLCommandDescriptor* cmd,
                                void* arguments,
                                char* cmdline, size_t size)
{
    SHLITokenInfo token = shli_parse_data(cmdline);
    uint16_t flags_buffer[16];
    uint8_t flags_top = 0;
    uint8_t token_index = 1, arg_index = 0;
    while (token.token != SHLT_Eof
           and token.token != SHLT_StateError
           and arg_index < cmd->arg_count)
    {
        while (token.token == SHLT_Whitespace)
        {
            token = shli_next_token(token);
            token_index++;
        }
        if (token.token == SHLT_Flag)
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
        }
        else if (is_argument(token.token))
        {
            void* arg = (char*)arguments + cmd->arguments[arg_index]->obj_offset;
            SCLError err = (SCLError)cmd->arguments[arg_index]->parse((cmd->arguments_opaques ? cmd->arguments_opaques[arg_index] : NULL), arg, token);
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
    return cmd->execute(arguments, flags_buffer, flags_top);
}


SCL_CPP_ANON_NAMESPACE_END



SCL_EXPORT_API_BEFORE_TYPE
SCLExecuteError SCL_EXPORT_API_AFTER_TYPE
    scl_execute_inplace(const SCLCommandDescriptor* cmd,
                        const SCLAllocator* alloc,
                        char* cmdline, size_t size)
{
    void* arguments = alloc_arguments_buffer(cmd, alloc);
    SCLExecuteError error = parse_arguments(cmd, arguments, cmdline, size);
    free_arguments_buffer(cmd, alloc, arguments);
    return error;
}


SCL_EXPORT_API_BEFORE_TYPE
SCLExecuteError SCL_EXPORT_API_AFTER_TYPE
    scl_find_and_execute_inplace(const SCLCommandDescriptor * const *cmd,
                                 size_t count,
                                 const SCLAllocator *alloc,
                                 char *cmdline, size_t size)
{
    shli_parse_inplace(cmdline, size);
    SHLITokenInfo name = shli_parse_data(cmdline);
    SHLITokenInfo token = shli_next_token(name);
    SCLExecuteError err = {SCLE_CommandNotFound, 0};
    while (token.token == SHLT_Whitespace)
        token = shli_next_token(token);
    for (size_t i = 0; i < count; ++i)
    {
        const char* cmd_name = (const char*)(cmd[i] + 1);
        if (strlen(cmd_name) == name.size and memcmp(name.data, cmd_name, name.size) == 0)
            return scl_execute_inplace(cmd[i], alloc, (char*)token.head, size - ((char*)token.head - cmdline));
    }
    return err;
}
