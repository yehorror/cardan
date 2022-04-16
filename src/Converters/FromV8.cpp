#include <stdexcept>

#include "FromV8.hpp"
#include "Value/Function.hpp"

namespace cardan::FromV8
{
    int convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<int>)
    {
        if (value->IsNumber())
        {
            return value->Int32Value(context).ToChecked();
        }
        throw std::runtime_error("Convertible value is not an integer");
    }

    double convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<double>)
    {
        if (value->IsNumber())
        {
            return value->NumberValue(context).ToChecked();
        }
        throw std::runtime_error("Convertible value is not a double");
    }

    std::string convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<std::string>)
    {
        return *v8::String::Utf8Value(context->GetIsolate(), value);
    }

    cardan::Function convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<cardan::Function>)
    {
        return cardan::Function(value.As<v8::Function>(), context);
    }
}
