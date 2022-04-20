#include <stdexcept>
#include "Value.hpp"
#include "Object.hpp"
#include "Function.hpp"

namespace cardan
{
    std::string Value::asString() const
    {
        if (isString())
        {
            v8::String::Utf8Value utf8String(m_context.getIsolate(), m_value);
            return std::string(*utf8String, utf8String.length());
        }
        throw std::runtime_error("Invalid value type");
    }

    int Value::asInt() const
    {
        if (isInt())
        {
            return m_value->ToInt32(m_context.getContext()).ToLocalChecked()->Value();
        }
        throw std::runtime_error("Invalid value type");
    }

    double Value::asDouble()
    {
        if (isDouble())
        {
            return m_value->ToNumber(m_context.getContext()).ToLocalChecked()->Value();
        }
        throw std::runtime_error("Invalid value type");
    }

    bool Value::asBool() const
    {
        if (isBool())
        {
            return m_value->ToBoolean(m_context.getIsolate())->Value();
        }
        throw std::runtime_error("Invalid value type");
    }

    Array Value::asArray()
    {
        if (isArray())
        {
            return Array(m_value.As<v8::Array>(), m_context);
        }
        throw std::runtime_error("Invalid value type");
    }

    Object Value::asObject()
    {
        if (isObject())
        {
            return Object(m_value.As<v8::Object>(), m_context);
        }
        throw std::runtime_error("Invalid value type");
    }

    Function Value::asFunction()
    {
        if (isFunction())
        {
            return Function(m_value.As<v8::Function>(), m_context);
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

    bool Value::isBool() const
    {
        return m_value->IsBoolean();
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

    bool Value::isNull()
    {
        return m_value->IsNull();
    }

    Value& Value::operator =(int newValue)
    {
        m_value = v8::Integer::New(m_context.getIsolate(), newValue);
        return *this;
    }

    Value::Value(v8::Local<v8::Value> value, Context& context)
        : m_value(std::move(value))
        , m_context(context)
    {
    }

    template <class IndexType, class ContainerType>
    ValueReference<IndexType, ContainerType>::ValueReference(cardan::Value value, IndexType index, ContainerType& parentContainer)
        : Value(value)
        , m_index(index)
        , m_parentContainer(parentContainer)
    {
    }

    template class ValueReference<uint32_t, Array>;
    template class ValueReference<std::string, Object>;
}
