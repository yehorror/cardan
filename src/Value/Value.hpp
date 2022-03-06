#pragma once

#include <v8.h>

#include <string>
#include <optional>

#include "Array.hpp"
#include "Object.hpp"

namespace cardan
{
    class Context;
    class Array;
    class Object;
    class Function;

    // Value represents Javascript value
    // Is valid ONLY when Context in which it was created exists
    class Value
    {
    public:
        std::string asString() const;
        int asInt() const;
        double asDouble();
        Array asArray();
        Object asObject();
        Function asFunction();

        bool isUndefined();
        bool isString() const;
        bool isInt() const;
        bool isDouble();
        bool isArray();
        bool isObject();
        bool isFunction();

    private:
        Value(v8::Local<v8::Value> value, v8::Isolate* isolate, v8::Local<v8::Context>& context);

        friend class Context;
        friend class Array;
        friend class Object;
        friend class Function;

    private:
        v8::Local<v8::Value> m_value;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
