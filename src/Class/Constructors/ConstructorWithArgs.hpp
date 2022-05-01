#pragma once

#include "Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT, typename... Args>
    class ConstructorWithArgs : public Constructor
    {
    public:
        void addConstructor(Context& context, v8::Local<v8::FunctionTemplate>& constructorFuncTemplate) override;
    };
}

#include "ConstructorWithArgs.inl"
