#pragma once

#include "Value.hpp"
#include "Array.hpp"

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
        void operator++();
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

    private:

        Object(v8::Local<v8::Object> object, v8::Local<v8::Context>& context);

        friend class Value;

    private:
        v8::Local<v8::Object> m_object;
        v8::Local<v8::Context> m_context;
    };
}
