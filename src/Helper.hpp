#pragma once

#include "v8.h"
#include "Converters/Converters.hpp"
#include "Converters/ConvertersFromV8.hpp"

namespace cardan::details
{
    template <class T>
    static void convertValueToV8ReturnValue(v8::Isolate* isolate, T value, v8::ReturnValue<v8::Value>& returnValue)
    {
        returnValue.Set(converters::convert(isolate->GetCurrentContext(), value));
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

    //------------------------------------------------------------------------------------

    template <size_t Idx=0, class... TupleT>
    static void argumentsToVector(const std::tuple<TupleT...>& arguments, std::vector<v8::Local<v8::Value>>& values, v8::Isolate* isolate)
    {
        values.push_back(
            converters::convert(isolate->GetCurrentContext(), std::get<Idx>(arguments))
        );

        if constexpr (Idx < (std::tuple_size<std::tuple<TupleT...>>::value - 1))
        {
            argumentsToVector<Idx + 1, TupleT...>(arguments, values, isolate);
        }
    }

    template <>
    void argumentsToVector(const std::tuple<>&, std::vector<v8::Local<v8::Value>>&, v8::Isolate*)
    {
    }

}
