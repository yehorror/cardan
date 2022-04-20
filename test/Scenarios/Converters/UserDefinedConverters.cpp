#include "UserDefinedConverters.hpp"

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(Context& context, Person& person)
    {
        auto personObject = v8::Object::New(context.getIsolate());

        personObject->Set(context.getContext(), convert(context, "name"), convert(context, person.name)).Check();
        personObject->Set(context.getContext(), convert(context, "age"), convert(context, person.age)).Check();

        return personObject;
    }
}

namespace cardan::FromV8
{
    Person convert(Context& context, v8::Local<v8::Value> value, To<Person>)
    {
        Person p;

        auto object = value.As<v8::Object>();
        p.age = convert(context, object->Get(context.getContext(), ToV8::convert(context, "age")).ToLocalChecked(), To<int>{});
        p.name = convert(context, object->Get(context.getContext(), ToV8::convert(context, "name")).ToLocalChecked(), To<std::string>{});

        return p;
    }
}
