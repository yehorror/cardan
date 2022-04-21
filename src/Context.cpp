#include "Context.hpp"
#include "Value/Object.hpp"

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
    void Context::IsolateDisposer::operator() (v8::Isolate* isolate)
    {
        isolate->Dispose();
    }

    Context::Context()
        : m_arrayBufferAllocator(v8::ArrayBuffer::Allocator::NewDefaultAllocator())
        , m_isolate(createIsolate(m_arrayBufferAllocator.get()))
        , m_isolateScope(m_isolate.get())
        , m_handleScope(m_isolate.get())
        , m_context(v8::Context::New(m_isolate.get()))
        , m_contextScope(m_context)
    {
    }

    Context::ScriptRunResult Context::runScript(const std::string& code)
    {
        v8::Local<v8::String> source = convert(*this, code, ToV8::ADLTag{}).As<v8::String>();

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

    Value Context::get(const std::string& valueName)
    {
        v8::Local<v8::Value> valueNameV8 = convert(*this, valueName, ToV8::ADLTag{});
        v8::Local<v8::Value> value = m_context->Global()->Get(m_context, valueNameV8).ToLocalChecked();

        return Value(value, *this);
    }

    Object Context::global()
    {
        return Value(m_context->Global(), *this).asObject();
    }

    v8::Isolate* Context::getIsolate()
    {
        return m_isolate.get();
    }

    v8::Local<v8::Context> Context::getContext()
    {
        return m_context;
    }

    Context::ScriptRunResult Context::processRunResult(
        v8::MaybeLocal<v8::Value>& scriptRunResult,
        v8::TryCatch& tryCatchHandler
    )
    {
        // TODO Store whole exception object within JSException class
        if (tryCatchHandler.HasCaught())
        {
            Object exception(
                tryCatchHandler.Exception().As<v8::Object>(),
                *this
            );

            throw JSException(exception["message"].as<std::string>());
        }

        auto resultValue = scriptRunResult.ToLocalChecked();

        return Value(resultValue, *this);
    }
}
