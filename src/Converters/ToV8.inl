#pragma once

#include "Helper.hpp"
#include "Context.hpp"

namespace cardan::ToV8
{
    namespace Detail
    {
        template <class FunctionType>
        struct FunctionWithContext
        {
            FunctionWithContext(FunctionType&& function, Context& context)
                : m_function(std::move(function))
                , m_context(context)
            {
            }

            FunctionType m_function;
            Context& m_context;
        };
    }

    // TODO Research if we can implement same functions for lambdas, etc
    // Can we have some internal storage for these functions objects? And delete them when objects are deleted in JS context
    // TODO Also this method is too big. It is better to refactor it somehow
    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(Context& context, std::function<FuncReturnType(FuncArgs...)> func, ToV8::ADLTag)
    {
        using FunctionType = std::function<FuncReturnType(FuncArgs...)>;

        struct FunctionWithContext : public ValueHolderBase
        {
            FunctionWithContext(FunctionType&& function, Context& context)
                : m_function(std::move(function))
                , m_context(context)
            {
            }

            FunctionType m_function;
            Context& m_context;
        };

        auto savedFunctionWithContext = std::make_unique<FunctionWithContext>(std::move(func), context);

        FunctionWithContext* savedFunctionWithContextPtr = savedFunctionWithContext.get();

        context.saveFunction(std::move(savedFunctionWithContext));

        auto callbackLambda = [] (const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            auto funcPtr = info.Data().As<v8::External>()->Value();
            auto& functionWithContext = *static_cast<FunctionWithContext*>(funcPtr);
            auto isolate = info.GetIsolate();

            if (std::tuple_size<std::tuple<FuncArgs...>>::value != info.Length())
            {
                isolate->ThrowException(
                    convert(functionWithContext.m_context, "Invalid number of arguments", ToV8::ADLTag{})
                );
                return;
            }

            // TODO Catch possible exceptions here
            auto tupleWithArguments = details::convertArgumentsFromV8<FuncArgs...>(functionWithContext.m_context, info);

            try
            {
                if constexpr (std::is_same_v<void, FuncReturnType>)
                {
                    // If bound function has 'void' return type, just call function and don't bother about result
                    std::apply(functionWithContext.m_function, tupleWithArguments);
                }
                else
                {
                    // Otherwise, convert this value to V8's return value
                    auto funcExecutionResult = std::apply(functionWithContext.m_function, tupleWithArguments);
                    auto returnValue = info.GetReturnValue();

                    returnValue.Set(convert(functionWithContext.m_context, funcExecutionResult, ToV8::ADLTag{}));
                }
            }
            catch (const std::exception& exception)
            {
                isolate->ThrowException(convert(functionWithContext.m_context, exception.what(), ToV8::ADLTag{}));
            }
        };

        auto funcTemplate = v8::FunctionTemplate::New(
            context.getIsolate(), callbackLambda, v8::External::New(context.getIsolate(), savedFunctionWithContextPtr)
        );

        return funcTemplate->GetFunction(context.getContext()).ToLocalChecked();
    }

    // Scary enable_if to force compiler not to use this function for non-invocable types
    template <class FunctorType, class... InvokeArgs>
    std::enable_if_t<
        std::is_invocable<FunctorType, InvokeArgs...>::value,
        v8::Local<v8::Value>
    > convert(Context& context, FunctorType func, ToV8::ADLTag)
    {
        using StdFunctionType = typename details::FunctionTraits<FunctorType>::StdFunctionType;
        return convert(context, StdFunctionType(func), ToV8::ADLTag{});
    }
}
