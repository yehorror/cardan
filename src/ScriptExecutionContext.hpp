#pragma once

#include <string>
#include <variant>

#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>
#include <iostream>

namespace cardan
{
    static int convertArgumentFromV8Value(v8::Local<v8::Value> /*value*/)
    {
        return 123;
    }

    template <size_t Idx=0, class... Args>
    static void packArgumentsHelper(std::tuple<Args...>& tuple, const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        std::get<Idx>(tuple) = convertArgumentFromV8Value(info[Idx]);
        if constexpr (Idx < (std::tuple_size<std::tuple<Args...>>::value - 1))
        {
            packArgumentsHelper<Idx + 1, Args...>(tuple, info);
        }
    }

    template <>
    void packArgumentsHelper(std::tuple<>& tuple, const v8::FunctionCallbackInfo<v8::Value>& info)
    {
    }

    template <class... Args>
    static std::tuple<Args...> packArguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        std::tuple<Args...> result;
        packArgumentsHelper(result, info);
        return result;
    }

    template <>
    std::tuple<> packArguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        return std::tuple<>();
    }

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

                std::apply(function, packArguments<FuncArgs...>(info));
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
