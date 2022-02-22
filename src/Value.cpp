#include "Value.hpp"

namespace cardan
{
    std::string Value::asString()
    {
        if (m_value->IsString())
        {
            v8::String::Utf8Value utf8String(m_isolate, m_value);
            return std::string(*utf8String, utf8String.length());
        }
    }

    int Value::asInt()
    {
        if (m_value->IsInt32())
        {
            return m_value->ToInt32(m_context).ToLocalChecked()->Value();
        }
    }

    double Value::asDouble()
    {
        if (m_value->IsNumber())
        {
            return m_value->ToNumber(m_context).ToLocalChecked()->Value();
        }
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

    Value::Value(v8::Local<v8::Value> value, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_value(std::move(value))
        , m_isolate(isolate)
        , m_context(context)
    {
    }
}
