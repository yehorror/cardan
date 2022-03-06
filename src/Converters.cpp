#include "Converters.hpp"

namespace cardan::converters
{
    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> /*context*/, int value)
    {
        return v8::Integer::New(isolate, value);
    }

    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> /*context*/, double value)
    {
        return v8::Number::New(isolate, value);
    }

    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> /*context*/, const std::string& value)
    {
        return v8::String::NewFromUtf8(isolate, value.c_str()).ToLocalChecked();
    }
}
