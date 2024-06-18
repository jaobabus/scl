#ifndef SHLINPLACE_H
#define SHLINPLACE_H

#include <stdint.h>
#include <stddef.h>

#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif




typedef struct {
    const void* header;
    const void* data;
    uint8_t token;
    uint8_t data_type;
    uint16_t size;
} SHLIDataInfo;


typedef struct {
    void* head;
    void* current;
    uint8_t flags;
    char temp;
} SHLInplaceContext;


// Writing
SHLInplaceContext shli_make_context(void* buffer, uint8_t token);
void shli_reuse_context(SHLInplaceContext* ctx, uint8_t token);
void shli_next(SHLInplaceContext* ctx, uint8_t chr);
void shli_end(SHLInplaceContext* ctx);


// Reading
uint8_t shli_get_header_size(uint8_t data_type);
SHLIDataInfo shli_parse_data(const void* head);
SHLIDataInfo shli_next_token(SHLIDataInfo prev);


// Using
void shli_parse_inplace(void* buffer, size_t size);



#ifdef __cplusplus
}
#endif


#endif // SHLINPLACE_H
