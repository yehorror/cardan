#include "Function.hpp"

namespace cardan
{
    Function::Function(v8::Local<v8::Function> function, Context& context)
        : m_function(function)
        , m_context(context)
    {
    }

    v8::Local<v8::Function> Function::v8()
    {
        return m_function;
    }
}
