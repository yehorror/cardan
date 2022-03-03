#pragma once

#include <v8.h>

#include <string>
#include <optional>

#include "Exceptions.hpp"
#include "Array.hpp"
#include "Object.hpp"

namespace cardan
{
    class ScriptExecutionContext;
    class Array;
    class Object;

    // Value represents Javascript value
    // Is valid ONLY when ScriptExecutionContext in which it was created exists
    class Value
    {
    public:
        std::string asString();
        int asInt() const;
        double asDouble();
        Array asArray();
        Object asObject();

        bool isUndefined();
        bool isString();
        bool isInt() const;
        bool isDouble();
        bool isArray();
        bool isObject();

    private:
        Value(v8::Local<v8::Value> value, v8::Isolate* isolate, v8::Local<v8::Context>& context);

        friend class ScriptExecutionContext;
        friend class Array;
        friend class Object;

    private:
        v8::Local<v8::Value> m_value;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
