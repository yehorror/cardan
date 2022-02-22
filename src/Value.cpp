#include <stdexcept>
#include "Value.hpp"

namespace cardan
{
    std::string Value::asString()
    {
        if (isString())
        {
            v8::String::Utf8Value utf8String(m_isolate, m_value);
            return std::string(*utf8String, utf8String.length());
        }
        throw std::runtime_error("Invalid value type");
    }

    int Value::asInt()
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

    bool Value::isUndefined()
    {
        return m_value->IsUndefined();
    }

    bool Value::isString()
    {
        return m_value->IsString();
    }

    bool Value::isInt()
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

    Value::Value(v8::Local<v8::Value> value, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_value(std::move(value))
        , m_isolate(isolate)
        , m_context(context)
    {
    }

    Array::Array(v8::Local<v8::Array> array, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_array(array)
        , m_isolate(isolate)
        , m_context(context)
    {
    }

}
