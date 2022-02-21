#include "Value.hpp"

namespace cardan
{
    Value::Value(v8::Local<v8::Value> value)
        : m_value(std::move(value))
    {
    }
}
