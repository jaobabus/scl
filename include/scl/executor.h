#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.hpp"

#ifdef __cplusplus
extern "C" {
#endif


void* get_args_buf(size_t size);
void release_args_buf(void* ptr, size_t size);

SCLExecuteError scl_execute_inplace(const void* command,
                                    const SCLCommand* cmd,
                                    void** argument_opaque_table,
                                    char* cmdline, size_t size);



#ifdef __cplusplus
}
#endif


#endif // EXECUTOR_H
