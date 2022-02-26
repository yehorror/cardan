#pragma once

#include <v8.h>

#include <string>
#include <optional>

#include "Exceptions.hpp"
#include "Array.hpp"

namespace cardan
{
    class ScriptExecutionContext;
    class Array;

    // Value represents Javascript value
    // Is valid ONLY when ScriptExecutionContext in which it was created exists
    class Value
    {
    public:
        std::string asString();
        int asInt();
        double asDouble();
        Array asArray();

        bool isUndefined();
        bool isString();
        bool isInt();
        bool isDouble();
        bool isArray();

    private:
        Value(v8::Local<v8::Value> value, v8::Isolate* isolate, v8::Local<v8::Context>& context);

        friend class ScriptExecutionContext;
        friend class Array;

    private:
        v8::Local<v8::Value> m_value;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
