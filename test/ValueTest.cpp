#include <gtest/gtest.h>
#include "ScriptExecutionContext.hpp"
#include "Value.hpp"

using namespace testing;
using namespace cardan;

TEST(ValueTest, CreateStringValue_ValueStoresString)
{
    ScriptExecutionContext ctx(" 'string value' ");
    Value value = ctx.runScript();

    EXPECT_TRUE(value.isString());
}

TEST(ValueTest, CreateStringvalue_asStringReturnsValueWhichItWasCreatedWith)
{
    ScriptExecutionContext ctx(" 'string value' ");
    Value value = ctx.runScript();

    ASSERT_TRUE(value.isString());
    EXPECT_EQ("string value", value.asString());
}

TEST(ValueTest, CreateIntegerValue_ValueStoresInt)
{
    ScriptExecutionContext ctx("123");
    Value value = ctx.runScript();

    EXPECT_TRUE(value.isInt());
}

TEST(ValueTest, CreateIntegerValue_asIntReturnsValueWhichItWasCreatedWith)
{
    ScriptExecutionContext ctx("123");
    Value value = ctx.runScript();

    ASSERT_TRUE(value.isInt());
    EXPECT_EQ(123, value.asInt());
}

TEST(ValueTest, CreateDoubleValue_ValueStoresDouble)
{
    ScriptExecutionContext ctx("123.456");
    Value value = ctx.runScript();

    EXPECT_TRUE(value.isDouble());
}

TEST(ValueTest, CreateDoubleValue_asDoubleReturnsValueWhichItWasCreatedWith)
{
    ScriptExecutionContext ctx("123.456");
    Value value = ctx.runScript();

    ASSERT_TRUE(value.isDouble());
    EXPECT_DOUBLE_EQ(123.456, value.asDouble());
}

TEST(ValueTest, CreateUndefinedValue_isUndefinedReturnsTrue)
{
    ScriptExecutionContext ctx("undefined");
    auto result = ctx.runScript();

    EXPECT_TRUE(result.isUndefined());
}
