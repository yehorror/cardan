#include "Object.hpp"

namespace cardan
{
    Value Object::operator[](const std::string& key)
    {
        auto v8Key = v8::String::NewFromUtf8(m_isolate, key.c_str()).ToLocalChecked();
        return Value(m_object->Get(m_context, v8Key).ToLocalChecked(), m_isolate, m_context);
    }

    Array Object::getKeys()
    {
        auto array = m_object->GetPropertyNames(m_context).ToLocalChecked();
        return Array(array, m_isolate, m_context);
    }

    Object::Object(v8::Local<v8::Object> object, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_object(object)
        , m_isolate(isolate)
        , m_context(context)
    {
    }
}
