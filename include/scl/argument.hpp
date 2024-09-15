#ifndef SCL_ARGUMENT_HPP
#define SCL_ARGUMENT_HPP

#include "argument.h"
#include "inplace.h"
#include "utils.hpp"

#ifdef __cplusplus

#ifndef SCLA_MAKE_NAME
#define SCLA_MAKE_NAME(name) SHL_CONCAT(argument_, name)
#endif


template<typename T>
void destruct_object(void* ptr)
{
    static_cast<T*>(ptr)->~T();
}


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

public:
    static constexpr SCLArgumentDescriptor sc_descriptor{&destruct_object<Type>,
                                                         &sc_parse,
                                                         &sc_completes,
                                                         (uint16_t)sizeof(Type),
                                                         0};

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

#endif

#endif // SCL_ARGUMENT_HPP
