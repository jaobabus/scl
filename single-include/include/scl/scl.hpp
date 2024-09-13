#ifndef _HOME_JABUS_REPOS_PROJECTS_SHLEXER_SCL_SRC_SINGLEINCLUDE_CPP_SINGLE_HEADER
# define _HOME_JABUS_REPOS_PROJECTS_SHLEXER_SCL_SRC_SINGLEINCLUDE_CPP_SINGLE_HEADER

#define SCL_SINGLE_HEADER 

       

#include <stdint.h>



extern "C" {



/* Ascii Rule Dict
 *
 * Dict structure:
 *  Dict:
 *  0: [hia: bit] [key: bit[7]]
 *  1: [value: bit[8]]
 *  2: [id_after: bit[8]]
 *
 *  0:
 *    0[0] bit is hia (Has Id After)
 *    0[1] byte is ascii7 key
 *
 *    if not key is EOF
 *
 *  2: has if hia
 *    0[0] byte is id after command
 *
 */

#define ARD_EOF 0
#define ARD_EOT 0x80
#define ARD_HIA_MASK 0x80
#define ARD_HIA 0x80
#define ARD_KEY_MASK 0x7F


typedef struct {
    uint8_t value;
    uint8_t id_after;
} ARDRule;


typedef struct {
    uint8_t key;
    uint8_t value;
} ARDMiniRule;



ARDRule ard_find_rule(const uint8_t* data, uint8_t key);



}
       

#include <stdbool.h>
#include <iso646.h>



#define SCL_EXPORT_API_BEFORE_TYPE inline
#define SCL_EXPORT_API_AFTER_TYPE 
#define SCL_STATIC_API_BEFORE_TYPE inline
#define SCL_CPP_ANON_NAMESPACE_START namespace {
#define SCL_CPP_ANON_NAMESPACE_END }






SCL_EXPORT_API_BEFORE_TYPE
ARDRule SCL_EXPORT_API_AFTER_TYPE ard_find_rule(const uint8_t* data, uint8_t key)
{
    ARDRule rule = {0, 0};
    while (1)
    {
        uint8_t head = *data;
        uint8_t hkey = head & ARD_KEY_MASK;
        rule.value = data[1];
        rule.id_after = 0;
        if (head & ARD_HIA_MASK)
            rule.id_after = (++data)[1];
        if (hkey == key || hkey == 0)
            break;
        data += 2;
    }
    return rule;
}

#define SHLTOKEN_H 

#include <stddef.h>



#define ERROR_H 

#include <stddef.h>

enum ESCLError {
    SCLE_NoError = 0,

    /** Memory limit reached
     */
    SCLE_MemoryLimitError,

    /** Feature not implemented
     */
    SCLE_NotImplemented,

    /** Lexer errors section
     */
    SHLE_LexerErrorsStart,

    /** Unknown lexer state
     *
     *  internal error or bug
     */
    SHLE_UnknownState = SHLE_LexerErrorsStart,

    /** Invalid or not implemented escape sequence
     */
    SHLE_UnknownEscape,

    /** Invalid escape sequence
     */
    SHLE_EscapeError,

    /** Unexpected EOF char
     */
    SHLE_UnexpectedEOF,

    /** Unexpected char
     */
    SHLE_UnexpectedChar,

    /** Unallowed char
     */
    SHLE_UnallowedChar,

    /** Lexer errors section end
     */
    SHLE_LexerErrorsEnd = SHLE_UnallowedChar,

    /** Command errors section
     */
    SCLE_CommandErrorsStart,

    /** Command token must be SHLT_Word
     */
    SCLE_InvalidCommandToken = SCLE_CommandErrorsStart,

    /** After '--flag=' expected common argument
     */
    SCLE_CommandNotFound,

    /** After '--flag=' expected common argument
     */
    SCLE_ExpectedFlagValue,

    /** Command errors section end
     */
    SCLE_CommandErrorsEnd = SCLE_ExpectedFlagValue,

    /** User errors section
     */
    SCLE_UserErrorsStart = 64,

    /** User errors section end
     */
    SCLE_UserErrorsEnd = 255,
};

typedef enum ESCLError SCLError;



extern "C" {


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

const char* shl_str_token(SHLToken type);


}






SCL_EXPORT_API_BEFORE_TYPE
const char* SCL_EXPORT_API_AFTER_TYPE shl_str_token(SHLToken type)
{
    switch (type)
    {
    case SHLT_None:
        return "None";
    case SHLT_Eof:
        return "EOF";
    case SHLT_Whitespace:
        return "Whitespace";
    case SHLT_Word:
        return "Word";
    case SHLT_String:
        return "String";
    case SHLT_SubProgram:
        return "SubProgram";
    case SHLT_Flag:
        return "Flag";
    case SHLT_FlagEqual:
        return "FlagEqual";
    case SHLT_Variable:
        return "Variable";
    case SHLT_StateError:
        return "StateError";
    case SHLT_EscapeStart:
        return "EscapeStart";
    case SHLT_EscapeHex:
        return "EscapeHex";
    case SHLT_EscapeHex2:
        return "EscapeHex2";
    default:
        return "Unknown";
    }
}

#define SHLPARSE_H 



extern "C" {


// [token: bit[4]] [state: bit[4]]
typedef uint8_t SHLParseState;

typedef struct
{
    SHLParseState state;
    uint8_t parsed;
} SHLParseResult;

SHLToken shl_get_token(SHLParseState state);
SHLParseResult shl_parse_next(SHLParseState prev_state, char next);


}




#include <stdio.h>


#define TRACE_VIA_PRINTF(a...) 
//#define TRACE_VIA_PRINTF(a...) a


SCL_CPP_ANON_NAMESPACE_START

// -- ARD-Table 'ParseRules' --
// THIS BLOCK AUTOGENERATED
// Do NOT change it
// {

/* Rules mapping
 * 
 * Token:
 *   [escape: bit[2]] [token: bit[4]] [state: bit[2]]
 *
 * Key:
 *   [char: bit[7]]
 *
 * Value:
 *   [data: bit[2]] [ret: bit] [0: bit] [0: bit] [0: bit[3]]
 *   [data: bit[2]] [ret: bit] [0: bit] [1: bit] [0: bit] [state: bit[2]]
 *   [data: bit[2]] [ret: bit] [1: bit] [token: bit[4]]
 *
 */
#define SCL_PRIVATE_DataAction(a) ((a) << 6)
#define SCL_PRIVATE_NoStore SCL_PRIVATE_DataAction(0)
#define SCL_PRIVATE_Store8 SCL_PRIVATE_DataAction(1)
#define SCL_PRIVATE_Store3 SCL_PRIVATE_DataAction(2)
#define SCL_PRIVATE_Store4 SCL_PRIVATE_DataAction(3)
#define SCL_PRIVATE_Return 0x20
#define SCL_PRIVATE_Token(token) (0x10 | (token))
#define SCL_PRIVATE_State(state,switch) (0x08 | (state) | ((switch) ? 0x4 : 0x0))

SCL_STATIC_API_BEFORE_TYPE
const ARDMiniRule token_id_map[] = {
    {(0x40 | SHLT_None << 2) | 0, 0x00},
    {(0x40 | SHLT_Eof << 2) | 0, 0x0C},
    {(0x40 | SHLT_StateError << 2) | 0, 0x55},
    {(0x40 | SHLT_Whitespace << 2) | 0, 0x06},
    {(0x40 | SHLT_FlagEqual << 2) | 0, 0x23},
    {(0x40 | SHLT_Flag << 2) | 0, 0x1B},
    {(0x40 | SHLT_Flag << 2) | 3, 0x1F},
    {(0x40 | SHLT_Variable << 2) | 0, 0x25},
    {(0x40 | SHLT_Variable << 2) | 3, 0x27},
    {(0x40 | SHLT_String << 2) | 0, 0x29},
    {(0x40 | SHLT_String << 2) | 1, 0x2D},
    {(0x40 | SHLT_String << 2) | 2, 0x33},
    {(0x40 | SHLT_String << 2) | 3, 0x39},
    {(0x40 | SHLT_Word << 2) | 0, 0x0F},
    {(0x40 | SHLT_Word << 2) | 1, 0x19},
    {(0x40 | SHLT_EscapeStart << 2) | 0, 0x3B},
    {(0x40 | SHLT_EscapeHex << 2) | 0, 0x51},
    {(0x40 | SHLT_EscapeHex2 << 2) | 0, 0x53},
    {0, 0x55}
};

SCL_STATIC_API_BEFORE_TYPE
uint8_t get_id_by_token_state(uint8_t state)
{
    if (state & 0xC0)
        state = (SHLT_EscapeStart - 1 + (state >> 6)) << 2;
    return ard_find_rule((const uint8_t*)token_id_map, 0x40 | state).value;
}

SCL_STATIC_API_BEFORE_TYPE
uint8_t get_id_by_token(uint8_t token, uint8_t state)
{
    return get_id_by_token_state((token << 2) | state);
}

SCL_STATIC_API_BEFORE_TYPE
const char* dbg_name_of_rulemap(uint8_t id)
{
    static char buf[1024];
    switch(id) {
    case 0x00: return "SHLT_None[0]";
    case 0x06: return "SHLT_Whitespace[0]";
    case 0x0C: return "SHLT_Eof[0]";
    case 0x0F: return "SHLT_Word[0]";
    case 0x19: return "SHLT_Word[1]";
    case 0x1B: return "SHLT_Flag[0]";
    case 0x1F: return "SHLT_Flag[3]";
    case 0x23: return "SHLT_FlagEqual[0]";
    case 0x25: return "SHLT_Variable[0]";
    case 0x27: return "SHLT_Variable[3]";
    case 0x29: return "SHLT_String[0]";
    case 0x2D: return "SHLT_String[1]";
    case 0x33: return "SHLT_String[2]";
    case 0x39: return "SHLT_String[3]";
    case 0x3B: return "SHLT_EscapeStart[0]";
    case 0x51: return "SHLT_EscapeHex[0]";
    case 0x53: return "SHLT_EscapeHex2[0]";
    case 0x55: return "SHLT_StateError[0]";
    default: sprintf(buf, "Unknown 0x%02hhX", id); return buf;
    }
}


SCL_STATIC_API_BEFORE_TYPE
const uint8_t rules[] = {
/* 00 */ // Token.NotInited[0]
  /* 00 */ '$',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_Variable),
  /* 02 */ '-',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_Flag),
  /* 04 */ 0x00,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_Word),
/* 06 */ // Token.Whitespace[0]
  /* 06 */ ' ',
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
  /* 08 */ '\t',
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
  /* 0A */ 0x00,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_None),
/* 0C */ // Token.Eof[0]
  /* 0C */ 0x00 | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_Eof) | SCL_PRIVATE_Return,
  /* -- */ 0x00, // const
/* 0F */ // Token.Word[0]
  /* 0F */ '\"',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_String),
  /* 11 */ '\'',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_String),
  /* 13 */ ' ',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_Whitespace),
  /* 15 */ '\\',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(1, false) | SCL_PRIVATE_Return,
  /* 17 */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
/* 19 */ // Token.Word[1]
  /* 19 */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_State(0, false) | SCL_PRIVATE_Return,
/* 1B */ // Token.Flag[0]
  /* 1B */ '-',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(3, false) | SCL_PRIVATE_Return,
  /* 1D */ 0x00,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(3, false) | SCL_PRIVATE_Return,
/* 1F */ // Token.Flag[3]
  /* 1F */ '=',
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Token(SHLT_FlagEqual) | SCL_PRIVATE_Return,
  /* 21 */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
/* 23 */ // Token.FlagEqual[0]
  /* 23 */ 0x00,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_None),
/* 25 */ // Token.Variable[0]
  /* 25 */ 0x00,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(3, false) | SCL_PRIVATE_Return,
/* 27 */ // Token.Variable[3]
  /* 27 */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
/* 29 */ // Token.String[0]
  /* 29 */ '\"',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(1, false) | SCL_PRIVATE_Return,
  /* 2B */ '\'',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(2, false) | SCL_PRIVATE_Return,
/* 2D */ // Token.String[1]
  /* 2D */ '\\',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_EscapeStart) | SCL_PRIVATE_Return,
  /* 2F */ '\"',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(3, false) | SCL_PRIVATE_Return,
  /* 31 */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
/* 33 */ // Token.String[2]
  /* 33 */ '\\',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_EscapeStart) | SCL_PRIVATE_Return,
  /* 35 */ '\'',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_State(3, false) | SCL_PRIVATE_Return,
  /* 37 */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
/* 39 */ // Token.String[3]
  /* 39 */ 0x00,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_None),
/* 3B */ // Escape.Start[0]
  /* 3B */ 'x',
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Token(SHLT_EscapeHex),
  /* 3D */ 'r' | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ '\r', // const
  /* 40 */ 'b' | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ '\b', // const
  /* 43 */ 'e' | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ '\033', // const
  /* 46 */ 'a' | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ '\a', // const
  /* 49 */ 'v' | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ '\v', // const
  /* 4C */ 'n' | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ '\n', // const
  /* 4F */ 0x00,
  /* -- */ SCL_PRIVATE_Store8 | SCL_PRIVATE_Return,
/* 51 */ // Escape.Hex[0]
  /* 51 */ 0x00,
  /* -- */ SCL_PRIVATE_Store4 | SCL_PRIVATE_Token(SHLT_EscapeHex2),
/* 53 */ // Escape.Hex2[0]
  /* 53 */ 0x00,
  /* -- */ SCL_PRIVATE_Store4 | SCL_PRIVATE_Return,
/* 55 */ // Token.StateError[0]
  /* 55 */ 0x00 | ARD_HIA,
  /* -- */ SCL_PRIVATE_NoStore | SCL_PRIVATE_Return,
  /* -- */ 0x01, // const
};

// }
// -- ARD-Table --


SCL_STATIC_API_BEFORE_TYPE
const char* parse_value(uint8_t s)
{
    static char buffer[1024];
    size_t size = 0;
    uint8_t d = s & 0xC0;
    size += snprintf(buffer + size, 1024 - size, "%s",
                    &"NoStore\0Store8\0\0Store3\0\0Store4\0"[(d >> 6) * 8]);
    if (s & SCL_PRIVATE_Return)
        size += sprintf(buffer + size, ", Return");
    if (s & SCL_PRIVATE_Token(0))
        size += sprintf(buffer + size, ", token=%s",
                        shl_str_token(shl_get_token(s << 2)));
    else if (s & SCL_PRIVATE_State(0, 0)) {
        size += sprintf(buffer + size, ", state=%d",
                        s & 0x3);
        if (s & 0x40)
        size += sprintf(buffer + size, ", switch");
    }
    return buffer;
}

SCL_STATIC_API_BEFORE_TYPE
const char* parse_state(uint8_t s)
{
    static char buffer[1024];
    size_t size = 0;
    size += snprintf(buffer + size, 1024 - size, "%s",
                    &"NoEscape\0\0StartEsc\0\0HexEscape\0OctEscape\0"[(s >> 6) * 10]);
    size += sprintf(buffer + size, ", token=%s",
                    shl_str_token(shl_get_token(s)));
    size += sprintf(buffer + size, ", state=%d",
                    s & 0x3);
    return buffer;
}

SCL_STATIC_API_BEFORE_TYPE
uint8_t parse_next(SHLParseResult* res, uint8_t key)
{
    TRACE_VIA_PRINTF(printf("parse_next(%s, '%c'(%d))\n", parse_state(res->state), key, key);)
    if (key == 0) {
        res->state = SHLT_Eof << 2;
        return true;
    }
    if ((res->state & 0x3) == 3 and
            (shl_get_token(res->state) == SHLT_Flag or shl_get_token(res->state) == SHLT_Variable))
    {
        if (!(key >= 'A' and key <= 'Z' or key >= '0' and key <= '9' or key >= 'a' and key <= 'z' or key == '_' or key == '='))
            res->state = SHLT_None << 2;
    }
    uint8_t off = get_id_by_token_state(res->state);
    ARDRule rule = ard_find_rule((const uint8_t*)rules + off, key);
    TRACE_VIA_PRINTF(printf("%s['%c'(%d)] = *0x%02hhX = (%s, put=%d)\n",
                            dbg_name_of_rulemap(off), key, key, off, parse_value(rule.value), rule.id_after);)

    if (res->state & 0xC0)
    {
        if (rule.value & SCL_PRIVATE_Return) {
            TRACE_VIA_PRINTF(printf("End escape\n");)
            res->state = res->state & 0x3F;
        }
        rule.value |= SCL_PRIVATE_Return;
    }
    if (rule.value & SCL_PRIVATE_Token(0))
    {
        uint8_t token = (rule.value & 0xF);
        if (token < SHLT_EscapeStart) {
            TRACE_VIA_PRINTF(printf("token(%s -> %s)\n", shl_str_token(shl_get_token(res->state)), shl_str_token(shl_get_token(rule.value << 2)));)
            res->state = (token << 2);
        }
        else {
            TRACE_VIA_PRINTF(printf("escape=%s\n", shl_str_token(shl_get_token(rule.value << 2)));)
            res->state = res->state & 0x3F | ((token - SHLT_EscapeStart + 1) << 6);
        }
    }
    else if (rule.value & SCL_PRIVATE_State(0, 0))
    {
        TRACE_VIA_PRINTF(printf("state(%d -> %d)\n", res->state & 3, rule.value & 3);)
        res->state = res->state & ~0x3 | rule.value & 0x3;
    }

    uint8_t action = rule.value >> 6;
    switch (action)
    {
    case SCL_PRIVATE_NoStore >> 6:
        break;
    case SCL_PRIVATE_Store8 >> 6:
        res->parsed = key;
        break;
    case SCL_PRIVATE_Store3 >> 6:
        res->parsed = key - '0';
        break;
    case SCL_PRIVATE_Store4 >> 6:
        res->parsed = key - '0';
        break;
    }
    return (rule.value & SCL_PRIVATE_Return);
}


SCL_CPP_ANON_NAMESPACE_END


SCL_EXPORT_API_BEFORE_TYPE
SHLToken SCL_EXPORT_API_AFTER_TYPE shl_get_token(SHLParseState state)
{
    return (SHLToken)((state >> 2) & 0xF);
}

SCL_EXPORT_API_BEFORE_TYPE
SHLParseResult SCL_EXPORT_API_AFTER_TYPE shl_parse_next(SHLParseState prev_state, char next)
{
    SHLParseResult result = {prev_state};
    while (!parse_next(&result, next));
    return result;
}


#define SHLINPLACE_H 




extern "C" {


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




}





#include <stddef.h>




SCL_CPP_ANON_NAMESPACE_START


#define SCL_PRIVATE_FLAG_HAS_HEAD 0x80
#define SCL_PRIVATE_FLAG_HAS_DATA 0x40
#define SCL_PRIVATE_FLAG_SWAP_TEMP 0x20
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

#define EXECUTOR_H 


#define SCL_COMMAND_H 


#define SCL_ARGUMENT_H 



extern "C" {


typedef struct
{
    void (*destruct)(void* obj); // Constructor must be called in parse()
    uint8_t (*parse)(const void* opaque, void* obj, SHLITokenInfo token);
    size_t (*completes)(
        const void* opaque, char* buffer, size_t buffer_size, const char* token, size_t size);
    uint16_t obj_size;
    uint16_t obj_offset;
} SCLArgumentDescriptor;


}




extern "C" {


#define SCLC_FLAG_VA_ARG 0x01

typedef struct
{
    uint8_t error;
    uint8_t token;
} SCLExecuteError;

typedef struct
{
    SCLExecuteError (*execute)(void* obj_buffer, const uint16_t* flags, size_t size);
    const SCLArgumentDescriptor* const* arguments;
    const void* const* arguments_opaques;
    uint8_t arg_count;
    uint8_t flags;
    uint8_t name_size;
    uint8_t pd[5];
} SCLCommandDescriptor;


}

template<size_t name_size>
struct SCLCommandDescriptorWithName
{
    SCLCommandDescriptor base;
    char name_continue[name_size];
};

static_assert(sizeof(SCLCommandDescriptorWithName<8>) - sizeof(void*) * 3 - 8 == 8, "Align error");




extern "C" {


typedef struct
{
    void* (*alloc)(size_t size);
    void (*release)(void* ptr, size_t);
} SCLAllocator;

SCLExecuteError scl_execute_inplace(const SCLCommandDescriptor* cmd,
                                    const SCLAllocator* alloc,
                                    char* cmdline,
                                    size_t size);

SCLExecuteError scl_find_and_execute_inplace(const SCLCommandDescriptor * const *cmd,
                                             size_t count,
                                             const SCLAllocator *alloc,
                                             char *cmdline, size_t size);


}




#include <string.h>



SCL_CPP_ANON_NAMESPACE_START


SCL_STATIC_API_BEFORE_TYPE
bool is_argument(uint8_t token)
{
    return token == SHLT_Word
            or token == SHLT_String
            or token == SHLT_String;
}

SCL_STATIC_API_BEFORE_TYPE
void* alloc_arguments_buffer(const SCLCommandDescriptor* cmd, const SCLAllocator* alloc)
{
    size_t size = 1;
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        size_t sz = cmd->arguments[i]->obj_offset + cmd->arguments[i]->obj_size;
        if (sz > size)
            size = sz;
    }
    void* buffer = alloc->alloc(size);
    return buffer;
}

SCL_STATIC_API_BEFORE_TYPE
void free_arguments_buffer(const SCLCommandDescriptor* cmd, const SCLAllocator* alloc, void* buffer)
{
    size_t size = 1;
    for (uint8_t i = 0; i < cmd->arg_count; i++)
    {
        if (cmd->arguments[i]->destruct)
            cmd->arguments[i]->destruct((char*)buffer + cmd->arguments[i]->obj_offset);
        size_t sz = cmd->arguments[i]->obj_offset + cmd->arguments[i]->obj_size;
        if (sz > size)
            size = sz;
    }
    alloc->release(buffer, size);
}

SCL_STATIC_API_BEFORE_TYPE
SCLExecuteError parse_arguments(const SCLCommandDescriptor* cmd,
                                void* arguments,
                                char* cmdline, size_t size)
{
    SHLITokenInfo token = shli_parse_data(cmdline);
    uint16_t flags_buffer[16];
    uint8_t flags_top = 0;
    uint8_t token_index = 1, arg_index = 0;
    while (token.token != SHLT_Eof
           and token.token != SHLT_StateError
           and arg_index < cmd->arg_count)
    {
        while (token.token == SHLT_Whitespace)
        {
            token = shli_next_token(token);
            token_index++;
        }
        if (token.token == SHLT_Flag)
        {
            if (flags_top >= sizeof(flags_buffer) / sizeof(flags_buffer[0]))
            {
                SCLExecuteError error;
                error.token = token_index;
                error.error = SCLE_MemoryLimitError;
                return error;
            }
            flags_buffer[flags_top++] = (char*)token.head - cmdline;
            SHLITokenInfo next = shli_next_token(token);
        }
        else if (is_argument(token.token))
        {
            void* arg = (char*)arguments + cmd->arguments[arg_index]->obj_offset;
            SCLError err = (SCLError)cmd->arguments[arg_index]->parse((cmd->arguments_opaques ? cmd->arguments_opaques[arg_index] : NULL), arg, token);
            if (err != SCLE_NoError)
            {
                SCLExecuteError error;
                error.token = token_index;
                error.error = err;
                return error;
            }
            arg_index++;
        }
        else
        {
            SCLExecuteError error;
            error.token = token_index;
            error.error = SCLE_NotImplemented;
            return error;
        }
        token = shli_next_token(token);
        token_index++;
    }
    return cmd->execute(arguments, flags_buffer, flags_top);
}


SCL_CPP_ANON_NAMESPACE_END



SCL_EXPORT_API_BEFORE_TYPE
SCLExecuteError SCL_EXPORT_API_AFTER_TYPE
    scl_execute_inplace(const SCLCommandDescriptor* cmd,
                        const SCLAllocator* alloc,
                        char* cmdline, size_t size)
{
    void* arguments = alloc_arguments_buffer(cmd, alloc);
    SCLExecuteError error = parse_arguments(cmd, arguments, cmdline, size);
    free_arguments_buffer(cmd, alloc, arguments);
    return error;
}


SCL_EXPORT_API_BEFORE_TYPE
SCLExecuteError SCL_EXPORT_API_AFTER_TYPE
    scl_find_and_execute_inplace(const SCLCommandDescriptor * const *cmd,
                                 size_t count,
                                 const SCLAllocator *alloc,
                                 char *cmdline, size_t size)
{
    shli_parse_inplace(cmdline, size);
    SHLITokenInfo name = shli_parse_data(cmdline);
    SHLITokenInfo token = shli_next_token(name);
    SCLExecuteError err = {SCLE_CommandNotFound, 0};
    while (token.token == SHLT_Whitespace)
        token = shli_next_token(token);
    for (size_t i = 0; i < count; ++i)
    {
        const char* cmd_name = (const char*)(cmd[i] + 1);
        if (strlen(cmd_name) == name.size and memcmp(name.data, cmd_name, name.size) == 0)
            return scl_execute_inplace(cmd[i], alloc, (char*)token.head, size - ((char*)token.head - cmdline));
    }
    return err;
}

       


#include <stddef.h>




extern "C" {




typedef struct
{
    void (*write)(void* opaque, const void* data, size_t size);
    void (*execute)(void* opaque, void* buffer, size_t size);

    const uint8_t* colors;

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



}



#include <memory.h>


// ------------------- For debug ------------------- //

#define coprintf(...) 

// ------------------------------------------------- //

SCL_CPP_ANON_NAMESPACE_START

// -- ARD-Table 'EscapeTable' --
// THIS BLOCK AUTOGENERATED
// Do NOT change it
// {

/*
 * EscapeRule:
 *   [0: bit[3]] [ret: bit] [action: bit[4]]
 *
 */

#define SCLCB_PRIVATE_Return 0x10

typedef enum { 
    NoAction = 0,
    CursorUp = 1,
    CursorDown = 2,
    CursorRight = 3,
    CursorLeft = 4,
    DeleteBefore = 5,
    DeleteAfter = 6,
    Execute = 7,
    Skip = 8,
    Fallback = 9,
    Insert = 10,
} Action;

SCL_STATIC_API_BEFORE_TYPE
const uint8_t escape_rules[] = {
/* 00 */ // Root escapes
  /* 00 */ '\e' | ARD_HIA,
  /* -- */ NoAction | SCLCB_PRIVATE_Return,
  /* -- */ 0x18, // 2lvl escapes
  /* 03 */ '\x7F',
  /* -- */ DeleteBefore | SCLCB_PRIVATE_Return, // Backspace
  /* 05 */ '\n',
  /* -- */ Execute | SCLCB_PRIVATE_Return, // Execute command
  /* 07 */ '\r',
  /* -- */ Execute | SCLCB_PRIVATE_Return, // Execute command
  /* 09 */ 0x00,
  /* -- */ Insert | SCLCB_PRIVATE_Return,
/* 0B */ // lbrace escapes
  /* 0B */ 'A',
  /* -- */ CursorUp | SCLCB_PRIVATE_Return, // Key arrow up
  /* 0D */ 'B',
  /* -- */ CursorDown | SCLCB_PRIVATE_Return, // Key arrow down
  /* 0F */ 'C',
  /* -- */ CursorRight | SCLCB_PRIVATE_Return, // Key arrow right
  /* 11 */ 'D',
  /* -- */ CursorLeft | SCLCB_PRIVATE_Return, // Key arrow left
  /* 13 */ '3' | ARD_HIA,
  /* -- */ DeleteAfter | SCLCB_PRIVATE_Return, // Key delete
  /* -- */ 0x1D, // Skip rules
  /* 16 */ 0x00,
  /* -- */ Fallback | SCLCB_PRIVATE_Return,
/* 18 */ // 2lvl escapes
  /* 18 */ '[' | ARD_HIA,
  /* -- */ NoAction | SCLCB_PRIVATE_Return,
  /* -- */ 0x0B, // lbrace escapes
  /* 1B */ 0x00,
  /* -- */ Fallback | SCLCB_PRIVATE_Return,
/* 1D */ // Skip rules
  /* 1D */ '~',
  /* -- */ Skip | SCLCB_PRIVATE_Return,
  /* 1F */ 0x00,
  /* -- */ Fallback | SCLCB_PRIVATE_Return,
};

// }
// -- ARD-Table --



SCL_STATIC_API_BEFORE_TYPE
uint8_t translate(uint8_t ch)
{
    if (ch == '\e')
        return '^';
    if (ch < 0x20)
        return '?';
    return ch;
}

//static
//void buffer_insert(uint8_t* buffer, size_t buf_size, size_t index, char ch);
SCL_STATIC_API_BEFORE_TYPE
void buffer_insert(uint8_t* buffer, size_t buf_size, size_t index, char ch)
{
    if (index < buf_size)
        for (size_t i = buf_size - 1; i > index; i--)
            buffer[i] = buffer[i - 1];
    buffer[index] = ch;
}

//static
//void buffer_pop(uint8_t* buffer, size_t buf_size, size_t index);
SCL_STATIC_API_BEFORE_TYPE
void buffer_pop(uint8_t* buffer, size_t buf_size, size_t index)
{
    for (size_t i = index + 1; i < buf_size; i++)
        buffer[i - 1] = buffer[i];
}


typedef enum
{
    NoSequence = 0,
    SeqCursorLeft = 1,
    SeqCursorRight = 2,
    SeqCursorUp = 3,
    SeqCursorDown = 4,
    SeqColor256 = 5,
} Sequence;

typedef struct
{
    const char* start;
    char end;
} SequenceInfo;

SCL_STATIC_API_BEFORE_TYPE const
SequenceInfo sequences[] = {
    {0, 0},
    {"[", 'D'},
    {"[", 'C'},
    {"[", 'A'},
    {"[", 'B'},
    {"[38;5;", 'm'},
};

SCL_STATIC_API_BEFORE_TYPE
void write_sequence1_8(SCLConsoleBufferContext* ctx, Sequence seq, uint8_t arg)
{
    if (not seq)
        return;
    char buffer[16] = {"\e[000m"};
    char* it = buffer + 1;
    const char* start = sequences[seq].start;
    while (*start)
        *it++ = *start++;
    uint8_t len = (arg > 0 ? arg >= 10 ? arg >= 100 ? 3 : 2 : 1 : 0);
    it += len;
    char* ait = it;
    while (arg) {
        *--ait = arg % 10 + '0';
        arg /= 10;
    }
    *it++ = sequences[seq].end;
    ctx->write(ctx->opaque, buffer, it - buffer);
}

SCL_STATIC_API_BEFORE_TYPE
void safe_update(SCLConsoleBufferContext* ctx, const void* data, size_t size, SHLParseState prev_state, uint8_t off, const char* completes)
{
    coprintf("Safe write at '%s'",
             std::string((const char*)data, (const char*)data + size).c_str());
    ctx->write(ctx->opaque, "\e7", 2);
    const char* it = (const char*)data;
    for (size_t i = 0; i < size; i++) {
        if (ctx->colors) {
            prev_state = shl_parse_next(prev_state, *it).state;
            write_sequence1_8(ctx, SeqColor256, ctx->colors[shl_get_token(prev_state)]);
        }
        ctx->write(ctx->opaque, it++, 1);
    }
    if (ctx->colors) {
        write_sequence1_8(ctx, SeqColor256, 15);
    }
    ctx->write(ctx->opaque, "\e8", 2);
}

typedef struct
{
    const void* data;
    size_t size;
} UpdateRegion;

typedef struct
{
    Sequence seq;
    uint8_t arg;
} SequenceAfter;

#define UPDATE_BITMAP16 ( (0 << NoAction) | (0 << CursorUp) | (0 << CursorDown) | (0 << CursorRight) | (0 << CursorLeft) | (1 << DeleteBefore) | (1 << DeleteAfter) | (0 << Execute) | (0 << Skip) | (1 << Fallback) | (1 << Insert) )

SCL_STATIC_API_BEFORE_TYPE
bool sclcb_on_char_next(SCLConsoleBufferContext* ctx, char ch)
{
    if (ctx->escape_index)
        ctx->buffer[ctx->size + ctx->escape_stack_size++] = ch;
    ARDRule rule = ard_find_rule(&escape_rules[ctx->escape_index], ch);
    ctx->escape_index = rule.id_after;
    uint16_t old_cur = ctx->cursor;
    SequenceAfter seq_after = {NoSequence, 0};
    uint8_t action = rule.value & 0xF;
    switch (action)
    {
    case NoAction:
        coprintf("NoAction\n");
        break;

    case Insert:
        coprintf("Insert %c(%d)\n", ch, ch);
        buffer_insert(ctx->buffer, ctx->size + 1, ctx->cursor, ch);
        seq_after.seq = SeqCursorRight;
        ctx->size++;
        ctx->cursor++;
        break;

    case Skip:
        coprintf("Skip\n");
        break;

    case Fallback: {
        const uint8_t* escape_stack = ctx->buffer + ctx->size;
        uint16_t old_cur = ctx->cursor;
        coprintf("Reached Fallback, flush escape stack: '%s'\n",
                 [&] { std::string s; for (int i = 0; i < ctx->escape_stack_size; i++) s += translate(escape_stack[i]); return s; }().c_str());
        for (uint8_t i = 0; i < ctx->escape_stack_size; i++) {
            uint8_t tch = translate(escape_stack[i]);
            buffer_insert(ctx->buffer, ctx->size, ctx->cursor, tch);
            ctx->size++;
            ctx->cursor++;
        }
        seq_after.seq = SeqCursorRight;
        seq_after.arg = ctx->size - old_cur;
        ctx->escape_stack_size = 0;
    }   break;

    case CursorUp:
        coprintf("CursorUp\n");
        break;

    case CursorDown:
        coprintf("CursorDown\n");
        break;

    case CursorRight:
        coprintf("CursorRight %d < %d\n",
                 ctx->cursor, ctx->size);
        if (ctx->cursor < ctx->size)
        {
            ctx->cursor++;
            seq_after.seq = SeqCursorRight;
        }
        break;

    case CursorLeft:
        coprintf("CursorLeft %d > 0\n",
                 ctx->cursor);
        if (ctx->cursor > 0)
        {
            ctx->cursor--;
            seq_after.seq = SeqCursorLeft;
        }
        break;

    case DeleteAfter:
        coprintf("DeleteAfter %d < %d\n",
                 ctx->cursor, ctx->size);
        if (ctx->cursor < ctx->size)
        {
            buffer_pop(ctx->buffer, sizeof(ctx->buffer), ctx->cursor);
            ctx->size--;
            ctx->buffer[ctx->size] = ' ';
        }
        break;

    case DeleteBefore:
        coprintf("DeleteBefore %d != 0\n",
                 ctx->cursor);
        if (ctx->cursor != 0)
        {
            ctx->cursor--;
            buffer_pop(ctx->buffer, ctx->size + 1, ctx->cursor);
            ctx->size--;
            ctx->buffer[ctx->size] = ' ';
            seq_after.seq = SeqCursorLeft;
        }
        break;

    case Execute:
        coprintf("Execute '%s'\n",
                 std::string((char*)ctx->buffer, (char*)ctx->buffer + ctx->size).c_str());
        ctx->write(ctx->opaque, "\r\n", 2);
        ctx->buffer[ctx->size] = '\0';
        ctx->execute(ctx->opaque, &ctx->buffer[0], ctx->size);
        memset(ctx->buffer, 0, ctx->buffer_size);
        ctx->size = 0;
        ctx->cursor = 0;
        break;

    default:
        coprintf("Unknown instruction '%d'\n",
                 rule.value);
        break;
    }
    if (UPDATE_BITMAP16 & (1 << action)) {
        SHLParseState state = 0;
        uint8_t* start = ctx->buffer + old_cur;
        if (ctx->colors) {
            for (const uint8_t* it = ctx->buffer; it < start; it++)
                state = shl_parse_next(state, *it).state;
        }
        char completes[32] = {"Tab1\nTab2\nTab3"};
        safe_update(ctx, start, (ctx->size > old_cur ? ctx->size - old_cur : 1), state, ctx->cursor, completes);
    }
    write_sequence1_8(ctx, seq_after.seq, seq_after.arg);
    return !(rule.value & SCLCB_PRIVATE_Return);
}

SCL_CPP_ANON_NAMESPACE_END


SCL_EXPORT_API_BEFORE_TYPE
SCLConsoleBufferContext SCL_EXPORT_API_AFTER_TYPE sclcb_make_context(void* buffer, uint16_t size,
                                                                     void* opaque,
                                                                     void (*write)(void*, const void*, size_t),
                                                                     void (*execute)(void*, void*, size_t),
                                                                     const uint8_t* colors)
{
    SCLConsoleBufferContext ctx;
    ctx.buffer = (uint8_t*)buffer;
    ctx.buffer_size = size;
    ctx.size = ctx.cursor = 0;
    ctx.escape_stack_size = 0;
    ctx.opaque = opaque;
    ctx.write = write;
    ctx.execute = execute;
    ctx.colors = colors;
    return ctx;
}

SCL_EXPORT_API_BEFORE_TYPE
void SCL_EXPORT_API_AFTER_TYPE sclcb_on_char(SCLConsoleBufferContext* ctx, char ch)
{
    while (sclcb_on_char_next(ctx, ch));
}

SCL_EXPORT_API_BEFORE_TYPE
void SCL_EXPORT_API_AFTER_TYPE sclcb_clear(SCLConsoleBufferContext* ctx)
{
    ctx->size = ctx->cursor = 0;
}

#define SCL_ARGUMENT_HPP 



#define UTILS_H 

#include <stddef.h>





struct StringView
{
    const char* _data;
    size_t _size;

    const char* data() const noexcept { return _data; }
    size_t size() const noexcept { return _size; }

    const char* begin() const noexcept { return data(); }
    const char* end() const noexcept { return data() + size(); }
};

template<typename... A>
struct MyTuple
{
    static_assert(sizeof...(A) * 0, "Error instantiation");
};

template<typename First, typename... Types>
struct MyTuple<First, Types...>
{
    using ThisContainer = MyTuple<First, Types...>;
    using NextContainer = MyTuple<Types...>;
    using type = First;
    static constexpr size_t next_offset = offsetof(ThisContainer, next);
    First value;
    NextContainer next;
};

template<>
struct MyTuple<>
{};

template<typename Tuple, size_t _offset>
struct TupleFinderContext
{
    using type = typename Tuple::type;
    using container = Tuple;
    static constexpr size_t offset = _offset;
};

template<typename Original, typename Context, size_t index>
struct TupleContainerFinderImpl
    : TupleContainerFinderImpl<Original,
                               TupleFinderContext<typename Context::container::NextContainer,
                                                  Context::offset + Context::container::next_offset>,
                               index - 1>
{
    static constexpr typename TupleContainerFinderImpl<
        Original,
        TupleFinderContext<typename Context::container::NextContainer,
                           Context::offset + Context::container::next_offset>,
        index - 1>::container::type&
    get(typename Context::container& tuple)
    {
        return TupleContainerFinderImpl<
            Original,
            TupleFinderContext<typename Context::container::NextContainer,
                               Context::offset + Context::container::next_offset>,
            index - 1>::get(tuple.next);
    }
};

template<typename Original, typename Context>
struct TupleContainerFinderImpl<Original, Context, 0>
{
    static constexpr size_t offset = Context::offset;
    using container = typename Context::container;
    static constexpr typename Context::type& get(typename Context::container& tuple)
    {
        return tuple.value;
    }
};

template<typename Tuple, size_t index>
using FindTupleContainer = TupleContainerFinderImpl<Tuple, TupleFinderContext<Tuple, 0>, index>;

template<size_t index, typename Tuple>
constexpr size_t get_offset_of()
{
    using info = FindTupleContainer<Tuple, index>;
    return info::offset;
}

template<size_t index, typename Tuple>
constexpr typename FindTupleContainer<Tuple, index>::container::type& get(Tuple& tuple)
{
    using info = FindTupleContainer<Tuple, index>;
    return info::get(tuple);
}

template<size_t _index, typename Tuple>
struct tuple_element
{
    using type = typename FindTupleContainer<Tuple, _index>::container::type;
    static constexpr size_t index = _index;
    static constexpr size_t offset = FindTupleContainer<Tuple, index>::offset;
};

// /////////////////////////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/76302127/c11-equivalent-of-stdapply-plus-how-to-do-it-on-member-functions
// INVOKE -> invoke
// +apply(&A::f, A*, ...)
// std::tuple -> MyTuple
// fix make_integer_sequence
#include <utility>

namespace std14 {
template<typename T, T... Ints>
struct integer_sequence
{
    typedef T value_type;
    static constexpr std::size_t size() { return sizeof...(Ints); }
};

template<std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

template<typename T, std::size_t N, T... Is>
struct st_make_integer_sequence : st_make_integer_sequence<T, N - 1, N - 1, Is...>
{};

template<typename T, T... Is>
struct st_make_integer_sequence<T, 0, Is...>
{
    using sequence = integer_sequence<T, Is...>;
};

template<typename T, std::size_t N>
using make_integer_sequence = typename st_make_integer_sequence<T, N>::sequence;

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<typename... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;
} // namespace std14

template<class F, class... Args>
constexpr auto invoke(F&& f,
                      Args&&... args) -> decltype(std::forward<F>(f)(std::forward<Args...>(args)...))
{
    return std::forward<F>(f)(std::forward<Args...>(args)...);
}

template<typename Fptr, typename Class, typename... Args>
constexpr auto invoke(Fptr fptr, Class&& obj, Args&&... args)
    -> decltype((std::forward<Class>(obj).*fptr)(std::forward<Args>(args)...))
{
    return (std::forward<Class>(obj).*fptr)(std::forward<Args>(args)...);
}

template<typename F, typename Tuple, std::size_t... I>
constexpr auto apply_impl(F&& f, Tuple& t, std14::index_sequence<I...>) -> decltype(f(get<I>(t)...))
{
    return f(get<I>(t)...);
    // return invoke(std::forward<F>(f), get<I>(t)...);
}

template<typename F, typename Cls, typename Tuple, std::size_t... I>
constexpr auto apply_impl(F&& f, Cls* ptr, Tuple& t, std14::index_sequence<I...>)
    -> decltype((ptr->*f)(get<I>(t)...))
{
    return (ptr->*f)(get<I>(t)...);
    // return invoke(std::forward<F>(f), std::forward<Cls>(*ptr), get<I>(t)...);
}

template<typename F, typename... Args>
constexpr auto apply(F&& f, MyTuple<Args...>& tup)
    -> decltype(apply_impl(std::forward<F>(f), tup, std14::make_index_sequence<sizeof...(Args)>{}))
{
    return apply_impl(std::forward<F>(f), tup, std14::make_index_sequence<sizeof...(Args)>{});
}

template<typename F, typename Cls, typename... Args>
constexpr auto apply(F&& f, Cls* ptr, MyTuple<Args...>& tup)
    -> decltype(apply_impl(
        std::forward<F>(f), ptr, tup, std14::make_index_sequence<sizeof...(Args)>{}))
{
    return apply_impl(std::forward<F>(f), ptr, tup, std14::make_index_sequence<sizeof...(Args)>{});
}
// /////////////////////////////////////////////////////////////////////////////////////////////////////




#define SHL_STR2(a...) #a
#define SHL_STR(a...) SHL_STR2(a)
#define SHL_EXPAND(a...) a
#define SHL_CONCAT2(a,b) a ##b
#define SHL_CONCAT(a,b) SHL_CONCAT2(a, b)
#define SHL_SIZEOF_IMPL(a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,n,...) n
#define SHL_SIZEOF(a...) SHL_SIZEOF_IMPL(a, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define SHL_JOIN(j,a...) SHL_EXPAND(SHL_CONCAT(SHL_JOIN_, SHL_SIZEOF(a))(j, a))

/*
for i in range(1, 12 + 1):
    d = f"#define SHL_JOIN_{i}(j, {', '.join(f'a{j}' for j in range(1, i + 1))})"
    v = ' j '.join(f"a{j}" for j in range(1, i + 1))
    print(f"{d} {v}")
*/

#define SHL_JOIN_1(j,a1) a1
#define SHL_JOIN_2(j,a1,a2) a1 j a2
#define SHL_JOIN_3(j,a1,a2,a3) a1 j a2 j a3
#define SHL_JOIN_4(j,a1,a2,a3,a4) a1 j a2 j a3 j a4
#define SHL_JOIN_5(j,a1,a2,a3,a4,a5) a1 j a2 j a3 j a4 j a5
#define SHL_JOIN_6(j,a1,a2,a3,a4,a5,a6) a1 j a2 j a3 j a4 j a5 j a6
#define SHL_JOIN_7(j,a1,a2,a3,a4,a5,a6,a7) a1 j a2 j a3 j a4 j a5 j a6 j a7
#define SHL_JOIN_8(j,a1,a2,a3,a4,a5,a6,a7,a8) a1 j a2 j a3 j a4 j a5 j a6 j a7 j a8
#define SHL_JOIN_9(j,a1,a2,a3,a4,a5,a6,a7,a8,a9) a1 j a2 j a3 j a4 j a5 j a6 j a7 j a8 j a9
#define SHL_JOIN_10(j,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) a1 j a2 j a3 j a4 j a5 j a6 j a7 j a8 j a9 j a10
#define SHL_JOIN_11(j,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) a1 j a2 j a3 j a4 j a5 j a6 j a7 j a8 j a9 j a10 j a11
#define SHL_JOIN_12(j,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) a1 j a2 j a3 j a4 j a5 j a6 j a7 j a8 j a9 j a10 j a11 j a12






#define SCLA_MAKE_NAME(name) SHL_CONCAT(argument_, name)


template<typename T>
class DefaultArgument
{
public:
    uint8_t parse(T& value, SHLITokenInfo token) const noexcept;
    static size_t completes(char* buffer, size_t buffer_size, const char* token, size_t size) noexcept { return 0; }

};


template<typename Derrived, typename Type>
class TypedArgument : public DefaultArgument<Type>
{
public:
    using type = Type;

public:
    static uint8_t sc_parse(const void* th, void* obj, SHLITokenInfo token) noexcept
    {
        return static_cast<const Derrived*>(th)->parse(*(Type*)obj, token);
    }
    static size_t sc_completes(const void* th, char* buffer, size_t buffer_size, const char* token, size_t size) noexcept
    {
        return static_cast<const Derrived*>(th)->completes(buffer, buffer_size, token, size);
    }
};


template<typename T>
struct SCLArgumentByType;

template<typename T>
using SCLArgumentByTypeT = typename SCLArgumentByType<T>::type;

#define SCLA_SPECIALIZE_TYPE(Argument,Type) template<> struct SCLArgumentByType<Type> { using type = Argument; }






template<typename D, typename T>
class SCLArgumentWrapper;


template<typename Derrived, typename R, typename Arg>
struct SCLArgumentWrapper<Derrived, R (*)(Arg)>
{
public:
    using type = Arg;

public:
    static size_t sc_completes(const void* th, char* buffer, size_t buffer_size, const char* token, size_t size) noexcept { return 0; }

};





#define CMD_COMMAND_H 






#define SCLC_MAKE_NAME(name) SHL_CONCAT(command_, name)



template<typename T>
void destruct_object(void* ptr)
{
    static_cast<T*>(ptr)->~T();
}

template<typename Elem, typename ArgElem>
constexpr SCLArgumentDescriptor make_arg_desc()
{
    using Arg = typename ArgElem::type;
    return SCLArgumentDescriptor{destruct_object<typename Arg::type>,
                                 Arg::sc_parse,
                                 Arg::sc_completes,
                                 (uint16_t) (sizeof(typename Arg::type)),
                                 (uint16_t) Elem::offset};
}

template<typename Tuple, typename ArgsTuple, typename Indices>
struct SCLArgumentDescTable;

template<typename Tuple, typename ArgsTuple, size_t... indices>
struct SCLArgumentDescTable<Tuple, ArgsTuple, std14::index_sequence<indices...>>
{
    SCLArgumentDescriptor table[sizeof...(indices)] = {
        make_arg_desc<tuple_element<indices, Tuple>, tuple_element<indices, ArgsTuple>>()...};
    template<size_t index>
    constexpr const SCLArgumentDescriptor* get_ptr() const
    {
        return &table[index];
    }
    const SCLArgumentDescriptor* ptr_table[sizeof...(indices)] = {get_ptr<indices>()...};
};

template<typename TypedCommand, typename... Args>
class DefaultCommand
{
public:
    /* TypedCommand requires execute in Derrived class, please be careful
        * > static uint8_t Derrived::execute(const typename Args::type&...) noexcept;
        */
    static uint8_t execute(const typename Args::type&...) noexcept;
};

template<typename _Derrived, typename... Args>
class TypedCommand : public DefaultCommand<TypedCommand<_Derrived, Args...>, Args...>
{
public:
    using Derrived = _Derrived;
    using Tuple = MyTuple<typename Args::type...>;

public:
    static SCLExecuteError sc_execute(void* obj_buffer, const uint16_t* flags, size_t size)
    {
        auto err = apply(Derrived::execute, *static_cast<Tuple*>(obj_buffer));
        return SCLExecuteError{(uint8_t) err, 0};
    }

public:
    static constexpr SCLArgumentDescTable<Tuple,
                                          MyTuple<Args...>,
                                          std14::make_index_sequence<sizeof...(Args)>>
        sc_arg_descriptors_table{};

    static constexpr SCLCommandDescriptor sc_descriptor_base{sc_execute,
                                                             sc_arg_descriptors_table.ptr_table,
                                                             Derrived::sc_opaques,
                                                             (uint8_t) sizeof...(Args)};
};

template<size_t _size>
struct CStringContainer
{
    static constexpr size_t size = _size;
    char string[_size];
};

template<typename Derrived, typename... Args>
constexpr SCLArgumentDescTable<typename TypedCommand<Derrived, Args...>::Tuple,
                               MyTuple<Args...>,
                               std14::make_index_sequence<sizeof...(Args)>>
    TypedCommand<Derrived, Args...>::sc_arg_descriptors_table;
template<typename Derrived, typename... Args>
constexpr SCLCommandDescriptor TypedCommand<Derrived, Args...>::sc_descriptor_base;

// v rot manal clangd
template<typename... Ts>
using SCLC_TypedArgument = TypedArgument<Ts...>;


template<typename D, typename T>
struct ArgsCommandWrapper {};


template<typename Derrived, typename R, typename... Args>
struct ArgsCommandWrapper<Derrived, R (*)(Args...)> {

    using Tuple = MyTuple<Args...>;

    static constexpr SCLArgumentDescTable<Tuple,
                                          MyTuple<SCLArgumentByTypeT<Args>...>,
                                          std14::make_index_sequence<sizeof...(Args)>>
        sc_arg_descriptors_table{};

    static constexpr SCLCommandDescriptor sc_descriptor_base{&Derrived::sc_execute,
                                                             sc_arg_descriptors_table.ptr_table,
                                                             nullptr,
                                                             (uint8_t) sizeof...(Args)};
};


#define SCLC_DeclareCommand(name,args...) void SHL_CONCAT(SCLC_MAKE_NAME(name), _dummy_args)(args); struct SHL_CONCAT(SCLC_MAKE_NAME(name), _type) : ArgsCommandWrapper<SHL_CONCAT(SCLC_MAKE_NAME(name), _type), decltype(&SHL_CONCAT(SCLC_MAKE_NAME(name), _dummy_args))> { struct Body { uint8_t execute(); SHL_JOIN(;, args); }; static inline SCLExecuteError sc_execute(void* obj_buffer, const uint16_t* flags, size_t size) { return {reinterpret_cast<Body*>(obj_buffer)->execute(), 0}; } static constexpr SCLCommandDescriptorWithName<sizeof(#name)> sc_descriptor{sc_descriptor_base, {#name}}; } SCLC_MAKE_NAME(name); uint8_t SHL_CONCAT(SCLC_MAKE_NAME(name), _type)::Body::execute()



#endif // _HOME_JABUS_REPOS_PROJECTS_SHLEXER_SCL_SRC_SINGLEINCLUDE_CPP_SINGLE_HEADER
