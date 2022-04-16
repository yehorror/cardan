#pragma once

#include "Converters/FromV8.hpp"
#include "Converters/ToV8.hpp"

struct Person
{
    std::string name;
    int age;
};

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const Person& person);
}

namespace cardan::FromV8
{
    Person convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value, To<Person>);
}
