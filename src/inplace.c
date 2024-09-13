#include "scl/ard.h"
#include <scl/inplace.h>
#include <scl/parse.h>

#include <stddef.h>
#include <scl/calt.h>



SCL_CPP_ANON_NAMESPACE_START


#define SCL_PRIVATE_FLAG_HAS_HEAD  0x80
#define SCL_PRIVATE_FLAG_HAS_DATA  0x40
#define SCL_PRIVATE_FLAG_SWAP_TEMP 0x20 // for sizeof(header) == 2
#define SCL_PRIVATE_FLAG_STATE_ERR 0x10


SCL_STATIC_API_BEFORE_TYPE
const uint8_t cvt_table_wi[6] = {
    0,
    SHLI_CVT_DTE_11_OFFSET | SCL_PRIVATE_FLAG_HAS_HEAD,
    SHLI_CVT_DTE_12_OFFSET | SCL_PRIVATE_FLAG_HAS_HEAD | SCL_PRIVATE_FLAG_HAS_DATA,
    SHLI_CVT_DTE_21_OFFSET | SCL_PRIVATE_FLAG_HAS_HEAD | SCL_PRIVATE_FLAG_HAS_DATA | SCL_PRIVATE_FLAG_SWAP_TEMP,
    SHLI_CVT_DTE_3_OFFSET  | 0             | SCL_PRIVATE_FLAG_HAS_DATA,
    16                     | 0
};


SCL_STATIC_API_BEFORE_TYPE
const uint8_t init_mask_table[6] = {
    0x00,
    0x00,
    0x08,
    0x80,
    0x00,
    0x00
};


SCL_STATIC_API_BEFORE_TYPE
uint8_t get_data_type(uint8_t token)
{
    size_t i;
    SHLITokenInfo info;
    for (i = 1; i < sizeof(cvt_table_wi); i++) {
        if (token < (cvt_table_wi[i] & 0x1F))
            return i - 1;
    }
    return 0; // error
}

SCL_STATIC_API_BEFORE_TYPE
void make_head(SHLInplaceContext* ctx, uint8_t token)
{
    uint8_t dt = get_data_type(token);
    ctx->current = (char*)ctx->current + (cvt_table_wi[dt] >> 7);
    ctx->flags = (cvt_table_wi[dt] & (SCL_PRIVATE_FLAG_HAS_HEAD | SCL_PRIVATE_FLAG_HAS_DATA | SCL_PRIVATE_FLAG_SWAP_TEMP))
                 | (token == SHLT_StateError ? SCL_PRIVATE_FLAG_STATE_ERR : 0);
    if (cvt_table_wi[dt] & SCL_PRIVATE_FLAG_HAS_HEAD && token != SHLT_Eof) {
        uint8_t tokoff = (cvt_table_wi[dt] & 0x0F); // :(
        *(uint8_t*)ctx->head = (token - tokoff) | init_mask_table[dt];
    }
}


SCL_STATIC_API_BEFORE_TYPE
uint16_t ascii_strlen(const uint8_t* data)
{
    const uint8_t* start = data;
    while (*data >= 0x20 && *data <= 0x7F)
        ++data;
    return data - start;
}

SCL_STATIC_API_BEFORE_TYPE
const ARDMiniRule data_types[] = {
    // 0x8 | 1 -> SHLI_CVT_DTE_3
    // 0x8 | 2 -> SHLI_CVT_DTE_12
    // 0x8 | 0 -> SHLI_CVT_DTE_21
    // 0x0 | 1 -> SHLI_CVT_DTE_3
    // 0x0 | 2 -> SHLI_CVT_DTE_11
    // 0x0 | 0 -> SHLI_CVT_DTE_21

    //      [headoff: bit[2]] [tokoff: bit[4]]          [dte: bit[2]]
    {8 | 1, (0 << 6) |               (SHLI_CVT_DTE_3_OFFSET << 2)  | SHLI_CVT_DTE_3 },
    {8 | 2, (1 << 6) |               (SHLI_CVT_DTE_12_OFFSET << 2) | SHLI_CVT_DTE_12},
    {8 | 0, (2 << 6) |               (SHLI_CVT_DTE_21_OFFSET << 2) | SHLI_CVT_DTE_21},
    {0 | 1, (0 << 6) |               (SHLI_CVT_DTE_3_OFFSET << 2)  | SHLI_CVT_DTE_3 },
    {0 | 2, (0 << 6) |               (SHLI_CVT_DTE_11_OFFSET << 2) | SHLI_CVT_DTE_11},
    {0 | 0, (2 << 6) |               (SHLI_CVT_DTE_21_OFFSET << 2) | SHLI_CVT_DTE_21}
};


SCL_CPP_ANON_NAMESPACE_END



SCL_EXPORT_API_BEFORE_TYPE
uint8_t SCL_EXPORT_API_AFTER_TYPE shli_get_header_size(uint8_t data_type)
{
    return (cvt_table_wi[data_type] & SCL_PRIVATE_FLAG_HAS_HEAD ? 1 : 0)
            + (cvt_table_wi[data_type] & SCL_PRIVATE_FLAG_SWAP_TEMP ? 1 : 0);
}

SCL_EXPORT_API_BEFORE_TYPE
SHLInplaceContext SCL_EXPORT_API_AFTER_TYPE shli_make_context(void* buffer, uint8_t token)
{
    SHLInplaceContext ctx;
    ctx.current = ctx.head = buffer;
    make_head(&ctx, token);
    ctx.temp = 0;
    return ctx;
}

SCL_EXPORT_API_BEFORE_TYPE
void SCL_EXPORT_API_AFTER_TYPE shli_reuse_context(SHLInplaceContext* ctx, uint8_t token)
{
    ctx->head = ctx->current;
    ctx->temp = 0;
    make_head(ctx, token);
}

SCL_EXPORT_API_BEFORE_TYPE
void SCL_EXPORT_API_AFTER_TYPE shli_next(SHLInplaceContext* ctx, uint8_t chr)
{
    uint8_t next = chr;
    if (ctx->flags & SCL_PRIVATE_FLAG_SWAP_TEMP) {
        next = ctx->temp;
        ctx->temp = chr;
    }
    if (ctx->flags & SCL_PRIVATE_FLAG_HAS_DATA) {
        *(uint8_t*)ctx->current = next;
        ctx->current = (char*)ctx->current + 1;
    }
}

SCL_EXPORT_API_BEFORE_TYPE
void SCL_EXPORT_API_AFTER_TYPE shli_end(SHLInplaceContext* ctx)
{
    if (ctx->flags & SCL_PRIVATE_FLAG_SWAP_TEMP) {
        *(uint8_t*)ctx->current = ctx->temp;
        uint16_t size = (char*)ctx->current - (char*)ctx->head - 1;
        *((uint8_t*)ctx->head) |= (size >> 5) & 0x78;
        *((uint8_t*)ctx->head + 1) = size & 0xFF;
        ctx->current = (char*)ctx->current + 1;
    }
    if (ctx->flags & (SCL_PRIVATE_FLAG_HAS_HEAD | SCL_PRIVATE_FLAG_HAS_DATA))
        *(uint8_t*)ctx->current = 0;
    if (ctx->flags & SCL_PRIVATE_FLAG_STATE_ERR) { // StateError
        *((uint8_t*)ctx->current) = 0x01;
        *((uint8_t*)ctx->current + 1) = 0;
    }
}

SCL_EXPORT_API_BEFORE_TYPE
SHLITokenInfo SCL_EXPORT_API_AFTER_TYPE shli_parse_data(void* phead)
{
    uint8_t head = *(const uint8_t*)phead;
    uint8_t idx = (head < 0x80) + (head < 0x20) + (head & 0x08);
    uint8_t rule = ard_find_rule((const uint8_t*)&data_types, idx).value;
    SHLITokenInfo info;
    info.head = phead;
    uint8_t headoff = (rule >> 6) & 0x3;
    info.data = (uint8_t*)phead + headoff;
    uint8_t tokoff = (rule >> 2) & 0xF;
    uint8_t dte = (rule) & 0x3;
    info.data_type = dte;
    info.token = tokoff + (dte != SHLI_CVT_DTE_3 ? head & 0x7 : 0);
    if (dte < 3)
        info.size = ascii_strlen((const uint8_t*)info.data) + !dte;
    else {
        info.size =
                ((uint16_t)(head & (0xF << 3)) << 5)
                | *((const uint8_t*)phead + 1);
    }
    return info;
}

SCL_EXPORT_API_BEFORE_TYPE
SHLITokenInfo SCL_EXPORT_API_AFTER_TYPE shli_next_token(SHLITokenInfo prev)
{
    return shli_parse_data((char*)prev.data + prev.size);
}

SCL_EXPORT_API_BEFORE_TYPE
void SCL_EXPORT_API_AFTER_TYPE shli_parse_inplace(void* buffer, size_t size)
{
    SHLParseState state = 0;
    SHLInplaceContext ictx = shli_make_context(buffer, SHLT_None);
    for (size_t i = 0; i < size; i++)
    {
        SHLParseResult result = shl_parse_next(state, ((char*)buffer)[i]);
        if (shl_get_token(result.state) != shl_get_token(state)) {
            shli_end(&ictx);
            shli_reuse_context(&ictx, shl_get_token(result.state));
        }
        if (result.parsed) {
            shli_next(&ictx, result.parsed);
        }
        state = result.state;
    }
    shli_end(&ictx);
}

SCL_EXPORT_API_BEFORE_TYPE
SHLInplaceContext SCL_EXPORT_API_AFTER_TYPE shli_continue(SHLITokenInfo token)
{
    SHLInplaceContext ctx;
    ctx.head = token.head;
    ctx.current = (char*)token.data + token.size;
    ctx.flags = (cvt_table_wi[token.data_type] & (SCL_PRIVATE_FLAG_HAS_DATA | SCL_PRIVATE_FLAG_SWAP_TEMP))
                | (token.token == SHLT_StateError ? SCL_PRIVATE_FLAG_STATE_ERR : 0);
    return ctx;
}
