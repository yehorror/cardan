#pragma once

#include "Context.hpp"

struct Person
{
    std::string name;
    int age;
};

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(cardan::Context& context, const Person& person, cardan::ToV8::ADLTag);
}


namespace cardan::FromV8
{
    Person convert(Context& context, v8::Local<v8::Value> value, To<Person>);
}
