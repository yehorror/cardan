#include "ScriptExecutionContext.hpp"

namespace cardan
{
    ScriptExecutionContext::ScriptExecutionContext(const std::string& src)
        : m_jsCode(src)
    {
        m_arrayBuffer = std::unique_ptr<v8::ArrayBuffer::Allocator>(v8::ArrayBuffer::Allocator::NewDefaultAllocator());

        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = m_arrayBuffer.get();
        m_isolate = v8::Isolate::New(create_params);
    }

    ScriptExecutionContext::~ScriptExecutionContext()
    {
        m_isolate->Dispose();
    }

    std::variant<std::string, int> ScriptExecutionContext::runScript()
    {
        v8::Isolate::Scope isolate_scope(m_isolate);
        v8::HandleScope    handle_scope(m_isolate);

        v8::Local<v8::Context> context = v8::Context::New(m_isolate);
        v8::Context::Scope context_scope(context);

        v8::Local<v8::String> source =
            v8::String::NewFromUtf8(m_isolate, m_jsCode.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // Dispose the isolate and tear down V8.
        if (result->IsInt32())
        {
            return result->ToInt32(context).ToLocalChecked()->Value();
        }
        else if (result->IsString())
        {
            v8::String::Utf8Value utf8String(m_isolate, result);
            return *utf8String;
        }
    }
}
