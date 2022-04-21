#include "UserDefinedConverters.hpp"

namespace UserNamespace
{
    v8::Local<v8::Value> convert(cardan::Context& context, const Person& person, cardan::ToV8::ADLTag)
    {
        auto personObject = v8::Object::New(context.getIsolate());

        personObject->Set(
            context.getContext(),
            convert(context, "name", cardan::ToV8::ADLTag{}),
            convert(context, person.name, cardan::ToV8::ADLTag{})
        ).Check();

        personObject->Set(
            context.getContext(),
            convert(context, "age", cardan::ToV8::ADLTag{}),
            convert(context, person.age, cardan::ToV8::ADLTag{})
        ).Check();

        return personObject;
    }

    Person convert(cardan::Context& context, v8::Local<v8::Value> value, cardan::FromV8::To<Person>)
    {
        Person p;

        auto object = value.As<v8::Object>();
        p.age = convert(
            context,
            object->Get(context.getContext(), convert(context, "age", cardan::ToV8::ADLTag{})).ToLocalChecked(),
            cardan::FromV8::To<int>{}
        );

        p.name = convert(
            context,
            object->Get(context.getContext(), convert(context, "name", cardan::ToV8::ADLTag{})).ToLocalChecked(),
            cardan::FromV8::To<std::string>{}
        );

        return p;
    }
}
