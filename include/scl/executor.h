#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.hpp"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    void* (*alloc)(size_t size);
    void (*release)(void* ptr, size_t);
} SCLAllocator;

SCLExecuteError scl_execute_inplace(const void* command,
                                    const SCLCommand* cmd,
                                    void** argument_opaque_table,
                                    const SCLAllocator* alloc,
                                    char* cmdline, size_t size);



#ifdef __cplusplus
}
#endif


#endif // EXECUTOR_H
