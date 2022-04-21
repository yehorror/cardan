#include "UserDefinedConverters.hpp"

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(Context& context, const Person& person, ToV8::ADLTag)
    {
        auto personObject = v8::Object::New(context.getIsolate());

        personObject->Set(context.getContext(), convert(context, "name", ToV8::ADLTag{}), convert(context, person.name, ToV8::ADLTag{})).Check();
        personObject->Set(context.getContext(), convert(context, "age", ToV8::ADLTag{}), convert(context, person.age, ToV8::ADLTag{})).Check();

        return personObject;
    }
}

namespace cardan::FromV8
{
    Person convert(Context& context, v8::Local<v8::Value> value, To<Person>)
    {
        Person p;

        auto object = value.As<v8::Object>();
        p.age = convert(context, object->Get(context.getContext(), convert(context, "age", ToV8::ADLTag{})).ToLocalChecked(), To<int>{});
        p.name = convert(context, object->Get(context.getContext(), convert(context, "name", ToV8::ADLTag{})).ToLocalChecked(), To<std::string>{});

        return p;
    }
}
