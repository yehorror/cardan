#pragma once

#include "Value.hpp"

namespace cardan
{
    class Value;

    class Array
    {
    public:
        Value at(int idx);
        Value operator[](int idx);
        size_t length();

    private:
        Array(v8::Local<v8::Array> array, v8::Isolate* isolate, v8::Local<v8::Context>& context);

        friend class Value;

    private:
        v8::Local<v8::Array> m_array;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
