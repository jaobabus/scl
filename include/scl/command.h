#ifndef SCL_COMMAND_H
#define SCL_COMMAND_H

#include "argument.h"

#ifdef __cplusplus
extern "C" {
#endif


#define SCLC_FLAG_VA_ARG 0x01


/** SCLExecuteError
 */
typedef struct
{
    /// error
    /// @see SCLError
    uint8_t error;

    /// token
    /// Token index
    uint8_t token;
} SCLExecuteError;


/** SCLCommandDescriptor
 */
typedef struct
{
    /** execute
     *
     *  Execute with arguments packed to object
     *
     *  @param obj_buffer Raw pointer to packed object
     *  @param flags      Pointer to flags table (now not implemented)
     *  @param size       size of flags table
     *  @param @see SCLExecuteError
     */
    SCLExecuteError (*execute)(void* obj_buffer, const uint16_t* flags, size_t size);

    /** arguments
     *
     *  Table to arguments descriptors pointers
     */
    const SCLArgumentDescriptor* const* arguments;

    /** arguments_opaques
     *
     *  Tsble to arguments opaques
     */
    const void* const* arguments_opaques;

    /** arg_count
     */
    uint8_t arg_count;

    /** flags
     *
     *  combination of SCLC_FLAG_*
     */
    uint8_t flags;

    /** name_size
     */
    uint8_t name_size;

    /** pd
     *
     *  Padding to 8 bytes
     */
    uint8_t pd[5];
} SCLCommandDescriptor;


#ifdef __cplusplus
}

template<size_t name_size>
struct SCLCommandDescriptorWithName
{
    SCLCommandDescriptor base;
    char name_continue[name_size];
};

static_assert(sizeof(SCLCommandDescriptorWithName<8>) - sizeof(void*) * 3 - 8 == 8, "Align error");

#endif

#endif // SCL_COMMAND_H
