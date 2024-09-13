#ifndef SCL_ARGUMENT_H
#define SCL_ARGUMENT_H

#include "inplace.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void (*destruct)(void* obj); // Constructor must be called in parse()
    uint8_t (*parse)(const void* opaque, void* obj, SHLITokenInfo token);
    size_t (*completes)(
        const void* opaque, char* buffer, size_t buffer_size, const char* token, size_t size);
    uint16_t obj_size;
    uint16_t obj_offset;
} SCLArgumentDescriptor;

#ifdef __cplusplus
}
#endif

#endif // SCL_ARGUMENT_H
