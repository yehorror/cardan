#pragma once

#include <v8.h>

#include <string>
#include <optional>

#include "Converters/FromV8.hpp"

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
        Value(v8::Local<v8::Value> value, Context& context);

        std::string asString() const;
        int asInt() const;
        double asDouble();
        bool asBool() const;
        Array asArray();
        Object asObject();
        Function asFunction();

        template <typename Type>
        Type as()
        {
            return convert(m_context, m_value, cardan::FromV8::To<Type>{});
        }

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

        v8::Local<v8::Value> v8();

    protected:
        friend class Context;
        friend class Array;
        friend class Object;
        friend class Function;

    protected:
        v8::Local<v8::Value> m_value;
        Context& m_context;
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
