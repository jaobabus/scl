#include <scl/parse.h>
#include <scl/error.h>

#include <ctype.h>


#define true 1
#define false 0
#define not !
#define and &&
#define or ||

typedef uint8_t bool;


static bool iswhitespace(char c);
static bool hex2octet(char c);

typedef SHLParseResult StateContext;
static bool parse_next_none       (StateContext* context, char next);
static bool parse_next_eof        (StateContext* context, char next);
static bool parse_next_whitespace (StateContext* context, char next);
static bool parse_next_argument   (StateContext* context, char next);
static bool parse_next_string_dq  (StateContext* context, char next);
static bool parse_next_string_sq  (StateContext* context, char next);
static bool parse_next_subprogram (StateContext* context, char next);
static bool parse_next_flag       (StateContext* context, char next);
static bool parse_next_long_flag  (StateContext* context, char next);
static bool parse_next_variable   (StateContext* context, char next);
static bool parse_next_reserved   (StateContext* context, char next);
static bool parse_next_error_state(StateContext* context, char next);
static bool parse_next_not_inited (StateContext* context, char next);

typedef bool(*ParseNext)(StateContext* context, char next);
static ParseNext const parse_table[16] = {
    parse_next_none,
    parse_next_eof,
    parse_next_error_state,
    parse_next_whitespace,
    parse_next_reserved,
    parse_next_flag,
    parse_next_long_flag,
    parse_next_variable,
    parse_next_string_sq,
    parse_next_string_dq,
    parse_next_subprogram,
    parse_next_argument,
    parse_next_reserved,
    parse_next_reserved,
    parse_next_reserved,
    parse_next_reserved,
};



SHLParseResult shl_parse_next(SHLParseState prev_state, char next)
{
    StateContext ctx = {prev_state, 0};
    while (parse_table[(uint8_t)(ctx.state >> 4)](&ctx, next));
    return ctx;
}


static void reset_token(StateContext* context, uint8_t type)
{
    context->state = type << 4;
}

static void done_token(StateContext* context)
{
    reset_token(context, SHLT_None);
}

static void parsed(StateContext* context, char parsed)
{
    context->parsed = parsed;
}

static void error(StateContext* context, uint8_t err)
{
    context->state = (SHLT_StateError << 4) | (uint8_t)(err);
}

static void set_state(StateContext* context, uint8_t state)
{
    context->state = (context->state & 0xF0) | state;
}


static bool parse_next_none(StateContext* context, char next)
{
    if (next == '\0') {
        reset_token(context, SHLT_EOF);
        return false;
    }
    else if (next == ' ' or next == '\t') {
        reset_token(context, SHLT_Whitespace);
        return true;
    }
    else if (next == '"') {
        reset_token(context, SHLT_StringDQ);
        return true;
    }
    else if (next == '\'') {
        reset_token(context, SHLT_StringSQ);
        return true;
    }
    else if (next == '`') {
        reset_token(context, SHLT_SubProgram);
        return true;
    }
    else if (next == '-') {
        reset_token(context, SHLT_Flag);
        return true;
    }
    else if (next == '$') {
        reset_token(context, SHLT_Variable);
        return true;
    }
    else {
        reset_token(context, SHLT_Argument);
        return true;
    }
}

static bool parse_next_not_inited(StateContext* context, char next)
{
    done_token(context);
    return true;
}

static bool parse_next_eof(StateContext* context, char next)
{
    return false;
}

static bool parse_next_whitespace(StateContext* context, char next)
{
    if (next == ' ' or next == '\t') {
        parsed(context, next);
        return false;
    }
    done_token(context);
    return true;
}

static bool parse_next_argument(StateContext* context, char next)
{
    if (next == '"' or next == '\'' or next == '`' or next == '$' or next == ' ' or next == '\t' or next == '\0') {
        done_token(context);
        return true;
    }
    parsed(context, next);
    return false;
}

static bool parse_next_string_dq(StateContext* context, char next)
{
    if (shl_get_state(context->state) == 0) {
        set_state(context, shl_get_state(context->state) + 1);
        return false;
    }
    if (next == '"') {
        done_token(context);
        return false;
    }
    if (next == '\0') {
        error(context, SHLE_UnexpectedEOF);
        return false;
    }
    parsed(context, next);
    return false;
}

static bool parse_next_string_sq(StateContext* context, char next)
{
    if (shl_get_state(context->state) == 0) {
        set_state(context, shl_get_state(context->state) + 1);
        return false;
    }
    if (next == '\'') {
        done_token(context);
        return false;
    }
    if (next == '\0') {
        error(context, SHLE_UnexpectedEOF);
        return false;
    }
    parsed(context, next);
    return false;
}

static bool parse_next_subprogram(StateContext* context, char next)
{
    if (shl_get_state(context->state) == 0) {
        set_state(context, shl_get_state(context->state) + 1);
        return false;
    }
    if (next == '`') {
        done_token(context);
        return false;
    }
    if (next == '\0') {
        error(context, SHLE_UnexpectedEOF);
        return false;
    }
    parsed(context, next);
    return false;
}

static bool parse_next_flag(StateContext* context, char next)
{
    if (shl_get_state(context->state) == 0) {
        set_state(context, shl_get_state(context->state) + 1);
        return false;
    }
    if (next == '-') {
        reset_token(context, SHLT_LongFlag);
        return true;
    }
    if (next == '\0' && shl_get_state(context->state) < 2) {
        error(context, SHLE_UnexpectedEOF);
        return false;
    }
    if (not isalnum(next)) {
        if (shl_get_state(context->state) < 2)
            error(context, SHLE_UnexpectedChar);
        else
            done_token(context);
        return true;
    }
    if (shl_get_state(context->state) < 2)
        set_state(context, shl_get_state(context->state) + 1);
    parsed(context, next);
    return false;
}

static bool parse_next_long_flag(StateContext* context, char next)
{
    if (shl_get_state(context->state) == 0) {
        set_state(context, shl_get_state(context->state) + 1);
        return false;
    }
    if (next == '\0' and shl_get_state(context->state) < 2) {
        error(context, SHLE_UnexpectedEOF);
        return false;
    }
    if (not isalnum(next) && next != '-') {
        if (shl_get_state(context->state) < 2)
            error(context, SHLE_UnexpectedChar);
        else
            done_token(context);
        return true;
    }
    if (shl_get_state(context->state) < 2)
        set_state(context, shl_get_state(context->state) + 1);
    parsed(context, next);
    return false;
}

static bool parse_next_variable(StateContext* context, char next)
{
    if (shl_get_state(context->state) == 0) {
        set_state(context, shl_get_state(context->state) + 1);
        return false;
    }
    if (next == '\0' and shl_get_state(context->state) < 2) {
        error(context, SHLE_UnexpectedEOF);
        return false;
    }
    if (not isalnum(next)) {
        if (shl_get_state(context->state) < 2)
            error(context, SHLE_UnexpectedChar);
        else
            done_token(context);
        return true;
    }
    if (shl_get_state(context->state) < 2)
        set_state(context, shl_get_state(context->state) + 1);
    parsed(context, next);
    return false;
}

static bool parse_next_reserved(StateContext* context, char next)
{
    return false;
}

static bool parse_next_error_state(StateContext* context, char next)
{
    return false;
}

static bool iswhitespace(char c) {
    return c == ' ' or c == '\t';
}

static bool hex2octet(char c) {
    if (c >= '0' and c <= '9')
        return c - '0';
    else if (c >= 'a' and c <= 'f')
        return c - 'a' + 10;
    else if (c >= 'A' and c <= 'F')
        return c - 'A' + 10;
    return 0;
}

SHLToken shl_get_token(SHLParseState state)
{
    return (SHLToken)(state >> 4);
}

uint8_t shl_get_state(SHLParseState state)
{
    return state & 0xF;
}





















