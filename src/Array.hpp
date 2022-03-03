#pragma once

#include "Value.hpp"

namespace cardan
{
    class Value;
    class Object;
    class Array;

    class ArrayIterator
    {
    public:
        Value operator*();
        void operator++();
        bool operator != (const ArrayIterator& rhs) const;

    private:
        friend class Array;
        ArrayIterator(Array& array, uint32_t idx);

    private:
        Array& m_array;
        uint32_t m_idx;
    };

    class Array
    {
    public:
        Value at(int idx);
        Value operator[](int idx);
        size_t length();

        // for iterating:
        ArrayIterator begin();
        ArrayIterator end();

    private:
        Array(v8::Local<v8::Array> array, v8::Isolate* isolate, v8::Local<v8::Context>& context);

        friend class Value;
        friend class Object;

    private:
        v8::Local<v8::Array> m_array;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
