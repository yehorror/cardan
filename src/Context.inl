#pragma once

#include "Context.hpp"
#include "Converters.hpp"

namespace cardan
{
    template <class ValueType>
    void Context::set(const std::string& name, ValueType&& value)
    {
        v8::Local<v8::String> v8Name = v8::String::NewFromUtf8(m_isolate.get(), name.c_str()).ToLocalChecked();
        v8::Local<v8::Value> v8Value = converters::convert(m_isolate.get(), m_context, std::forward<ValueType>(value));

        m_context->Global()->Set(
            m_context,
            v8Name,
            v8Value
        ).Check();
    }
}
