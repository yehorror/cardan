#pragma once

#include <v8.h>
#include <string>
#include <functional>

namespace cardan
{
    class Function;
}

namespace cardan::converters
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, int value);
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, double value);
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const std::string& value);
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, Function value);

    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, std::function<FuncReturnType(FuncArgs...)>& func);
}

#include "Converters.inl"
