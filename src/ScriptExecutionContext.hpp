#pragma once

#include <string>
#include <variant>

#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>

#include "Helper.hpp"

namespace cardan
{
    // TODO Move exceptions to separate header
    class JSException : public std::exception
    {
    };

    class InvalidJSCodeException : public std::exception
    {
    };

    // TODO Think about compile-time configuration for it
    struct ScriptExecutionContextConfig
    {
        bool rethrowExceptions = false;
    };

    // Context of script execution
    // TODO:
    // * JS function calling from C++
    // * C++ classes binding into JS
    // * Consider moving of script compilation out of this class
    class ScriptExecutionContext
    {
    public:
        using Undefined = std::monostate;
        // TODO Don't use plain std::variant for script execution result; move to separate class
        using ScriptRunResult = std::variant<std::string, int, double, JSException, Undefined>;

    public:
        ScriptExecutionContext(const std::string& src, const ScriptExecutionContextConfig& config = {});

        ScriptRunResult runScript();

    public: // Template methods, which needs to be defined inline

        // TODO Move definition out of here (.inl file?)
        template<class FuncReturnType, class... FuncArgs>
        void addFunction(const std::string& funcName, std::function<FuncReturnType(FuncArgs...)>& func)
        {
            auto funcTemplate = v8::FunctionTemplate::New(
                m_isolate.get(), callCppFunctionFromJS<FuncReturnType, FuncArgs...>, v8::External::New(m_isolate.get(), &func)
            );

            m_context->Global()->Set(
                m_context,
                v8::String::NewFromUtf8(m_isolate.get(), funcName.c_str()).ToLocalChecked(),
                funcTemplate->GetFunction(m_context).ToLocalChecked()
            ).Check();
        }

    private:

        // TODO Move definition out of here (.inl file?)
        template<class FuncReturnType, class... FuncArgs>
        static void callCppFunctionFromJS(const v8::FunctionCallbackInfo<v8::Value>& info)
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

    private:
        // Shouldn't we keep here compiled v8::Script?
        const std::string m_jsCode;
        std::unique_ptr<v8::ArrayBuffer::Allocator> m_arrayBufferAllocator;

        // TODO Move isolate's guard declaration out of here
        struct IsolateDisposer
        {
            void operator() (v8::Isolate* isolate);
        };

        std::unique_ptr<v8::Isolate, IsolateDisposer> m_isolate;

        v8::Isolate::Scope m_isolateScope;
        v8::HandleScope    m_handleScope;

        v8::Local<v8::Context> m_context;
        v8::Context::Scope m_contextScope;

        ScriptExecutionContextConfig m_config;
    };
}
