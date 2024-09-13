#ifndef SCL_COMMAND_H
#define SCL_COMMAND_H

#include "argument.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCLC_FLAG_VA_ARG 0x01

typedef struct
{
    uint8_t error;
    uint8_t token;
} SCLExecuteError;

typedef struct
{
    SCLExecuteError (*execute)(void* obj_buffer, const uint16_t* flags, size_t size);
    const SCLArgumentDescriptor* const* arguments;
    const void* const* arguments_opaques;
    uint8_t arg_count;
    uint8_t flags;
    uint8_t name_size;
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
