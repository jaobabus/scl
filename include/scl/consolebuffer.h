#pragma once

#include <stdint.h>
#include <stddef.h>



#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
    void (*write)(void* opaque, const void* data, size_t size);
    void (*execute)(void* opaque, void* buffer, size_t size);
#ifndef SCL_NO_COLORS
    const uint8_t* colors;
#endif
    void* opaque;
    uint8_t* buffer;
    uint16_t buffer_size;
    uint16_t size;
    uint16_t cursor;
    uint8_t escape_index;
    uint8_t escape_stack_size;
} SCLConsoleBufferContext;


SCLConsoleBufferContext sclcb_make_context(void* buffer, uint16_t size,
                                           void* opaque,
                                           void (*write)(void* opaque, const void* data, size_t size),
                                           void (*execute)(void* opaque, void* buffer, size_t size),
                                           const uint8_t* colors);


void sclcb_on_char(SCLConsoleBufferContext* ctx, char ch);
void sclcb_clear(SCLConsoleBufferContext* ctx);


#ifdef __cplusplus
}
#endif
