#pragma once

#include "v8.h"
#include "Converters/Converters.hpp"
#include "Converters/ConvertersFromV8.hpp"

namespace cardan::details
{
    // TODO Add comments here to describe what happens
    // Also, maybe refactor it in some way, so we won't need to forward-declare some functions
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

    template <size_t Idx=0, class... TupleT, class Functor>
    static void forEachElementInTuple(const std::tuple<TupleT...>& tuple, Functor callback)
    {
        callback(std::get<Idx>(tuple));

        if constexpr (Idx < (std::tuple_size<std::tuple<TupleT...>>::value - 1))
        {
            forEachElementInTuple<Idx + 1, TupleT...>(tuple, callback);
        }
    }

    template <class Functor>
    void forEachElementInTuple(const std::tuple<>& tuple, Functor callback)
    {
    }
}
