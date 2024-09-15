#pragma once

#include <scl/utils.hpp>
#include <scl/argument.hpp>
#include <scl/error.h>


class StringArg : public TypedArgument<StringArg, StringView>
{
public:
    SCLError parse(type& value, const SHLITokenInfo& token) const noexcept
    {
        value = StringView{(const char*)token.data, token.size};
        return SCLE_NoError;
    }

};

SCLA_SPECIALIZE_TYPE(StringArg, StringView);


class IntArg : public TypedArgument<IntArg, int>
{
public:
    SCLError parse(type& value, const SHLITokenInfo& token) const noexcept
    {
        return SCLE_NoError;
    }

};

SCLA_SPECIALIZE_TYPE(IntArg, int);
