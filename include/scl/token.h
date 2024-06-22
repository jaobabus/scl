#ifndef SHLTOKEN_H
#define SHLTOKEN_H

#include "scl/inplace.h"


#ifdef __cplusplus
extern "C" {
#endif


/** Inplace format v2.0
 *
 * data type 1.1:
 *   [0: bit[8]]
 *
 * data type 1.2:
 *   [0: bit[4]] [0: bit] [type: bit[3]]
 *
 * data type 1.3:
 *   [0: bit[4]] [1: bit] [type: bit[3]] [data: ascii]
 *
 * data type 2.1:
 *   [1: bit] [type: bit[3]] [size: bit[12]] [data: byte[size]]
 *
 * data type 3:
 *   [data: ascii]
 *
 * ascii = byte[(while in range 0x20..0x7E)]
 *
 */


/// data type 1.1:
//    [0: bit[8]]
#define SHLI_CVT_DTE_11 0


/// data type 1.2:
//    [0: bit[4]] [0: bit]          [type: bit[3] = 1..7]
#define SHLI_CVT_DTE_12 1


/// data type 1.3:
//    [0: bit[4]] [1: bit]          [type: bit[3]]                   [data: ascii]
#define SHLI_CVT_DTE_13 2


/// data type 2.1:
//    [1: bit] [size[8:12]: bit[4]] [type: bit[3]] [size[0:8]: byte] [data: byte[size]]
#define SHLI_CVT_DTE_21 3


/// data type 3:
//    [data: ascii]
#define SHLI_CVT_DTE_3 4



typedef enum
{
#define SHLI_CVT_OFFSET_DTE_11 SHLT_EOF

    /** Default value of token
     */
    SHLT_None = 0,

    /** Eof marker ('\0')
     */
    SHLT_EOF,

    /** State error marker
     */
    SHLT_StateError,

#define SHLI_CVT_OFFSET_DTE_12 (SHLT_Whitespace - 1) // avoid ambiguous for EOF and whitespace

    /** Whitespace marker (' ', '\t')
     */
    SHLT_Whitespace,

#define SHLI_CVT_OFFSET_DTE_13 SHLT_Flag

    /** Flag (/-([A-Za-z]\w*)/)
     */
    SHLT_Flag,

    /** Long flag (/--([A-Za-z][\w\-]*)/)
     */
    SHLT_LongFlag,

    /** Variable (/$(\w+)/)
     */
    SHLT_Variable,

#define SHLI_CVT_OFFSET_DTE_21 SHLT_StringSQ

    /** String (single quoted string)
     */
    SHLT_StringSQ,

    /** String (double quoted string)
     */
    SHLT_StringDQ,

    /** Subprogram (simplify apostrophe quoted string)
     */
    SHLT_SubProgram,

#define SHLI_CVT_OFFSET_DTE_3  SHLT_Argument

    /** Argument (/([\x20-\x7E]+)/)
     */
    SHLT_Argument,

} SHLToken;


enum SHLError {
    SHLE_NoError,
    SHLE_NotImplemented,
    SHLE_UnknownState,
    SHLE_UnknownEscape,
    SHLE_EscapeError,
    SHLE_UnexpectedToken,
    SHLE_UnexpectedEOF,
    SHLE_UnallowedChar,
    SHLE_TooLongArgument
};

const char* shl_str_token(SHLToken type);




#ifdef __cplusplus
}
#endif


#endif // SHLTOKEN_H
