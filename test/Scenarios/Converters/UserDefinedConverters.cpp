#include "UserDefinedConverters.hpp"

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const Person& person)
    {
        auto personObject = v8::Object::New(context->GetIsolate());

        personObject->Set(context, convert(context, "name"), convert(context, person.name)).Check();
        personObject->Set(context, convert(context, "age"), convert(context, person.age)).Check();

        return personObject;
    }
}

namespace cardan::FromV8
{
    template <>
    Person convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        Person p;

        auto object = value.As<v8::Object>();
        p.age = convert<int>(context, object->Get(context, ToV8::convert(context, "age")).ToLocalChecked());
        p.name = convert<std::string>(context, object->Get(context, ToV8::convert(context, "name")).ToLocalChecked());

        return p;
    }
}
