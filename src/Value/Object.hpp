#pragma once

#include "Value.hpp"
#include "Array.hpp"
#include "Function.hpp"
#include "Converters/Converters.hpp"

namespace cardan
{
    class Value;
    class Array;
    class Object;
    class Function;

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
        Value operator[](const std::string& key);
        Array getKeys();
        std::pair<std::string, Value> getByIndex(uint32_t idx);

        // for iterating
        ObjectIterator begin();
        ObjectIterator end();

        template <class ValueType>
        void set(const std::string& name, ValueType&& value);

    private:

        Object(v8::Local<v8::Object> object, v8::Local<v8::Context>& context);

        friend class Value;

    private:
        v8::Local<v8::Object> m_object;
        v8::Local<v8::Context> m_context;
    };

    template <class ValueType>
    void Object::set(const std::string& name, ValueType&& value)
    {
        v8::Local<v8::String> v8Name = v8::String::NewFromUtf8(m_context->GetIsolate(), name.c_str()).ToLocalChecked();
        v8::Local<v8::Value> v8Value = converters::convert(m_context, std::forward<ValueType>(value));

        m_object->Set(
            m_context,
            v8Name,
            v8Value
        ).Check();
    }
}
