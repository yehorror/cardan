#pragma once

#include "Value.hpp"

namespace cardan
{
    class Value;

    class Object
    {
    public:

        Value operator[](const std::string& key);

    private:

        Object(v8::Local<v8::Object> object, v8::Isolate* isolate, v8::Local<v8::Context>& context);

        friend class Value;

    private:
        v8::Local<v8::Object> m_object;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
