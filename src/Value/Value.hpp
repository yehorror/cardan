#pragma once

#include <v8.h>

#include <string>
#include <optional>

namespace cardan
{
    class Context;
    class Array;
    class Object;
    class Function;

    // Value represents Javascript value
    // Is valid ONLY when Context in which it was created exists
    class Value
    {
    public:
        std::string asString() const;
        int asInt() const;
        double asDouble();
        bool asBool() const;
        Array asArray();
        Object asObject();
        Function asFunction();

        bool isUndefined();
        bool isString() const;
        bool isInt() const;
        bool isDouble();
        bool isBool() const;
        bool isArray();
        bool isObject();
        bool isFunction();
        bool isNull();

        Value& operator = (int newValue);

    protected:
        Value(v8::Local<v8::Value> value, v8::Local<v8::Context>& context);

        friend class Context;
        friend class Array;
        friend class Object;
        friend class Function;

    protected:
        v8::Local<v8::Value> m_value;
        v8::Local<v8::Context> m_context;
    };

    template <class IndexType, class ContainerType>
    class ValueReference : public Value
    {
    public:

        template <class ValueType>
        ValueReference& operator = (ValueType&& value)
        {
            m_parentContainer.set(m_index, std::forward<ValueType>(value));
            Value::m_value = m_parentContainer[m_index].m_value;
            return *this;
        }

    private:
        ValueReference(Value value, IndexType index, ContainerType& parentContainer);

        friend class Array;
        friend class Object;

    private:
        const IndexType m_index;
        ContainerType& m_parentContainer;
    };
}
