#pragma once

#include "Context.hpp"

namespace cardan
{

    template <class ValueType>
    void Context::set(const std::string& name, ValueType&& value)
    {
        v8::Local<v8::String> v8Name = v8::String::NewFromUtf8(m_isolate.get(), name.c_str()).ToLocalChecked();
        setInternal(v8Name, std::forward<ValueType>(value));
    }

    template <class FuncReturnType, class... FuncArgs>
    void Context::setInternal(v8::Local<v8::String> funcName, std::function<FuncReturnType(FuncArgs...)>& func)
    {
        auto funcTemplate = v8::FunctionTemplate::New(
            m_isolate.get(), callCppFunctionFromJS<FuncReturnType, FuncArgs...>, v8::External::New(m_isolate.get(), &func)
        );

        m_context->Global()->Set(
            m_context,
            funcName,
            funcTemplate->GetFunction(m_context).ToLocalChecked()
        ).Check();
    }

    template<class FuncReturnType, class... FuncArgs>
    void Context::callCppFunctionFromJS(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        auto funcPtr = info.Data().As<v8::External>()->Value();
        auto& function = *static_cast<std::function<FuncReturnType(FuncArgs...)>*>(funcPtr);

        if constexpr (std::is_same_v<void, FuncReturnType>)
        {
            // If bound function has 'void' return type, just call function and don't bother about result
            std::apply(function, details::packArguments<FuncArgs...>(info));
        }
        else
        {
            // Otherwise, convert this value to V8's return value
            auto funcExecutionResult = std::apply(function, details::packArguments<FuncArgs...>(info));
            auto returnValue = info.GetReturnValue();

            details::convertValueToV8ReturnValue(info.GetIsolate(), funcExecutionResult, returnValue);
        }
    }

}
