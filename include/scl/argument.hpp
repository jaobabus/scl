#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "utils.hpp"
#include "inplace.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    void (*construct)(void* obj);
    void (*destruct)(void* obj);
    SCLError (*parse)(const void* opaque, void* obj, SHLITokenInfo token);
    size_t (*completes)(const void* opaque, char* buffer, size_t buffer_size, const char* token, size_t size);
    uint16_t obj_size;
    uint16_t obj_offset;
} SCLArgument;


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus


template<typename Type>
class TypedArgument
{
public:
    static SCLError sc_parse(const void* th, void* obj, SHLITokenInfo token) noexcept
    {
        return static_cast<const TypedArgument<Type>*>(th)->parse(*(Type*)obj, token);
    }
    static size_t sc_completes(const void* th, char* buffer, size_t buffer_size, const char* token, size_t size) noexcept
    {
        return static_cast<const TypedArgument<Type>*>(th)->completes(buffer, buffer_size, token, size);
    }

public:
    using type = Type;

    virtual SCLError parse(type& value, SHLITokenInfo token) const noexcept = 0;
    virtual size_t completes(char* buffer, size_t buffer_size, const char* token, size_t size) const noexcept { return 0; };

};

template<typename T>
T* argument_instance()
{
    static T value;
    return &value;
}


#endif


#endif // ARGUMENT_H
