#ifndef SHLINPLACE_H
#define SHLINPLACE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/** SHLITokenInfo
 *
 *  Inplace packed token info
 */
typedef struct {
    /// head
    /// Pointer to token head
    void* head;

    /// data
    /// Pointer to token data
    void* data;

    /// token
    /// @see SHLToken
    uint8_t token;

    /// data_type
    /// One of SHLI_CVT_DTE_*
    uint8_t data_type;

    /// size
    /// Size of token data
    uint16_t size;
} SHLITokenInfo;


/** SHLInplaceContext
 *
 *  Inplace pack token context
 */
typedef struct {
    /// head
    /// Pointer to last token head
    void* head;

    /// current
    /// Pointer to current byte
    void* current;

    /// flags
    /// Combination of SCL_PRIVATE_FLAG_*
    uint8_t flags;

    /// temp
    /// Field for support types with 2 byte head
    char temp;
} SHLInplaceContext;


// ------------------------ Writing ------------------------

/** shli_make_context
 *
 *  @param buffer Pointer to buffer where will be inplaced token
 *  @param token  @see SHLToken
 *  @return Valid context
 */
SHLInplaceContext shli_make_context(void* buffer, uint8_t token);


/** shli_reuse_context
 *
 *  @param ctx   Pointer to context
 *  @param token @see SHLToken
 */
void shli_reuse_context(SHLInplaceContext* ctx, uint8_t token);


/** shli_next
 *
 *  @param ctx Pointer to context
 *  @param chr next char to inplaced to token
 */
void shli_next(SHLInplaceContext* ctx, uint8_t chr);


/** shli_end
 *
 * Complete token
 *
 * @param ctx Pointer to context
 */
void shli_end(SHLInplaceContext* ctx);

// ------------------------ ------- ------------------------


// ------------------------ Reading ------------------------

/** shli_get_header_size
 *
 *  @param data_type SHLI_CVT_DTE_*
 *  @return header size
 */
uint8_t shli_get_header_size(uint8_t data_type);


/** shli_parse_data
 *
 *  @param head Pointer to token
 *  @return Parsed info
 */
SHLITokenInfo shli_parse_data(void* head);


/** shli_next_token
 *
 *  Skip current and place next token
 *
 * @param prev Previous token
 */
void shli_next_token(SHLITokenInfo* prev);

// ------------------------ ------- ------------------------


// ------------------------- Using -------------------------

/** shli_parse_inplace
 *
 *  Parse inplace command line
 *
 *  @param buffer command line buffer
 *  @param size   sizeof of buffer
 */
void shli_parse_inplace(void* buffer, size_t size);

/** shli_continue
 *
 *  ???
 *
 *  @param ctx   Pointer to context
 *  @param token Previous token
 */
void shli_continue(SHLInplaceContext* ctx, SHLITokenInfo token);

// ------------------------- ----- -------------------------


#ifdef __cplusplus
}
#endif

#endif // SHLINPLACE_H
