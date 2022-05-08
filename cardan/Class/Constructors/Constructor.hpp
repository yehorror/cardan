#pragma once

#include "cardan/Context.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    class Constructor
    {
    public:
        virtual ClassT* construct(Context& context, const v8::FunctionCallbackInfo<v8::Value>& callInfo) = 0;
        virtual ~Constructor() = default;
    };
}
