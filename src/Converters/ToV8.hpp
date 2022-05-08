#pragma once

#include <v8.h>
#include <string>
#include <functional>

#include "Value/Value.hpp"

namespace cardan
{
    class Function;
    class Context;

    template <class ClassT> class Class;
}

namespace cardan::ToV8
{
    // This struct is used to force compiler to use ADL on converters lookup
    struct ADLTag
    {
    };

    v8::Local<v8::Value> convert(Context& context, int value, ADLTag);
    v8::Local<v8::Value> convert(Context& context, double value, ADLTag);
    v8::Local<v8::Value> convert(Context& context, const std::string& value, ADLTag);
    v8::Local<v8::Value> convert(Context& context, v8::Local<v8::Function> function, ADLTag);
    v8::Local<v8::Value> convert(Context& context, Value value, ADLTag);

    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(Context& context, std::function<FuncReturnType(FuncArgs...)> func, ADLTag);

    /*
    template <class FunctorType>
    std::enable_if_t<
        std::is_convertible_v<FunctorType, std::
        v8::Local<v8::Value>
    > convert(Context& context, FunctorType func, ToV8::ADLTag);
    */
}

#include "ToV8.inl"
