#pragma once

#include "cardan/Value/Value.hpp"

namespace cardan
{
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

        v8::Local<v8::Object> v8();

    private:
        friend class Value;

    private:
        v8::Local<v8::Object> m_object;
        Context& m_context;
    };
}

#include "Object.inl"
