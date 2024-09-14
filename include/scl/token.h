#ifndef SHLTOKEN_H
#define SHLTOKEN_H

#include <stddef.h>
#include <stdint.h>

#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Inplace format v2.0
 *
 * data type 1.1:
 *   [0: bit[4]] [0: bit] [type: bit[3]]
 *
 * data type 1.2:
 *   [0: bit[4]] [1: bit] [type: bit[3]] [data: ascii]
 *
 * data type 3:
 *   [data: ascii]
 *
 * data type 2.1:
 *   [1: bit] [size: bit[4]] [type: bit[3]] [size: byte] [data: byte[size]]
 *
 * ascii = byte[(while in range 0x20..0x7E)]
 *
 */

/// data type 1.2:
//    [0: bit[4]] [0: bit]          [type: bit[3] = 1..7]
#define SHLI_CVT_DTE_11 0

/// data type 1.3:
//    [0: bit[4]] [1: bit]          [type: bit[3]]                   [data: ascii]
#define SHLI_CVT_DTE_12 1

/// data type 3:
//    [data: ascii]
#define SHLI_CVT_DTE_3 2

/// data type 2.1:
//    [1: bit] [size[8:12]: bit[4]] [type: bit[3]] [size[0:8]: byte] [data: byte[size]]
#define SHLI_CVT_DTE_21 3


typedef enum {
    /** Default value of token
     */
    SHLT_None = 0,

#define SHLI_CVT_DTE_11_OFFSET SHLT_Eof

    /** Eof marker ('\0')
     */
    SHLT_Eof = 1,

    /** State error marker
     */
    SHLT_StateError = 2,

    /** Whitespace marker (' ', '\t')
     */
    SHLT_Whitespace = 3,

    /** FlagEqual (/=/) after SHLT_Flag
     */
    SHLT_FlagEqual = 4,

#define SHLI_CVT_DTE_12_OFFSET SHLT_Flag

    /** Flag (/-([\-A-Za-z0-9]\w*)/)
     */
    SHLT_Flag = 5,

    /** Variable (/$(\w+)/)
     *
     *  now not implemented
     */
    SHLT_Variable = 6,

#define SHLI_CVT_DTE_21_OFFSET SHLT_String

    /** String (double/single quoted string)
     */
    SHLT_String = 7,

    /** Subprogram (simplify apostrophe quoted string)
     *
     * now not implemented
     */
    SHLT_SubProgram = 8,

#define SHLI_CVT_DTE_3_OFFSET SHLT_Word

    /** Argument (/([\x20-\x7E]+)/)
     */
    SHLT_Word = 9,

    /** Escape (/\\.+/)
     */
    SHLT_EscapeStart = 13,
    SHLT_EscapeHex = 14,
    SHLT_EscapeHex2 = 15,

} SHLToken;


/** shl_str_token
 *
 *  @param token Token
 *  @return Name of token
 */
const char* shl_str_token(SHLToken token);


#ifdef __cplusplus
}
#endif

#endif // SHLTOKEN_H
