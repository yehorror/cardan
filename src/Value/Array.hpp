#pragma once

#include "Value/Value.hpp"

namespace cardan
{
    class ArrayIterator
    {
    public:
        Value operator*();
        ArrayIterator& operator++();
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
        using ValueReference = cardan::ValueReference<uint32_t, Array>;

    public:
        ValueReference at(int idx);
        ValueReference operator[](int idx);
        uint32_t length();

        // for iterating:
        ArrayIterator begin();
        ArrayIterator end();

        template <class ValueType>
        void set(uint32_t index, ValueType&& value);

    private:
        Array(v8::Local<v8::Array> array, Context& context);

        friend class Value;
        friend class Object;

    private:
        v8::Local<v8::Array> m_array;
        Context& m_context;
    };
}

#include "Array.inl"
