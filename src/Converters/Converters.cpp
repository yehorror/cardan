#include "Converters.hpp"
#include "Value/Function.hpp"

namespace cardan::converters
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, int value)
    {
        return v8::Integer::New(context->GetIsolate(), value);
    }

    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, double value)
    {
        return v8::Number::New(context->GetIsolate(), value);
    }

    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const std::string& value)
    {
        return v8::String::NewFromUtf8(context->GetIsolate(), value.c_str()).ToLocalChecked();
    }

    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, Function value)
    {
        return value.m_function;
    }
}
