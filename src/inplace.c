#include <scl/inplace.h>
#include <scl/parse.h>

#include <stddef.h>


#define true 1
#define false 0
#define not !
#define and &&
#define or ||

typedef uint8_t bool;


#define FLAG_HAS_HEAD  0x80
#define FLAG_HAS_DATA  0x40
#define FLAG_SWAP_TEMP 0x20 // for sizeof(header) == 2
#define FLAG_STATE_ERR 0x10


static const
uint8_t cvt_table_wi[] = {
    SHLI_CVT_OFFSET_DTE_11 | 0x00,
    SHLI_CVT_OFFSET_DTE_12 | FLAG_HAS_HEAD,
    SHLI_CVT_OFFSET_DTE_13 | FLAG_HAS_HEAD | FLAG_HAS_DATA,
    SHLI_CVT_OFFSET_DTE_21 | FLAG_HAS_HEAD | FLAG_HAS_DATA | FLAG_SWAP_TEMP,
    SHLI_CVT_OFFSET_DTE_3  | FLAG_HAS_DATA,
    16                     | 0x00
};


static const
uint8_t init_mask_table[] = {
    0x00,
    0x00,
    0x08,
    0x80,
    0x00,
    0x00
};

static
uint8_t get_data_type(uint8_t token)
{
    size_t i;
    SHLIDataInfo info;
    for (i = 1; i < sizeof(cvt_table_wi); i++) {
        if (token < (cvt_table_wi[i] & 0x1F))
            return i - 1;
    }
    return 0; // error
}

static
void make_head(SHLInplaceContext* ctx, uint8_t token)
{
    uint8_t dt = get_data_type(token);
    ctx->current = (char*)ctx->current + (cvt_table_wi[dt] >> 7);
    ctx->flags = (cvt_table_wi[dt] & (FLAG_HAS_DATA | FLAG_SWAP_TEMP))
                 | (token == SHLT_StateError ? FLAG_STATE_ERR : 0);
    if (cvt_table_wi[dt] & FLAG_HAS_HEAD) {
        uint8_t tokoff = (dt == SHLI_CVT_DTE_12 ? 1 : cvt_table_wi[dt] & 0x1F); // :(
        *(uint8_t*)ctx->head = (token - tokoff) | init_mask_table[dt];
    }
}


SHLInplaceContext shli_make_context(void* buffer, uint8_t token)
{
    SHLInplaceContext ctx;
    ctx.current = ctx.head = buffer;
    make_head(&ctx, token);
    ctx.temp = 0;
    return ctx;
}

void shli_reuse_context(SHLInplaceContext* ctx, uint8_t token)
{
    ctx->head = ctx->current;
    ctx->temp = 0;
    make_head(ctx, token);
}

void shli_next(SHLInplaceContext* ctx, uint8_t chr)
{
    uint8_t next = chr;
    if (ctx->flags & FLAG_SWAP_TEMP) {
        next = ctx->temp;
        ctx->temp = chr;
    }
    if (ctx->flags & FLAG_HAS_DATA) {
        *(uint8_t*)ctx->current = next;
        ctx->current = (char*)ctx->current + 1;
    }
}

void shli_end(SHLInplaceContext* ctx)
{
    if (ctx->flags & FLAG_SWAP_TEMP) {
        *(uint8_t*)ctx->current = ctx->temp;
        uint16_t size = (char*)ctx->current - (char*)ctx->head - 1;
        *((uint8_t*)ctx->head) |= (size >> 5) & 0x78;
        *((uint8_t*)ctx->head + 1) = size & 0xFF;
        ctx->current = (char*)ctx->current + 1;
    }
    if (ctx->flags == 0) // EOF
        *(uint8_t*)ctx->current = 0;
    if (ctx->flags & FLAG_STATE_ERR) { // StateError
        *((uint8_t*)ctx->current) = 0x01;
        *((uint8_t*)ctx->current + 1) = 0;
    }
}


static
uint16_t ascii_strlen(const uint8_t* data)
{
    const uint8_t* start = data;
    while (*data >= 0x20 && *data <= 0x7F)
        ++data;
    return data - start;
}

uint8_t shli_get_header_size(uint8_t data_type)
{
    return (cvt_table_wi[data_type] & FLAG_HAS_HEAD ? 1 : 0)
            + (cvt_table_wi[data_type] & FLAG_SWAP_TEMP ? 1 : 0);
}

SHLIDataInfo shli_parse_data(const void* phead)
{
    SHLIDataInfo info;
    uint8_t head = *(const uint8_t*)phead;
    info.header = phead;
    if (head < 0x20) {
        // data 1.*
        info.size = 0;
        if (head == 0)
            info.data_type = SHLI_CVT_DTE_11;
        else if (not (head & 0x08))
            info.data_type = SHLI_CVT_DTE_12;
        else {
            info.data_type = SHLI_CVT_DTE_13;
            info.size = ascii_strlen((const uint8_t*)phead + 1);
        }
        info.data = (const char*)phead
                    + (cvt_table_wi[info.data_type] & FLAG_HAS_HEAD ? 1 : 0);
        uint8_t tokoff = (info.data_type == SHLI_CVT_DTE_12 ? 1 : cvt_table_wi[info.data_type] & 0x1F); // :(
        info.token = tokoff + (head & 0x07);
    }
    else if (head > 0x7F) {
        // data 2.*
        info.data_type = SHLI_CVT_DTE_21;
        info.data = (const char*)phead + 2;
        info.size =
                ((uint16_t)*((const uint8_t*)phead + 0) << 5) & 0x0F00
                | *((const uint8_t*)phead + 1);
        info.token = (cvt_table_wi[info.data_type] & 0x1F) + (head & 0x07);
    }
    else {
        // data 3
        info.data_type = SHLI_CVT_DTE_3;
        info.data = (const char*)phead;
        info.size = ascii_strlen((const uint8_t*)phead);
        info.token = SHLI_CVT_OFFSET_DTE_3;
    }
    return info;
}

SHLIDataInfo shli_next_token(SHLIDataInfo prev)
{
    return shli_parse_data((const char*)prev.data + prev.size);
}

void shli_parse_inplace(void* buffer, size_t size)
{
    SHLParseState state = 0;
    SHLInplaceContext ictx = shli_make_context(buffer, SHLT_EOF);
    for (size_t i = 0; i < size; i++)
    {
        SHLParseResult result = shl_parse_next(state, ((const char*)buffer)[i]);
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





