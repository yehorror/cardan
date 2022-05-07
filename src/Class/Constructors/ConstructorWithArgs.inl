#pragma once

#include "ConstructorWithArgs.hpp"

namespace cardan::classDetails
{
    template <class ClassT, class... Args>
    ClassT* makeInstance(Args&&... args)
    {
        return new ClassT(std::forward<Args>(args)...);
    }

    template <class ClassT, class... Args>
    ConstructorWithArgs<ClassT, Args...>::ConstructorWithArgs()
    {
    }

    template <class ClassT, class... Args>
    ClassT* ConstructorWithArgs<ClassT, Args...>::construct(
        Context& context,
        const v8::FunctionCallbackInfo<v8::Value>& callInfo
    )
    {
        auto tupleWithArgs = details::convertArgumentsFromV8<Args...>(context, callInfo);

        return std::apply(makeInstance<ClassT, Args...>, std::forward<decltype(tupleWithArgs)>(tupleWithArgs));
    }
}
