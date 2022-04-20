#pragma once

#include "Converters/ToV8.hpp"
#include "Value/Value.hpp"
#include "Value/Array.hpp"
#include "Value/Object.hpp"
#include "Value/Function.hpp"
#include "Helper.hpp"

namespace cardan
{
    template <class ValueType>
    void Context::set(const std::string& name, ValueType&& value)
    {
        v8::Local<v8::String> v8Name = v8::String::NewFromUtf8(m_isolate.get(), name.c_str()).ToLocalChecked();
        v8::Local<v8::Value> v8Value = ToV8::convert(*this, std::forward<ValueType>(value));

        m_context->Global()->Set(
            m_context,
            v8Name,
            v8Value
        ).Check();
    }

    template <class ValueType>
    Value Context::makeValue(ValueType&& value)
    {
        v8::Local<v8::Value> v8Value = ToV8::convert(*this, std::forward<ValueType>(value));
        return Value(v8Value, *this);
    }
    /*
    template <class FuncReturnType, class... FuncArgs>
    void Context::addFunction(const std::string& name, std::function<FuncReturnType(FuncArgs...)> func)
    {
        using FunctionType = std::function<FuncReturnType(FuncArgs...)>;

        struct FunctionValueHolder : ValueHolderBase
        {
            FunctionValueHolder(FunctionType&& function)
                : m_holdableFunction(std::make_unique<FunctionType>(std::move(function)))
            {
            }

            std::unique_ptr<FunctionType> m_holdableFunction;
        };

        auto holdedFunction = std::make_unique<FunctionValueHolder>(std::move(func));

        FunctionType* copiedFunctionPointer = holdedFunction->m_holdableFunction.get();

        m_functions.insert(std::move(holdedFunction));

        auto callbackLambda = [] (const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            auto funcPtr = info.Data().As<v8::External>()->Value();
            auto& function = *static_cast<std::function<FuncReturnType(FuncArgs...)>*>(funcPtr);
            auto isolate = info.GetIsolate();

            if (std::tuple_size<std::tuple<FuncArgs...>>::value != info.Length())
            {
                isolate->ThrowException(ToV8::convert(isolate->GetCurrentContext(), "Invalid number of arguments"));
                return;
            }

            // TODO Catch possible exceptions here
            auto tupleWithArguments = details::packArguments<FuncArgs...>(info);

            try
            {
                if constexpr (std::is_same_v<void, FuncReturnType>)
                {
                    // If bound function has 'void' return type, just call function and don't bother about result
                    std::apply(function, tupleWithArguments);
                }
                else
                {
                    // Otherwise, convert this value to V8's return value
                    auto funcExecutionResult = std::apply(function, tupleWithArguments);
                    auto returnValue = info.GetReturnValue();

                    returnValue.Set(ToV8::convert(info.GetIsolate()->GetCurrentContext(), funcExecutionResult));
                }
            }
            catch (const std::exception& exception)
            {
                isolate->ThrowException(ToV8::convert(isolate->GetCurrentContext(), exception.what()));
            }
        };

        auto funcTemplate = v8::FunctionTemplate::New(
            m_context->GetIsolate(), callbackLambda, v8::External::New(m_context->GetIsolate(), copiedFunctionPointer)
        );

        set(name, funcTemplate->GetFunction(m_context).ToLocalChecked());
    }

    template <class FunctorType>
    void Context::addFunction(const std::string& name, FunctorType func)
    {
        using StdFunctionType = typename details::FunctionTraits<FunctorType>::StdFunctionType;
        addFunction(name, StdFunctionType(func));
    }
    */
}
