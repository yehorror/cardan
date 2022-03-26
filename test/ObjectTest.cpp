#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Context.hpp"
#include "Value/Object.hpp"

using namespace testing;
using namespace cardan;

namespace
{
    // Values are valid objects untill context is destroyed
    // So, we need to keep it alive during testing values
    struct ObjectWithContext
    {
        Context context;
        Object object;

        ObjectWithContext(const std::string& jsCode)
            : object(context.runScript(jsCode).asObject())
        {
        }
    };

    ObjectWithContext makeObjectFromJSCode(const std::string& jsCode)
    {
        return ObjectWithContext(jsCode);
    }
}

TEST(ObjectTest, CreateObjectWithOneField_AccessViaStringKeyWithSquaredBrackets_ValueByThisKeyIsReturned)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("{\"some_key\": \"some_value\"}") )");
    auto value = object["some_key"];

    ASSERT_TRUE(value.isString());
    EXPECT_EQ("some_value", value.asString());
}

TEST(ObjectTest, CreateObjectWithMultipleFields_AccessTheseFields_ValuesCorresponds)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("\
    {\
        \"name\": \"John Smith\",\
        \"age\":  35\
    }"
    ))");
    auto nameValue = object["name"];
    auto ageValue = object["age"];

    ASSERT_TRUE(nameValue.isString());
    EXPECT_EQ("John Smith", nameValue.asString());

    ASSERT_TRUE(ageValue.isInt());
    EXPECT_EQ(35, ageValue.asInt());
}

TEST(ObjectTest, CreateObjectWithMultipleFields_getKeys_ReturnArrayOfKeys)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("\
    {\
        \"name\": \"John Smith\",\
        \"age\":  35\
    }"
    ))");

    auto keysArray = object.getKeys();

    ASSERT_EQ(2, keysArray.length());
    EXPECT_EQ("name", keysArray[0].asString());
    EXPECT_EQ("age", keysArray[1].asString());
}

TEST(ObjectTest, CreateObjectWithMultipleFields_IterateViaForEach_PredicateCalledWithCorrespondingValues)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("\
    {\
        \"name\": \"John Smith\",\
        \"age\":  35\
    }"
    ))");

    MockFunction<void(const std::pair<std::string, cardan::Value>&)> mockPredicate;

    {
        InSequence seq;
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce([](const std::pair<std::string, cardan::Value>& field)
        {
            EXPECT_EQ("name", field.first);
            EXPECT_EQ("John Smith", field.second.asString());
        });

        EXPECT_CALL(mockPredicate, Call(_)).WillOnce([](const std::pair<std::string, cardan::Value>& field)
        {
            EXPECT_EQ("age", field.first);
            EXPECT_EQ(35, field.second.asInt());
        });
    }

    std::for_each(object.begin(), object.end(), mockPredicate.AsStdFunction());
}

TEST(ObjectTest, CreateObjectWithMultipleFields_getByIndex_ReturnFildsByTheirIndices)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("\
    {\
        \"name\": \"John Smith\",\
        \"age\":  35\
    }"
    ))");

    auto [firstFieldName, firstFieldValue] = object.getByIndex(0);
    auto [secondFieldName, secondFieldValue] = object.getByIndex(1);

    EXPECT_EQ("name", firstFieldName);
    EXPECT_EQ("John Smith", firstFieldValue.asString());

    EXPECT_EQ("age", secondFieldName);
    EXPECT_EQ(35, secondFieldValue.asInt());
}

TEST(ObjectTest, CreateEmptyObject_setIntegerValueBySomeName_getValueByThatNameReturnsValueWhichWasSet)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("{}") )");

    object.set("some_value", 23);

    EXPECT_EQ(23, object["some_value"].asInt());
}

TEST(ObjectTest, CreateEmptyObject_setStringValueBySomeName_getValueByThatNameReturnsValueWhichWasSet)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("{}") )");

    object.set("some_value", "some string value");

    EXPECT_EQ("some string value", object["some_value"].asString());
}

TEST(ObjectTest, CreateEmptyObject_setDoubleValueBySomeName_getValueByThatNameReturnsValueWhichWasSet)
{
    auto [ctx, object] = makeObjectFromJSCode(R"( JSON.parse("{}") )");

    object.set("some_value", 2.71828);

    EXPECT_DOUBLE_EQ(2.71828, object["some_value"].asDouble());
}

TEST(ObjectTest, CreateContext_CreateObject_setSomeValue_valueWhichWasSetCanBeRetrieved)
{
    Context ctx;

    ctx.runScript("");

    Object object;

    object.set("name", "John Smith");

    EXPECT_EQ("John Smith", object["name"].asString());
}

TEST(ObjectTest, CreateContext_CreateObject_setSomeValueViaAssignment_valueWhichWasSetCanBeRetrieved)
{
    Context ctx;

    Object object;

    object["age"] = 35;

    EXPECT_EQ(35, object["age"].asInt());
}

TEST(ObjectTest, CreateContext_CreateObject_getValueViaSquaredBrackets_setNewValueToIt_ItHasValueWhichWasSet)
{
    Context ctx;

    Object object;
    object["age"] = 35;

    Object::ValueReference ageValueRef = object["age"];
    ageValueRef = 27;

    EXPECT_EQ(27, ageValueRef.asInt());
}
