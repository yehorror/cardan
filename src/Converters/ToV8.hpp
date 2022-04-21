#pragma once

#include <v8.h>
#include <string>
#include <functional>

namespace cardan
{
    class Function;
    class Context;
}

namespace cardan::ToV8
{
    struct ADLTag
    {
    };

    v8::Local<v8::Value> convert(Context& context, int value, ADLTag);
    v8::Local<v8::Value> convert(Context& context, double value, ADLTag);
    v8::Local<v8::Value> convert(Context& context, const std::string& value, ADLTag);
    v8::Local<v8::Value> convert(Context& context, v8::Local<v8::Function> function, ADLTag);

    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(Context& context, std::function<FuncReturnType(FuncArgs...)> func, ADLTag);

    template <class FunctorType, class... InvokeArgs>
    std::enable_if_t<
        std::is_invocable<FunctorType, InvokeArgs...>::value,
        v8::Local<v8::Value>
    > convert(Context& context, FunctorType func, ToV8::ADLTag);
}

#include "ToV8.inl"
