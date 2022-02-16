#include "ScriptExecutionContext.hpp"

namespace
{
    v8::Isolate* createIsolate(v8::ArrayBuffer::Allocator* allocator)
    {
        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = allocator;

        return v8::Isolate::New(create_params);
    }
}

namespace cardan
{
    void ScriptExecutionContext::IsolateDisposer::operator() (v8::Isolate* isolate)
    {
        isolate->Dispose();
    }

    ScriptExecutionContext::ScriptExecutionContext(const std::string& src, const ScriptExecutionContextConfig& config)
        : m_jsCode(src)
        , m_arrayBufferAllocator(v8::ArrayBuffer::Allocator::NewDefaultAllocator())
        , m_isolate(createIsolate(m_arrayBufferAllocator.get()))
        , m_isolateScope(m_isolate.get())
        , m_handleScope(m_isolate.get())
        , m_context(v8::Context::New(m_isolate.get()))
        , m_contextScope(m_context)
        , m_config(config)
    {
    }

    std::variant<std::string, int, JSException> ScriptExecutionContext::runScript()
    {
        v8::Local<v8::String> source =
            v8::String::NewFromUtf8(m_isolate.get(), m_jsCode.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

        v8::Local<v8::Script> script = v8::Script::Compile(m_context, source).ToLocalChecked();

        v8::TryCatch tryCatchHandler(m_isolate.get());

        auto scriptRunResult = script->Run(m_context);

        if (tryCatchHandler.HasCaught())
        {
            if (m_config.rethrowExceptions)
            {
                throw JSException();
            }
            return JSException();
        }

        auto resultValue = scriptRunResult.ToLocalChecked();

        if (resultValue->IsInt32())
        {
            return resultValue->ToInt32(m_context).ToLocalChecked()->Value();
        }
        else if (resultValue->IsString())
        {
            v8::String::Utf8Value utf8String(m_isolate.get(), resultValue);
            return *utf8String;
        }
    }
}
