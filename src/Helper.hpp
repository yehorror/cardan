#pragma once

#include "v8.h"
#include "Converters/ConvertersFromV8.hpp"

namespace cardan::details
{
    // Function overloads to convert C++ return values into JS return values
    template <class T>
    static void convertValueToV8ReturnValue(v8::Isolate* isolate, T value, v8::ReturnValue<v8::Value>& returnValue)
    {
        // We can rely on SFINAE here
        returnValue.Set(value);
    }

    static void convertValueToV8ReturnValue(v8::Isolate* isolate, const std::string& value, v8::ReturnValue<v8::Value>& returnValue)
    {
        returnValue.Set(v8::String::NewFromUtf8(isolate, value.c_str()).ToLocalChecked());
    }

    //------------------------------------------------------------------------------------

    template <size_t N, class... Types>
    struct getType;

    template <size_t N, class T, class... Tail>
    struct getType<N, T, Tail...>
    {
        using type = typename getType<N - 1, Tail...>::type;
    };

    template <class T, class... Tail>
    struct getType<0, T, Tail...>
    {
        using type = T;
    };

    template <class... Args, size_t... I>
    static std::tuple<Args...> packArgumentsImpl(const v8::FunctionCallbackInfo<v8::Value>& info, std::index_sequence<I...>)
    {
        return std::make_tuple<Args...>(
            cardan::converters::convertArgumentFromV8Value<typename getType<I, Args...>::type>(info.GetIsolate()->GetCurrentContext(), info[I])...
        );
    }

    template <class... Args>
    static std::tuple<Args...> packArguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        return packArgumentsImpl<Args...>(info, std::make_index_sequence<std::tuple_size<std::tuple<Args...>>::value> {});
    }

    template <>
    std::tuple<> packArguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        return std::tuple<>();
    }
}
