#pragma once

#include "Converters/ToV8.hpp"
#include "Value/Value.hpp"
#include "Value/Array.hpp"
#include "Value/Object.hpp"
#include "Value/Function.hpp"
#include "Helper.hpp"

namespace cardan
{
    template <class ValueType>
    void Context::set(const std::string& name, ValueType&& value)
    {
        v8::Local<v8::Value> v8Name = convert(*this, name, ToV8::ADLTag{});
        v8::Local<v8::Value> v8Value = convert(*this, std::forward<ValueType>(value), ToV8::ADLTag{});

        m_context->Global()->Set(
            m_context,
            v8Name,
            v8Value
        ).Check();
    }

    template <class ValueType>
    Value Context::makeValue(ValueType&& value)
    {
        v8::Local<v8::Value> v8Value = convert(*this, std::forward<ValueType>(value), ToV8::ADLTag{});
        return Value(v8Value, *this);
    }

    template <class FunctionWithContextType>
    void Context::saveFunction(std::unique_ptr<FunctionWithContextType> functionWithContext)
    {
        m_functions.emplace(std::move(functionWithContext));
    }
}
