#pragma once

#include "Value.hpp"
#include "Converters/ToV8.hpp"

namespace cardan
{
    class Value;
    class Object;
    class Array;
    class Function;

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

#include "Context.hpp"

namespace cardan
{
    template <class ValueType>
    void Array::set(uint32_t index, ValueType&& value)
    {
        m_array->Set(m_context.getContext(), index, ToV8::convert(m_context, std::forward<ValueType>(value), ToV8::ADLTag{})).Check();
    }
}
