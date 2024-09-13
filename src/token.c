#include <scl/token.h>
#include <scl/calt.h>



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
