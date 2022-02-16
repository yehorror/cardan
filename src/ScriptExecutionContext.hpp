#pragma once

#include <string>
#include <variant>

#include <v8.h>
#include <libplatform/libplatform.h>

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
