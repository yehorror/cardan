#include "Object.hpp"

namespace cardan
{
    Object::Object()
        : m_object(v8::Object::New(v8::Isolate::GetCurrent()))
        , m_context(v8::Isolate::GetCurrent()->GetCurrentContext())
    {
    }

    Object::ValueReference Object::operator [](const std::string& key)
    {
        auto v8Key = ToV8::convert(m_context, key);
        auto v8Value = m_object->Get(m_context, v8Key).ToLocalChecked();

        return ValueReference(Value(v8Value, m_context), key, *this);
    }

    Array Object::getKeys()
    {
        auto array = m_object->GetPropertyNames(m_context).ToLocalChecked();
        return Array(array, m_context);
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

    Object::Object(v8::Local<v8::Object> object, v8::Local<v8::Context>& context)
        : m_object(object)
        , m_context(context)
    {
    }

    std::pair<std::string, Value> ObjectIterator::operator *()
    {
        return m_object.getByIndex(m_idx);
    }

    ObjectIterator& ObjectIterator::operator++()
    {
        ++m_idx;
        return *this;
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
