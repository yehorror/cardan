#include "ConvertersFromV8.hpp"
#include "Value/Function.hpp"

namespace cardan::converters
{
    template <>
    int convertArgumentFromV8Value<int>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return value->Int32Value(context).ToChecked();
    }

    template <>
    std::string convertArgumentFromV8Value<std::string>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return *v8::String::Utf8Value(context->GetIsolate(), value);
    }

    template <>
    cardan::Function convertArgumentFromV8Value<cardan::Function>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return cardan::Function(value.As<v8::Function>(), context);
    }
}
