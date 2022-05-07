#pragma once

#include "v8.h"
#include "Converters/FromV8.hpp"
#include <functional>

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
    static std::tuple<Args...> convertArgumentsFromV8Impl(
        Context& context,
        const v8::FunctionCallbackInfo<v8::Value>& info,
        std::index_sequence<I...>
    )
    {
        return std::make_tuple<Args...>(
            convert(
                context,
                info[I],
                cardan::FromV8::To<
                    typename getType<I, Args...>::type
                >{}
            )...
        );
    }

    template <class... Args>
    static std::tuple<Args...> convertArgumentsFromV8(Context& context, const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        return convertArgumentsFromV8Impl<Args...>(
            context,
            info,
            std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>> {}
        );
    }

    template <>
    std::tuple<> convertArgumentsFromV8(Context& /*context*/, const v8::FunctionCallbackInfo<v8::Value>& /*info*/)
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

    //------------------------------------------------------------------------------------

    template <typename T>
    struct FunctionTraits
        : public FunctionTraits<decltype(&T::operator())>
    {};

    // Functors
    template <typename ClassType, typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType(ClassType::*)(Args...) const>
    {
        using StdFunctionType = std::function<ReturnType(Args...)>;
    };

    // Static functions
    template <typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType(*)(Args...)>
    {
        using StdFunctionType = std::function<ReturnType(Args...)>;
    };
}
