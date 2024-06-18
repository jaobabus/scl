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
    int (*parse)(const void* argument, void* obj, SHLIDataInfo token);
    size_t (*completes)(const void* argument, char* buffer, size_t buffer_size, const char* token, size_t size);
    uint16_t obj_size;
} SHCArgument;


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus


class Argument
{
public:
    static int sc_parse(const void* th, void* obj, SHLIDataInfo token)
    {
        return static_cast<const Argument*>(th)->parse(obj, token);
    }
    static size_t sc_completes(const void* th, char* buffer, size_t buffer_size, const char* token, size_t size)
    {
        return static_cast<const Argument*>(th)->completes(buffer, buffer_size, token, size);
    }

public:
    constexpr Argument(cmd::Description const* = {}) {}
    ~Argument() {}

    virtual int parse(void* obj, SHLIDataInfo token) const noexcept = 0;
    virtual size_t completes(char* buffer, size_t buffer_size, const char* token, size_t size) const noexcept { return 0; }

};


template<typename Type>
class TypedArgument : public Argument
{
public:
    using type = Type;
    using Argument::Argument;

    int parse(void* obj, SHLIDataInfo token) const noexcept override
    {
        return parse(*static_cast<Type*>(obj), token);
    }
    virtual int parse(type& value, SHLIDataInfo token) const noexcept = 0;

};

template<typename T>
Argument* argument_instance()
{
    static T value;
    return &value;
}


#endif


#endif // ARGUMENT_H
