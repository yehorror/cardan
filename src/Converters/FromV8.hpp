#pragma once

#include <v8.h>
#include <type_traits>

namespace cardan
{
    class Function;
    class Context;
}

namespace cardan::FromV8
{
    // This struct is used to force compiler to use ADL on converters lookup
    template <typename T>
    struct To
    {
    };

    int convert(Context& context, v8::Local<v8::Value> value, To<int>);
    double convert(Context& context, v8::Local<v8::Value> value, To<double>);
    float convert(Context& context, v8::Local<v8::Value> value, To<float>);
    std::string convert(Context& context, v8::Local<v8::Value> value, To<std::string>);
    cardan::Function convert(Context& context, v8::Local<v8::Value> value, To<cardan::Function>);
}
