#include "Function.hpp"

namespace cardan
{
    Value Function::call()
    {
        auto result = m_function->Call(m_context, m_context->Global(), 0, nullptr).ToLocalChecked();

        return Value(result, m_isolate, m_context);
    }

    Function::Function(v8::Local<v8::Function> function, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_function(function)
        , m_isolate(isolate)
        , m_context(context)
    {
    }

}
