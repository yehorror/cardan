#pragma once

#include "DefaultConstructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    ClassT* DefaultConstructor<ClassT>::construct(Context& context, const v8::FunctionCallbackInfo<v8::Value>& callInfo)
    {
        if constexpr (std::is_default_constructible_v<ClassT>)
        {
            return new ClassT;
        }
        throw std::logic_error("Think about this message");
    }
}
