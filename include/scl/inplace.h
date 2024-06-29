#ifndef SHLINPLACE_H
#define SHLINPLACE_H

#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif




typedef struct {
    void* head;
    void* data;
    uint8_t token;
    uint8_t data_type;
    uint16_t size;
} SHLITokenInfo;


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
SHLITokenInfo shli_parse_data(void* head);
SHLITokenInfo shli_next_token(SHLITokenInfo prev);


// Using
void shli_parse_inplace(void* buffer, size_t size);
SHLInplaceContext shli_continue(SHLITokenInfo token);



#ifdef __cplusplus
}
#endif


#endif // SHLINPLACE_H
