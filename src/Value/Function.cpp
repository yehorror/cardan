#include "Function.hpp"

namespace cardan
{
    Function::Function(v8::Local<v8::Function> function, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_function(function)
        , m_isolate(isolate)
        , m_context(context)
    {
    }

    Value Function::makeValue(v8::Local<v8::Value> value)
    {
        return Value(value, m_isolate, m_context);
    }

}
