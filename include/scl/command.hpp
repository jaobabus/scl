#ifndef CMD_COMMAND_H
#define CMD_COMMAND_H

#include "argument.hpp"



#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
    uint16_t error;
    uint8_t token;
    uint8_t pd[1];
} SCLExecuteError;


typedef struct
{
    SCLExecuteError (*execute)(const void* opaque, void* obj_buffer, const uint16_t* flags, size_t size);
    uint8_t (*is_command)(const void* opaque, const char* name, size_t size);
    const SCLArgument* arguments;
    uint8_t arg_count;
    uint8_t pd[3];
} SCLCommand;



#ifdef __cplusplus
}
#endif



#ifdef __cplusplus



template<typename T>
void construct_object(void* ptr)
{
    // new(static_cast<T*>(ptr)) T(); ????
}

template<typename T>
void destruct_object(void* ptr)
{
    static_cast<T*>(ptr)->~T();
}

template<typename Elem>
constexpr SCLArgument make_arg_desc()
{
    using Arg = typename Elem::type;
    return SCLArgument {
        construct_object<typename Arg::type>,
        destruct_object<typename Arg::type>,
        Arg::sc_parse,
        Arg::sc_completes,
        (uint16_t)(sizeof(typename Arg::type)),
        Elem::offset
    };
}

template<typename Tuple, typename Indices>
struct SCLArgumentDescTable;

template<typename Tuple, size_t... indices>
struct SCLArgumentDescTable<Tuple, std::index_sequence<indices...>>
{
    SCLArgument table[sizeof...(indices)] =
    {
        make_arg_desc<tuple_element<indices, Tuple>>()...
    };
};

template<typename... Args>
class TypedCommand
{
public:
    static SCLExecuteError sc_execute(const void* opaque, void* obj_buffer, const uint16_t* flags, size_t size)
    {
        auto err = apply(&TypedCommand<Args...>::execute, static_cast<const TypedCommand<Args...>*>(opaque), *static_cast<MyTuple<const typename Args::type...>*>(obj_buffer));
        return SCLExecuteError{err, 0};
    }

    static uint8_t sc_is_command(const void* opaque, const char* name, size_t size)
    {
        size_t th_name_size = strlen(static_cast<const TypedCommand<Args...>*>(opaque)->_name);
        if (th_name_size != size)
            return 0;
        return memcmp(name, static_cast<const TypedCommand<Args...>*>(opaque)->_name, size) == 0;
    }

public:
    TypedCommand(const char* name)
        : _name(name) {}

public:
    static constexpr SCLArgumentDescTable<MyTuple<Args...>, std::make_index_sequence<sizeof...(Args)>> sc_arg_descriptors_table {};

    static constexpr SCLCommand sc_descriptor
    {
        sc_execute,
        sc_is_command,
        sc_arg_descriptors_table.table,
        (uint8_t)sizeof...(Args)
    };

public:
    virtual SCLError execute(const typename Args::type&...) const noexcept = 0;

private:
    const char* _name;

};

template<typename... Args>
constexpr SCLArgumentDescTable<MyTuple<Args...>, std::make_index_sequence<sizeof...(Args)>> TypedCommand<Args...>::sc_arg_descriptors_table;
template<typename... Args>
constexpr SCLCommand TypedCommand<Args...>::sc_descriptor;



#endif


#endif // CMD_COMMAND_H
