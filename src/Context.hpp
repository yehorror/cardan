#pragma once

#include <string>
#include <variant>

#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>

#include "Helper.hpp"
#include "Exceptions.hpp"
#include "Value/Value.hpp"
#include "Value/Function.hpp"

namespace cardan
{
    // Context of script execution
    // TODO:
    // * C++ classes binding into JS
    // * Consider moving of script compilation out of this class
    class Context
    {
    public:
        using ScriptRunResult = Value;

    public:
        Context(const std::string& src);

        ScriptRunResult runScript();

        Value get(const std::string& valueName);

    public:

        template <class ValueType>
        void set(const std::string& name, ValueType&& value);

    private:

        inline void setInternal(v8::Local<v8::String> funcName, int value);

        inline void setInternal(v8::Local<v8::String> funcName, const std::string& value);

        template <class FuncReturnType, class... FuncArgs>
        void setInternal(v8::Local<v8::String> funcName, std::function<FuncReturnType(FuncArgs...)>& func);

        template<class FuncReturnType, class... FuncArgs>
        static void callCppFunctionFromJS(const v8::FunctionCallbackInfo<v8::Value>& info);

        ScriptRunResult processRunResult(v8::MaybeLocal<v8::Value>& value, v8::TryCatch& tryCatchHandler);

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
    };
}

#include "Context.inl"
