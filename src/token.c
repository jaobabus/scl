#include <scl/token.h>



const char* shl_str_token(SHLToken type)
{
    switch (type)
    {
    case SHLT_None:
        return "None";
    case SHLT_EOF:
        return "EOF";
    case SHLT_Whitespace:
        return "Whitespace";
    case SHLT_FlagEquals:
        return "FlagEquals";
    case SHLT_Argument:
        return "Argument";
    case SHLT_StringDQ:
        return "StringDQ";
    case SHLT_StringSQ:
        return "StringSQ";
    /*case SHLT_SubProgram:
        return "SubProgram";*/
    case SHLT_Flag:
        return "Flag";
    case SHLT_LongFlag:
        return "LongFlag";
    /*case SHLT_Variable:
        return "Variable";*/
    case SHLT_StateError:
        return "StateError";
    default:
        return "Unknown";
    }
}
