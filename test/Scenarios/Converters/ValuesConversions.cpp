#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "UserDefinedConverters.hpp"
#include "Context.hpp"

using namespace testing;

/* Example of a conversion from C++ types to JS values
 *
 * There is defined converter for a Person struct to V8 value in UserDefinedConverters:
  * v8::Local<v8::Value> cardan::ToV8::convert(v8::Local<v8::Context> context, const Person& person)
 * This implementation converts structure to a JSON object.
 * With this converter we are able to create values in JS context without boilerplate Object creation.
 */
TEST(ValuesConversions, ConverterForStructureDefinedByUser_StructureCanBeSetToJSValue)
{
    Person person;

    person.name = "John";
    person.age = 35;

    cardan::Context ctx;
    ctx.set("person", person);

    cardan::Object personObject = ctx.get("person").asObject();

    EXPECT_EQ(person.name, personObject["name"].asString());
    EXPECT_EQ(person.age,  personObject["age"].asInt());
}

/* Example of a conversion from JS values to native C++ types
 *
 * There is a converter from JS object to Person structure defined in UserDefinedConverters:
 *  template <>
 *  Person cardan::FromV8::convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
 * It allows us conversion of these values during, e.g., native C++ functions calls.
 * E.g., C++ function accepts Person as a parameter.
 * During C++ function call from JS code object passed as parameter will be automatically converted to a Person struct
 */
TEST(ValuesConversions, ConverterFromJSObjectToStructureDefinedByUser_ValueCanBeReceivedViaCppFunctionCallFromJSCode)
{
    const std::string JS = R"JS(

        var somePerson = {
            name: "Lester",
            age: 45
        };

        cppFunction(somePerson);

    )JS";

    cardan::Context ctx;

    MockFunction<void(Person)> mockFunction;
    auto stdMockFunction = mockFunction.AsStdFunction();

    ctx.set("cppFunction", stdMockFunction);

    EXPECT_CALL(mockFunction, Call(_)).WillOnce([] (const Person& p) {
        EXPECT_EQ("Lester", p.name);
        EXPECT_EQ(45, p.age);
    });

    ctx.runScript(JS);
}
