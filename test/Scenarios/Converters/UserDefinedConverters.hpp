#pragma once

#include "cardan/Context.hpp"

namespace UserNamespace
{
    struct Person
    {
        std::string name;
        int age;
    };

    v8::Local<v8::Value> convert(cardan::Context& context, const Person& person, cardan::ToV8::ADLTag);

    Person convert(cardan::Context& context, v8::Local<v8::Value> value, cardan::FromV8::To<Person>);
}
