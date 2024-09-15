#ifndef CMD_COMMAND_H
#define CMD_COMMAND_H

#include "argument.hpp"
#include "command.h"
#include "utils.hpp"


#ifndef SCLC_MAKE_NAME
#define SCLC_MAKE_NAME(name) SHL_CONCAT(command_, name)
#endif


template<typename Tuple, typename ArgsTuple, typename Indices>
struct SCLArgumentDescTable;

template<typename Tuple, typename ArgsTuple, size_t... indices>
struct SCLArgumentDescTable<Tuple, ArgsTuple, std14::index_sequence<indices...>>
{
    template<size_t index>
    constexpr const SCLArgumentDescriptor* get_ptr() const
    {
        return &tuple_element<index, ArgsTuple>::type::sc_descriptor;
    }
    template<size_t index>
    constexpr uint16_t get_offset() const
    {
        return get_offset_of<index, Tuple>();
    }
    const SCLArgumentDescriptor* ptr_table[sizeof...(indices)] = {get_ptr<indices>()...};
    const uint16_t offsets_table[sizeof...(indices)] = {get_offset<indices>()...};
};

template<typename TypedCommand, typename... Args>
class DefaultCommand
{
public:
    /* TypedCommand requires execute in Derrived class, please be careful
        * > static uint8_t Derrived::execute(const typename Args::type&...) noexcept;
        */
    static uint8_t execute(const typename Args::type&...) noexcept;
};

template<typename _Derrived, typename... Args>
class TypedCommand : public DefaultCommand<TypedCommand<_Derrived, Args...>, Args...>
{
public:
    using Derrived = _Derrived;
    using Tuple = MyTuple<typename Args::type...>;

public:
    static SCLExecuteError sc_execute(void* obj_buffer, const uint16_t* flags, size_t size)
    {
        auto err = apply(Derrived::execute, *static_cast<Tuple*>(obj_buffer));
        return SCLExecuteError{(uint8_t) err, 0};
    }

public:
    static constexpr SCLArgumentDescTable<Tuple,
                                          MyTuple<Args...>,
                                          std14::make_index_sequence<sizeof...(Args)>>
        sc_arg_descriptors_table{};

    static constexpr SCLCommandDescriptor sc_descriptor_base{sc_execute,
                                                             sc_arg_descriptors_table.ptr_table,
                                                             Derrived::sc_opaques,
                                                             sc_arg_descriptors_table.offsets_table,
                                                             (uint8_t) sizeof...(Args)};
};

constexpr size_t get_alloc_size(const SCLCommandDescriptor* descriptor)
{
    size_t max_size = 0;
    for (size_t i = 0; i < descriptor->arg_count; ++i)
    {
        const auto size = descriptor->arguments_offsets[i] + descriptor->arguments[i]->obj_size;
        if (size > max_size)
            max_size = size;
    }
    return max_size;
}

constexpr size_t get_max_object_alloc(const SCLCommandDescriptor* const* descriptors,
                                      size_t count)
{
    size_t max_size = 0;
    for (size_t i = 0; i < count; ++i)
    {
        const auto size = get_alloc_size(descriptors[i]);
        if (size > max_size)
            max_size = size;
    }
    return max_size;
}

template<size_t _size>
struct CStringContainer
{
    static constexpr size_t size = _size;
    char string[_size];
};

template<typename Derrived, typename... Args>
constexpr SCLArgumentDescTable<typename TypedCommand<Derrived, Args...>::Tuple,
                               MyTuple<Args...>,
                               std14::make_index_sequence<sizeof...(Args)>>
    TypedCommand<Derrived, Args...>::sc_arg_descriptors_table;
template<typename Derrived, typename... Args>
constexpr SCLCommandDescriptor TypedCommand<Derrived, Args...>::sc_descriptor_base;

// v rot manal clangd
template<typename... Ts>
using SCLC_TypedArgument = TypedArgument<Ts...>;


template<typename D, typename T>
struct ArgsCommandWrapper {};


template<typename Derrived, typename R, typename... Args>
struct ArgsCommandWrapper<Derrived, R (*)(Args...)> {

    using Tuple = MyTuple<Args...>;

    static constexpr SCLArgumentDescTable<Tuple,
                                          MyTuple<SCLArgumentByTypeT<Args>...>,
                                          std14::make_index_sequence<sizeof...(Args)>>
        sc_arg_descriptors_table{};

    static constexpr SCLCommandDescriptor sc_descriptor_base{&Derrived::sc_execute,
                                                             sc_arg_descriptors_table.ptr_table,
                                                             nullptr,
                                                             sc_arg_descriptors_table.offsets_table,
                                                             (uint8_t) sizeof...(Args)};
};


#define SCLC_DeclareCommand(name, args...) \
    static void SHL_CONCAT(SCLC_MAKE_NAME(name), _dummy_args)(args); \
    struct SHL_CONCAT(SCLC_MAKE_NAME(name), _type) \
            : ArgsCommandWrapper<SHL_CONCAT(SCLC_MAKE_NAME(name), _type), \
                                 decltype(&SHL_CONCAT(SCLC_MAKE_NAME(name), _dummy_args))> { \
        struct Body { \
            uint8_t execute(); \
            SHL_JOIN(;, args); \
        }; \
        static inline SCLExecuteError sc_execute(void* obj_buffer, const uint16_t* flags, size_t size) { \
            return {reinterpret_cast<Body*>(obj_buffer)->execute(), 0}; \
        } \
        static constexpr SCLCommandDescriptorWithName<sizeof(#name)> sc_descriptor{sc_descriptor_base, {#name}}; \
    } SCLC_MAKE_NAME(name); \
    uint8_t SHL_CONCAT(SCLC_MAKE_NAME(name), _type)::Body::execute()



#endif // CMD_COMMAND_H
