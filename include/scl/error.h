#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>
#include <stdint.h>



enum ESCLError
{
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

    /** Command token must be SHLT_Argument
     */
    SCLE_InvalidCommandToken = SCLE_CommandErrorsStart,

    /** After '--flag=' expected common argument
     */
    SCLE_ExpectedFlagValue,

    /** Command errors section end
     */
    SCLE_CommandErrorsEnd = SCLE_ExpectedFlagValue,

    /** User errors section
     */
    SCLE_UserErrorsStart,

    /** User errors section end
     */
    SCLE_UserErrorsEnd = 222,

    // All other reserved
};

typedef enum ESCLError SCLError;



#endif // ERROR_H
