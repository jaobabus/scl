#ifndef TOKENCOLORS_H
#define TOKENCOLORS_H

#include <scl/token.h>


static const char* shlt_none        = "\033[0m";
static const char* shlt_eof        = "\033[0m";
static const char* shlt_stateerror = "\033[38;5;196m";
static const char* shlt_whitespace = "\033[0m";
static const char* shlt_flag       = "\033[38;5;64m";
static const char* shlt_longflag   = "\033[38;5;148m";
static const char* shlt_variable   = "\033[38;5;198m";
static const char* shlt_stringsq   = "\033[38;5;208m";
static const char* shlt_stringdq   = "\033[38;5;208m";
static const char* shlt_subprogram = "\033[38;5;214m";
static const char* shlt_word   = "\033[38;5;253m";


static inline
const char* shlt_get_color(SHLToken token)
{
    switch (token)
    {
    case SHLT_None:
        return shlt_none;
    case SHLT_Eof:
        return shlt_eof;
    case SHLT_Whitespace:
        return shlt_whitespace;
    case SHLT_Word:
        return shlt_word;
    case SHLT_String:
        return shlt_stringsq;
    case SHLT_SubProgram:
        return shlt_subprogram;
    case SHLT_Flag:
        return shlt_flag;
    case SHLT_FlagEqual:
        return shlt_longflag;
    case SHLT_Variable:
        return shlt_variable;
    case SHLT_StateError:
        return shlt_stateerror;
    default:
        return shlt_stateerror;
    }
}


#endif // TOKENCOLORS_H
