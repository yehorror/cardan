#pragma once

#include "ScriptExecutionContext.hpp"

namespace cardan
{
    class Value
    {
    public:

    private:
        Value(v8::Local<v8::Value> value);

        friend class ScriptExecutionContext;

    private:
        v8::Local<v8::Value> m_value;
    };
}
