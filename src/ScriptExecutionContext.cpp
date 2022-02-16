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

    ScriptExecutionContext::ScriptExecutionContext(const std::string& src)
        : m_jsCode(src)
        , m_arrayBufferAllocator(v8::ArrayBuffer::Allocator::NewDefaultAllocator())
        , m_isolate(createIsolate(m_arrayBufferAllocator.get()))
        , m_isolateScope(m_isolate.get())
        , m_handleScope(m_isolate.get())
        , m_context(v8::Context::New(m_isolate.get()))
        , m_contextScope(m_context)
    {
    }

    std::variant<std::string, int> ScriptExecutionContext::runScript()
    {
        v8::Local<v8::String> source =
            v8::String::NewFromUtf8(m_isolate.get(), m_jsCode.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

        v8::Local<v8::Script> script = v8::Script::Compile(m_context, source).ToLocalChecked();

        v8::Local<v8::Value> result = script->Run(m_context).ToLocalChecked();

        if (result->IsInt32())
        {
            return result->ToInt32(m_context).ToLocalChecked()->Value();
        }
        else if (result->IsString())
        {
            v8::String::Utf8Value utf8String(m_isolate.get(), result);
            return *utf8String;
        }
    }
}
