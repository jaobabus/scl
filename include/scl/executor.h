#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void* (*alloc)(size_t size);
    void (*release)(void* ptr, size_t);
} SCLAllocator;

SCLExecuteError scl_execute_inplace(const SCLCommandDescriptor* cmd,
                                    const SCLAllocator* alloc,
                                    char* cmdline,
                                    size_t size);

SCLExecuteError scl_find_and_execute_inplace(const SCLCommandDescriptor * const *cmd,
                                             size_t count,
                                             const SCLAllocator *alloc,
                                             char *cmdline, size_t size);

#ifdef __cplusplus
}
#endif

#endif // EXECUTOR_H
