#pragma once

#include <string>
#include <variant>

#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>

#include "Helper.hpp"

namespace cardan
{
    class JSException : public std::exception
    {
    };

    struct ScriptExecutionContextConfig
    {
        bool rethrowExceptions = false;
    };

    class ScriptExecutionContext
    {
    public:
        ScriptExecutionContext(const std::string& src, const ScriptExecutionContextConfig& config = {});

        std::variant<std::string, int, JSException> runScript();

    public: // Template methods, which needs to be defined inline

        template<class FuncReturnType, class... FuncArgs>
        void addFunction(const std::string& funcName, std::function<FuncReturnType(FuncArgs...)>& func)
        {
            auto funcCallLambda = [](const v8::FunctionCallbackInfo<v8::Value>& info)
            {
                auto funcPtr = info.Data().As<v8::External>()->Value();
                auto& function = *static_cast<std::function<FuncReturnType(FuncArgs...)>*>(funcPtr);

                if constexpr (std::is_same_v<void, FuncReturnType>)
                {
                    std::apply(function, details::packArguments<FuncArgs...>(info));
                }
                else
                {
                    info.GetReturnValue().Set(std::apply(function, details::packArguments<FuncArgs...>(info)));
                }
            };

            auto funcTemplate = v8::FunctionTemplate::New(
                m_isolate.get(), funcCallLambda, v8::External::New(m_isolate.get(), &func)
            );

            m_context->Global()->Set(
                m_context,
                v8::String::NewFromUtf8(m_isolate.get(), funcName.c_str()).ToLocalChecked(),
                funcTemplate->GetFunction(m_context).ToLocalChecked()
            );
        }

    private:
        const std::string m_jsCode;
        std::unique_ptr<v8::ArrayBuffer::Allocator> m_arrayBufferAllocator;

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
