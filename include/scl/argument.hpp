#ifndef SCL_ARGUMENT_HPP
#define SCL_ARGUMENT_HPP

#include "inplace.h"
#include "utils.hpp"

#ifdef __cplusplus

#ifndef SCLA_MAKE_NAME
#define SCLA_MAKE_NAME(name) SHL_CONCAT(argument_, name)
#endif

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

#define SCLA_SPECIALIZE_TYPE(Argument, Type) \
    template<> \
    struct SCLArgumentByType<Type> { \
        using type = Argument; \
    }


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


#endif

#endif // SCL_ARGUMENT_HPP
