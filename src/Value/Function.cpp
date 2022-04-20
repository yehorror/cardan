#include "Function.hpp"

namespace cardan
{
    Function::Function(v8::Local<v8::Function> function, Context& context)
        : m_function(function)
        , m_context(context)
    {
    }

    Value Function::makeValue(v8::Local<v8::Value> value)
    {
        return Value(value, m_context);
    }

}
