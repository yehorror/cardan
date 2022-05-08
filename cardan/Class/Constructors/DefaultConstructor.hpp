#pragma once

#include "Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    class DefaultConstructor : public Constructor<ClassT>
    {
    public:
        ClassT* construct(Context& context, const v8::FunctionCallbackInfo<v8::Value>& callInfo) override;
    };
}

#include "DefaultConstructor.inl"
