#pragma once

#include <v8.h>
#include <type_traits>

namespace cardan
{
    class Function;
}

namespace cardan::FromV8
{
    template <typename T>
    struct To
    {
    };

    int convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<int>);
    double convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<double>);
    std::string convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<std::string>);
    cardan::Function convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<cardan::Function>);
}
