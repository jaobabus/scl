#ifndef CMD_COMMAND_H
#define CMD_COMMAND_H

#include "argument.hpp"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    int (*execute)(const void* command, const char* cmdline, size_t size);
    const SHCArgument* arguments;
    uint8_t arg_count;
    uint8_t pd[3];
} SHCCommand;


#ifdef __cplusplus
}
#endif



namespace cmd
{


#define TYPED_ARGUMENT_REF(type, field) \
    TypedArgumentTemplateRef<type, decltype(type::field), offsetof(type, field)>


template<typename Container, typename Type, size_t t_offset>
struct TypedArgumentTemplateRef
{
    using container = Container;
    using type = Type;
    constexpr static size_t offset = t_offset;
};


template<typename T>
void construct_object(void* ptr)
{
    new(static_cast<T*>(ptr)) T();
}

template<typename ArgRef>
constexpr SHCArgument make_arg_desc()
{
    return SHCArgument {
        construct_object<typename ArgRef::type>,
        ArgRef::sc_parse,
        ArgRef::sc_completes,
        (uint16_t)(sizeof(typename ArgRef::type))
    };
}


class Command
{
public:
    constexpr Command(const char* name, const cmd::Description* = {}) : name(name) {}

    __attribute__((deprecated))
    virtual const SHCArgument* get_argument(size_t index) const = 0;
    const char* name;

};


template<typename... Args>
class TypedCommand : public Command
{
public:
    using Command::Command;

public:
    static int sc_execute(const void* cmd, const char* cmdline, size_t size);

public:
    static constexpr SHCArgument arguments_table[]
    {
        make_arg_desc<Args>()...
    };

    static constexpr SHCCommand sc_descriptor
    {
        sc_execute,
        arguments_table,
        (uint8_t)(sizeof(arguments_table) / sizeof(arguments_table[0]))
    };

public:
    virtual int execute(const typename Args::type&...) const noexcept = 0;
    const SHCArgument* get_argument(size_t index) const override;

};

template<typename... Args>
constexpr SHCArgument TypedCommand<Args...>::arguments_table[];
template<typename... Args>
constexpr SHCCommand TypedCommand<Args...>::sc_descriptor;

template<typename... Args>
const SHCArgument* TypedCommand<Args...>::get_argument(size_t index) const
{
    return (index < sc_descriptor.arg_count ? &arguments_table[index] : nullptr);
}


}


#endif // CMD_COMMAND_H
