#include <gtest/gtest.h>
#include "ScriptExecutionContext.hpp"
#include "Object.hpp"

using namespace testing;
using namespace cardan;

namespace {
    // Values are valid objects untill context is destroyed
    // So, we need to keep it alive during testing values
    struct ObjectWithContext
    {
        ScriptExecutionContext context;
        Object object;

        ObjectWithContext(const std::string& jsCode)
            : context(jsCode)
            , object(context.runScript().asObject())
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
