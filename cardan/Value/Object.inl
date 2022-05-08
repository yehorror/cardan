#pragma once

#include "cardan/Context.hpp"

namespace cardan
{
    template <class ValueType>
    void Object::set(const std::string& name, ValueType&& value)
    {
        v8::Local<v8::Value> v8Name = convert(m_context, name, ToV8::ADLTag{});
        v8::Local<v8::Value> v8Value = convert(m_context, std::forward<ValueType>(value), ToV8::ADLTag{});

        m_object->Set(
            m_context.getContext(),
            v8Name,
            v8Value
        ).Check();
    }
}
