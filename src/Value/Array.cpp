#include "Array.hpp"

namespace cardan
{
    Array::Array()
        : m_array(v8::Array::New(v8::Isolate::GetCurrent()))
        , m_context(v8::Isolate::GetCurrent()->GetCurrentContext())
    {
    }

    Array::ValueReference Array::at(int idx)
    {
        auto v8Value = m_array->Get(m_context, idx).ToLocalChecked();

        return ValueReference(Value(v8Value, m_context), idx, *this);
    }

    Array::ValueReference Array::operator [](int idx)
    {
        return at(idx);
    }

    uint32_t Array::length()
    {
        return m_array->Length();
    }

    ArrayIterator Array::begin()
    {
        return ArrayIterator(*this, 0);
    }

    ArrayIterator Array::end()
    {
        return ArrayIterator(*this, length());
    }

    Array::Array(v8::Local<v8::Array> array, v8::Local<v8::Context>& context)
        : m_array(array)
        , m_context(context)
    {
    }

    Value ArrayIterator::operator*()
    {
        return m_array[m_idx];
    }

    ArrayIterator& ArrayIterator::operator++()
    {
        ++m_idx;
        return *this;
    }

    bool ArrayIterator::operator !=(const ArrayIterator& rhs) const
    {
        return  rhs.m_idx    != m_idx
             || &rhs.m_array != &m_array;  // also make sure iterators points to same arrays
    }

    ArrayIterator::ArrayIterator(Array& array, uint32_t idx)
        : m_array(array)
        , m_idx(idx)
    {
    }

}
