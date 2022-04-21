#pragma once

#include "Value.hpp"
#include "Array.hpp"
#include "Function.hpp"
#include "Converters/ToV8.hpp"

namespace cardan
{
    class Value;
    class Array;
    class Object;
    class Function;
    class Context;

    class ObjectIterator
    {
    public:
        std::pair<std::string, Value> operator*();
        ObjectIterator& operator ++();
        bool operator != (const ObjectIterator& rhs) const;

    private:
        friend class Object;
        ObjectIterator(Object& object, uint32_t idx);

    private:
        Object& m_object;
        uint32_t m_idx;
    };

    class Object
    {
    public:
        using ValueReference = cardan::ValueReference<std::string, Object>;

    public:
        Object(v8::Local<v8::Object> object, Context& context);

        ValueReference operator[](const std::string& key);
        Array getKeys();
        std::pair<std::string, Value> getByIndex(uint32_t idx);

        // for iterating
        ObjectIterator begin();
        ObjectIterator end();

        template <class ValueType>
        void set(const std::string& name, ValueType&& value);

    private:
        friend class Value;

    private:
        v8::Local<v8::Object> m_object;
        Context& m_context;
    };

}

#include "Context.hpp"

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
