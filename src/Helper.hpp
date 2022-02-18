#pragma once

#include <string>

#include "v8.h"

namespace cardan::details
{
    // TODO Move all these helper methods to another header
    static void convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value, int& out)
    {
        out = value->Int32Value(context).ToChecked();
    }

    static void convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value, std::string& out)
    {
        out = *v8::String::Utf8Value(context->GetIsolate(), value);
    }

    template <size_t Idx=0, class... Args>
    static void packArgumentsHelper(
        std::tuple<Args...>& tuple, const v8::FunctionCallbackInfo<v8::Value>& info
    )
    {
        convertArgumentFromV8Value(info.GetIsolate()->GetCurrentContext(), info[Idx], std::get<Idx>(tuple));
        if constexpr (Idx < (std::tuple_size<std::tuple<Args...>>::value - 1))
        {
            packArgumentsHelper<Idx + 1, Args...>(tuple, info);
        }
    }

    template <>
    void packArgumentsHelper(std::tuple<>& tuple, const v8::FunctionCallbackInfo<v8::Value>& info)
    {
    }

    template <class... Args>
    static std::tuple<Args...> packArguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        std::tuple<Args...> result;
        packArgumentsHelper(result, info);
        return result;
    }

    template <>
    std::tuple<> packArguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        return std::tuple<>();
    }
}