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

    std::pair<std::string, Value> Object::getByIndex(uint32_t idx)
    {
        std::string key = getKeys().at(idx).asString();
        Value value = (*this)[key];

        return {key, value};
    }

    ObjectIterator Object::begin()
    {
        return ObjectIterator(*this, 0);
    }

    ObjectIterator Object::end()
    {
        return ObjectIterator(*this, getKeys().length());
    }

    Object::Object(v8::Local<v8::Object> object, v8::Isolate* isolate, v8::Local<v8::Context>& context)
        : m_object(object)
        , m_isolate(isolate)
        , m_context(context)
    {
    }

    std::pair<std::string, Value> ObjectIterator::operator *()
    {
        return m_object.getByIndex(m_idx);
    }

    void ObjectIterator::operator++()
    {
        ++m_idx;
    }

    bool ObjectIterator::operator !=(const ObjectIterator& rhs) const
    {
        return rhs.m_idx != m_idx
            || &rhs.m_object != &m_object;
    }

    ObjectIterator::ObjectIterator(Object& object, uint32_t idx)
        : m_object(object)
        , m_idx(idx)
    {
    }

}
