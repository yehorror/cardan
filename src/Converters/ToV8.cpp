#include "ToV8.hpp"
#include "Context.hpp"

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(Context& context, int value, ADLTag)
    {
        return v8::Integer::New(context.getIsolate(), value);
    }

    v8::Local<v8::Value> convert(Context& context, double value, ADLTag)
    {
        return v8::Number::New(context.getIsolate(), value);
    }

    v8::Local<v8::Value> convert(Context& context, const std::string& value, ADLTag)
    {
        return v8::String::NewFromUtf8(context.getIsolate(), value.c_str()).ToLocalChecked();
    }

    v8::Local<v8::Value> convert(Context& context, v8::Local<v8::Function> function, ADLTag)
    {
        return function;
    }
}
