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

    ScriptExecutionContext::ScriptRunResult ScriptExecutionContext::runScript()
    {
        v8::Local<v8::String> source =
            v8::String::NewFromUtf8(m_isolate.get(), m_jsCode.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

        v8::MaybeLocal<v8::Script> scriptCompileResult = v8::Script::Compile(m_context, source);

        if (scriptCompileResult.IsEmpty())
        {
            throw InvalidJSCodeException();
        }

        v8::Local<v8::Script> script = scriptCompileResult.ToLocalChecked();

        v8::TryCatch tryCatchHandler(m_isolate.get());

        auto scriptRunResult = script->Run(m_context);

        return processRunResult(scriptRunResult, tryCatchHandler);
    }

    ScriptExecutionContext::ScriptRunResult ScriptExecutionContext::processRunResult(
        v8::MaybeLocal<v8::Value>& scriptRunResult,
        v8::TryCatch& tryCatchHandler
    )
    {
        // TODO Maybe extract some information about exception from JS and put it in JSException here?
        if (tryCatchHandler.HasCaught())
        {
            if (m_config.rethrowExceptions)
            {
                throw JSException();
            }
            //return Value(JSException(), m_isolate.get(), m_context);
        }

        auto resultValue = scriptRunResult.ToLocalChecked();

        return Value(resultValue, m_isolate.get(), m_context);
    }
}
