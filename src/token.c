#include <scl/token.h>



/** Lexical
 *
 * BNF:
     # WARN <IGNORE_ESCAPE>: Warning escape '%s' ignored
     #

     <COMMENT>       ::= "#"
     <WORD>          ::= [a-z] | [A-Z] | [0-9] | "_"
     <SPETIAL>       ::= "%" | "*" | "+" | "," | "-" | "." | "/" | ":" | "=" | "?" | "@" | "~"
     <PIPE>          ::= "<" | ">"
     <PARENS>        ::= "(" | ")"
     <SEMICOLON>     ::= ";"
     <BASH_EVENT>    ::= "!"
     <VARIABLE_W>    ::= "$"
     <BASH_TEST>     ::= "[" | "]"
     <ESCAPE>        ::= "\\"
     <BASH_SUBS>     ::= "^"
     <SUBPROGRAM>    ::= "`"
     <BASH_UNK1>     ::= "{" | "}"
     <BASH_OR>       ::= "|"
     <BASH_AND>      ::= "&"

     <NOT_IMPL>      ::= <PARENS> | <BASH_EVENT> | <BASH_TEST> | <BASH_SUBS> | <SUBPROGRAM> | <BASH_UNK1> | <BASH_OR> | <BASH_AND>

     <HEX_DIGIT>     ::= [0-9] | [a-f] | [A-F]

     <WP_DIGIT>      ::= " " | "\t"
     <STR_DIGIT>     ::= <WP_DIGIT> | <COMMENT> | <WORD> | <SPETIAL> | <PIPE> | <PARENS> | <SEMICOLON> | <BASH_EVENT> | <VARIABLE> | <BASH_TEST> | <BASH_SUBS> | <SUBPROGRAM> | <BASH_UNK1> | <BASH_OR>
     <ARG_DIGIT>     ::= <WORD> | <SPETIAL> | <NOT_IMPL>
     <FLAG_DIGIT>    ::= <WORD> | "-"
     <VAR_DIGIT>     ::= <WORD>

     <IGNORE_ESCAPE> ::= <ANY_ESCAPE>
     <HEX_ESCAPE>    ::= "\\" ( "x" | "X" ) <HEX_DIGIT> <HEX_DIGIT>
     <COMMON_ESCAPE> ::= "\\" ( "a" | "r" | "t" | "n" | "b" | "e" | "v" )
     <UNIC_ESCAPE>   ::= "\\u" <HEX_ESCAPE> <HEX_ESCAPE> <HEX_ESCAPE> <HEX_ESCAPE>

     <ANY_ESCAPE>    ::= "\\" ( <STR_DIGIT> | "'" | "\"" | "\\" )
     <STR_ESCAPE>    ::= <HEX_ESCAPE> | <COMMON_ESCAPE> | <UNIC_ESCAPE> | <IGNORE_ESCAPE>

     <WHITESPACE>    ::= <WP_DIGIT>+
     <ARGUMENT>      ::= ( <ARG_DIGIT> | <ANY_ESCAPE> )+
     <STRING_SQ>     ::= "'" ( <STR_DIGIT> | "\"" | <STR_ESCAPE> | "\\'" )* "'"
     <STRING_DQ>     ::= "\"" ( <STR_DIGIT> | "'" | <STR_ESCAPE> | "\\\"" )* "\""
     <FLAG>          ::= "-" <WORD>+
     <LONG_FLAG>     ::= "--" <FLAG_DIGIT>+
     <VARIABLE>      ::= "$" <VAR_DIGIT>+

     <TOKEN>         ::= <WHITESPACE> | <ARGUMENT> | <STRING_SQ> | <STRING_DQ> | <FLAG> | <LONG_FLAG> | <VARIABLE>
     <EXPR>          ::= <TOKEN>+

 *  !!! BNF AND REALIZATION NOT SAME !!!
 **/



const char* shl_str_token(SHLToken type)
{
    switch (type)
    {
    case SHLT_EOF:
        return "EOF";
    case SHLT_Whitespace:
        return "Whitespace";
    case SHLT_Argument:
        return "Argument";
    case SHLT_StringDQ:
        return "StringDQ";
    case SHLT_StringSQ:
        return "StringSQ";
    case SHLT_SubProgram:
        return "SubProgram";
    case SHLT_Flag:
        return "Flag";
    case SHLT_LongFlag:
        return "LongFlag";
    case SHLT_Variable:
        return "Variable";
    case SHLT_StateError:
        return "StateError";
    default:
        return "Unknown";
    }
}
