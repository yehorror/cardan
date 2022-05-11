#include "Object.hpp"
#include "cardan/Context.hpp"

namespace cardan
{
    Object::ValueReference Object::operator [](const std::string& key)
    {
        auto v8Key = convert(m_context, key, ToV8::ADLTag{});
        auto v8Value = m_object->Get(m_context.getContext(), v8Key).ToLocalChecked();

        return ValueReference(Value(v8Value, m_context), key, *this);
    }

    Array Object::getKeys()
    {
        auto array = m_object->GetPropertyNames(m_context.getContext()).ToLocalChecked();
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

    v8::Local<v8::Object> Object::v8()
    {
        return m_object;
    }

    Object::Object(v8::Local<v8::Object> object, Context& context)
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
