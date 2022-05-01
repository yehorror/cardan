#pragma once

#include <v8-function.h>

namespace cardan
{
    class Context;
}

namespace cardan::classDetails
{
    class Constructor
    {
    public:
        virtual void addConstructor(Context& context, v8::Local<v8::FunctionTemplate>& constructorFuncTemplate) = 0;
        virtual ~Constructor() = default;
    };
}

#include "Constructor.inl"
