#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.h"

#ifdef __cplusplus
extern "C" {
#endif


/** SCLAllocator
 */
typedef struct
{
    /** alloc
     *
     *  @param  size sizeof of object
     *  @return allocated object
     */
    void* (*alloc)(size_t size);

    /** release
     *
     *  @param  ptr  pointer to allocated object
     *  @param  size sizeof of allocated object
     */
    void (*release)(void* ptr, size_t size);
} SCLAllocator;


/** scl_execute_inplace
 *
 *  Parse, pack arguments and execute
 *
 *  @param cmd     Descriptor of executing command
 *  @param alloc   Allocator for arguments
 *  @param cmdline Parsed inplcace command line, first argument token
 *  @param size    size of cmdline in bytes
 *  @return Error while parsing arguments or error from SCLCommandDescriptor::execute
 */
SCLExecuteError scl_execute_inplace(const SCLCommandDescriptor* cmd,
                                    const SCLAllocator* alloc,
                                    char* cmdline,
                                    size_t size);


/** scl_find_and_execute_inplace
 *
 *  Find command and call scl_execute_inplace
 *
 *  @param cmds    Descriptors table
 *  @param count   Count of table
 *  @param alloc   Allocator for arguments
 *  @param cmdline Parsed inplcace command line, first argument token
 *  @param size    size of cmdline in bytes
 *  @return SCLE_CommandNotFound or error from scl_execute_inplace
 */
SCLExecuteError scl_find_and_execute_inplace(const SCLCommandDescriptor * const *cmds,
                                             size_t count,
                                             const SCLAllocator *alloc,
                                             char *cmdline, size_t size);


#ifdef __cplusplus
}
#endif

#endif // EXECUTOR_H
