#include <stdexcept>
#include "Value.hpp"
#include "Function.hpp"

namespace cardan
{
    std::string Value::asString() const
    {
        if (isString())
        {
            v8::String::Utf8Value utf8String(m_isolate, m_value);
            return std::string(*utf8String, utf8String.length());
        }
        throw std::runtime_error("Invalid value type");
    }

    int Value::asInt() const
    {
        if (isInt())
        {
            return m_value->ToInt32(m_context).ToLocalChecked()->Value();
        }
        throw std::runtime_error("Invalid value type");
    }

    double Value::asDouble()
    {
        if (isDouble())
        {
            return m_value->ToNumber(m_context).ToLocalChecked()->Value();
        }
        throw std::runtime_error("Invalid value type");
    }

    Array Value::asArray()
    {
        if (isArray())
        {
            return Array(m_value.As<v8::Array>(), m_isolate, m_context);
        }
        throw std::runtime_error("Invalid value type");
    }

    Object Value::asObject()
    {
        if (isObject())
        {
            return Object(m_value.As<v8::Object>(), m_isolate, m_context);
        }
        throw std::runtime_error("Invalid value type");
    }

    Function Value::asFunction()
    {
        if (isFunction())
        {
            return Function(m_value.As<v8::Function>(), m_isolate, m_context);
        }
        throw std::runtime_error("Invalid value type");
    }

    bool Value::isUndefined()
    {
        return m_value->IsUndefined();
    }

    bool Value::isString() const
    {
        return m_value->IsString();
    }

    bool Value::isInt() const
    {
        return m_value->IsInt32();
    }

    bool Value::isDouble()
    {
        return m_value->IsNumber();
    }

    bool Value::isArray()
    {
        return m_value->IsArray();
    }

    bool Value::isObject()
    {
        return m_value->IsObject();
    }

    bool Value::isFunction()
    {
        return m_value->IsFunction();
    }

    Value::Value(v8::Local<v8::Value> value, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_value(std::move(value))
        , m_isolate(isolate)
        , m_context(context)
    {
    }
}
