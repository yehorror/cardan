#include "ToV8.hpp"
#include "Context.hpp"

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(Context& context, int value)
    {
        return v8::Integer::New(context.getIsolate(), value);
    }

    v8::Local<v8::Value> convert(Context& context, double value)
    {
        return v8::Number::New(context.getIsolate(), value);
    }

    v8::Local<v8::Value> convert(Context& context, const std::string& value)
    {
        return v8::String::NewFromUtf8(context.getIsolate(), value.c_str()).ToLocalChecked();
    }

    v8::Local<v8::Value> convert(Context& context, v8::Local<v8::Function> function)
    {
        return function;
    }
}
