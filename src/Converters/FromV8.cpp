#include "FromV8.hpp"
#include "Value/Function.hpp"

namespace cardan::FromV8
{
    template <>
    int convert<int>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return value->Int32Value(context).ToChecked();
    }

    template <>
    double convert<double>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return value->NumberValue(context).ToChecked();
    }

    template <>
    std::string convert<std::string>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return *v8::String::Utf8Value(context->GetIsolate(), value);
    }

    template <>
    cardan::Function convert<cardan::Function>(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        return cardan::Function(value.As<v8::Function>(), context);
    }
}
