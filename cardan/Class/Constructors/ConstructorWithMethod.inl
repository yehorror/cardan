#pragma once

#include "ConstructorWithMethod.hpp"
#include "cardan/Helper.hpp"

namespace cardan::classDetails
{
    template <class ClassT, class MethodT>
    ConstructorWithMethod<ClassT, MethodT>::ConstructorWithMethod(MethodT&& method)
        : m_method(std::forward<MethodT>(method))
    {
    }

    template <class ClassT, class MethodT>
    ClassT* ConstructorWithMethod<ClassT, MethodT>::construct(
        Context& context,
        const v8::FunctionCallbackInfo<v8::Value>& callInfo
    )
    {
        using StdFunction = typename details::FunctionTraits<MethodT>::StdFunctionType;

        StdFunction func = m_method;

        return createInstance(context, callInfo, func);
    }

    template <class ClassT, class MethodT>
    template <typename... Args>
    ClassT* ConstructorWithMethod<ClassT, MethodT>::createInstance(
        Context& context,
        const v8::FunctionCallbackInfo<v8::Value>& callInfo,
        std::function<ClassT*(Args...)> creationFunction
    )
    {
        auto argsTuple = details::convertArgumentsFromV8<Args...>(context, callInfo);

        return std::apply(m_method, std::forward<decltype(argsTuple)>(argsTuple));
    }
}
