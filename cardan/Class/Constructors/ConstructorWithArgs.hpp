#pragma once

#include "Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT, class... Args>
    class ConstructorWithArgs : public Constructor<ClassT>
    {
    public:
        ConstructorWithArgs();
        ClassT* construct(Context& context, const v8::FunctionCallbackInfo<v8::Value>& callInfo) override;
    };
}

#include "ConstructorWithArgs.inl"
