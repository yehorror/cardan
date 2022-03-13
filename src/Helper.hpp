#pragma once

#include "v8.h"
#include "Value/Function.hpp"

namespace cardan::details
{
    // TODO Consider moving convert functions into separate header

    // Function overloads to convert JS call arguments into C++ functions arguments
    template <class Type>
    static Type convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value);

    template <>
    int convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return value->Int32Value(context).ToChecked();
    }

    template <>
    std::string convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return *v8::String::Utf8Value(context->GetIsolate(), value);
    }

    template <>
    Function convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return Function(value.As<v8::Function>(), context);
    }

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
            convertArgumentFromV8Value<typename getType<I, Args...>::type>(info.GetIsolate()->GetCurrentContext(), info[I])...
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
