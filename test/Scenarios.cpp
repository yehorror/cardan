#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "v8.h"

struct Person;

namespace cardan::converters
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const Person& person);
}

#include "Context.hpp"

using namespace testing;
using namespace cardan;

TEST(Scenarios, ScriptHasMultipleFunctionsDeclarations_GetEachOfThem_CallEachFunction_ResultsCorrespondsWithReturnValues)
{
    const std::string JS = R"JS(
        function intValueFunc() {
            return 123;
        }

        function stringValueFunc() {
            return 'string';
        }
    )JS";

    Context context;
    context.runScript(JS);

    Function intValueFunc = context.get("intValueFunc").asFunction();
    Function stringValueFunc = context.get("stringValueFunc").asFunction();

    EXPECT_EQ(123,      intValueFunc.call().asInt());
    EXPECT_EQ("string", stringValueFunc.call().asString());
}

TEST(Scenarios, ScriptHasFunctionWhichCallToCppFunction_CallThisFunction_CppFunctionIsCalled)
{
    const std::string JS = R"JS(

        function callCppFunction() {
            cppFunction();
        }

    )JS";

    MockFunction<void()> cppMockFunction;
    auto stdFunction = cppMockFunction.AsStdFunction();

    Context context;
    context.set("cppFunction", stdFunction);
    context.runScript(JS);

    Function jsFunction = context.get("callCppFunction").asFunction();

    EXPECT_CALL(cppMockFunction, Call());

    jsFunction.call();
}

TEST(Scenarios, CppFunctionCallsBackToJS_CallThisFunctionFromJSCode_CallbackFunctionCalled)
{
    const std::string JS = R"JS(

        var functionWasCalled = false;

        function callback() {
            functionWasCalled = true;
        }

        cppFunctionWithCallback(callback);

    )JS";

    std::function<void(Function)> cppFunction = [] (Function callback)
    {
        callback.call();
    };

    Context context;
    context.set("cppFunctionWithCallback", cppFunction);
    context.runScript(JS);

    auto wasFunctionCalled = context.get("functionWasCalled");

    EXPECT_EQ(true, wasFunctionCalled.asBool());
}

struct Person
{
    std::string name;
    int age;
};

namespace cardan::converters
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const Person& person)
    {
        auto personObject = v8::Object::New(context->GetIsolate());

        personObject->Set(context, convert(context, "name"), convert(context, person.name)).Check();
        personObject->Set(context, convert(context, "age"), convert(context, person.age)).Check();

        return personObject;
    }

    template <>
    Person convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    {
        Person p;
        auto object = value.As<v8::Object>();
        p.age = convertArgumentFromV8Value<int>(context, object->Get(context, convert(context, "age")).ToLocalChecked());
        p.name = convertArgumentFromV8Value<std::string>(context, object->Get(context, convert(context, "name")).ToLocalChecked());
        return p;
    }

}

TEST(Scenarios, ConverterForStructureDefinedByUser_StructureCanBeSetToJSValue)
{
    Person person {"John", 42};

    Context ctx;
    ctx.set("person", person);

    Object personObject = ctx.get("person").asObject();

    EXPECT_EQ(person.name, personObject["name"].asString());
    EXPECT_EQ(person.age,  personObject["age"].asInt());
}

TEST(Scenarios, ConverterFromJSObjectToStructureDefinedByUser_ValueCanBeReceivedViaCppFunctionCallFromJSCode)
{
    const std::string JS = R"JS(

        var somePerson = {
            name: "Lester",
            age: 45
        };

        cppFunction(somePerson);

    )JS";

    Context ctx;

    MockFunction<void(Person)> mockFunction;
    auto stdMockFunction = mockFunction.AsStdFunction();

    ctx.set("cppFunction", stdMockFunction);

    EXPECT_CALL(mockFunction, Call(_)).WillOnce([] (Person p) {
        EXPECT_EQ("Lester", p.name);
        EXPECT_EQ(45, p.age);
    });

    ctx.runScript(JS);
}
