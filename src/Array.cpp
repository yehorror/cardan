#include "Array.hpp"

namespace cardan
{
    Value Array::at(int idx)
    {
        return Value(m_array->Get(m_context, idx).ToLocalChecked(), m_isolate, m_context);
    }

    Value Array::operator[](int idx)
    {
        return at(idx);
    }

    size_t Array::length()
    {
        // TODO This method returns uint32; should we use size_t as return type of this function?
        return m_array->Length();
    }

    Array::Array(v8::Local<v8::Array> array, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_array(array)
        , m_isolate(isolate)
        , m_context(context)
    {
    }
}
