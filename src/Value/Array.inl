#pragma once

#include "Context.hpp"

namespace cardan
{
    template <class ValueType>
    void Array::set(uint32_t index, ValueType&& value)
    {
        m_array->Set(
            m_context.getContext(),
            index,
            ToV8::convert(m_context, std::forward<ValueType>(value), ToV8::ADLTag{})
        ).Check();
    }
}
