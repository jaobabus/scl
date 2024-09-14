#ifndef SHLPARSE_H
#define SHLPARSE_H

#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif


/** SHLParseState
 *
 *  format: [escape: bit[2]] [token: bit[4]] [state: bit[2]]
 *
 *  @param escape @see SHLToken::SHLT_EscapeStart
 *  @param token  @see SHLToken
 *  @param state  Variant of token, see rules in parse.c
 */
typedef uint8_t SHLParseState;


/** SHLParseResult
 */
typedef struct
{
    /// State for this char
    SHLParseState state;

    /// Parsed char or '\0'
    uint8_t parsed;
} SHLParseResult;


/// shl_get_token
/// extract token from SHLParseState
SHLToken shl_get_token(SHLParseState state);

/** shl_parse_next
 *
 *  Select new lexem from SHLToken
 *
 *  @param prev_state Previous state of 0
 *  @param next       Next char
 *  @return next state and parsed char
 */
SHLParseResult shl_parse_next(SHLParseState prev_state, char next);


#ifdef __cplusplus
}
#endif

#endif // SHLPARSE_H
