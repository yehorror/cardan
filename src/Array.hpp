#pragma once

#include "Value.hpp"

namespace cardan
{
    class Value;
    class Object;

    class Array
    {
    public:
        Value at(int idx);
        Value operator[](int idx);
        size_t length();

        // TODO Implement iterating support (being(), end(), etc.)

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
