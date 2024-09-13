#ifndef SHLPARSE_H
#define SHLPARSE_H

#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif

// [token: bit[4]] [state: bit[4]]
typedef uint8_t SHLParseState;

typedef struct
{
    SHLParseState state;
    uint8_t parsed;
} SHLParseResult;

SHLToken shl_get_token(SHLParseState state);
SHLParseResult shl_parse_next(SHLParseState prev_state, char next);

#ifdef __cplusplus
}
#endif

#endif // SHLPARSE_H
