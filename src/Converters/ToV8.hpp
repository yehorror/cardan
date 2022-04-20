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
    v8::Local<v8::Value> convert(Context& context, int value);
    v8::Local<v8::Value> convert(Context& context, double value);
    v8::Local<v8::Value> convert(Context& context, const std::string& value);
    v8::Local<v8::Value> convert(Context& context, v8::Local<v8::Function> function);

    /*
    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(Context& context, std::function<FuncReturnType(FuncArgs...)>& func);
    */
}

#include "ToV8.inl"
