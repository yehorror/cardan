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

TEST(ValueTest, CreateStringValue_TryToGetDouble_ExceptionOfWrongTypeIsThrown)
{
    ScriptExecutionContext ctx(" 'definitely not a double' ");
    Value value = ctx.runScript();

    ASSERT_FALSE(value.isDouble());
    EXPECT_THROW(value.asDouble(), std::runtime_error);
}

TEST(ValueTest, CreateStringValue_TryToGetInt_ExceptionOfWrongTypeIsThrown)
{
    ScriptExecutionContext ctx(" 'definitely not an integer' ");
    Value value = ctx.runScript();

    ASSERT_FALSE(value.isInt());
    EXPECT_THROW(value.asInt(), std::runtime_error);
}

TEST(ValueTest, CreateIntValue_TryToGetString_ExceptionOfWrongTypeIsThrown)
{
    ScriptExecutionContext ctx("123");
    Value value = ctx.runScript();

    ASSERT_FALSE(value.isString());
    EXPECT_THROW(value.asString(), std::runtime_error);
}

TEST(ValueTest, CreateArray_isArrayReturnsTrue)
{
    ScriptExecutionContext ctx("[12, 34]");
    Value value = ctx.runScript();

    EXPECT_TRUE(value.isArray());
}

TEST(ValueTest, CreateNotEmptyArray_isEmptyReturnsFalse)
{
    ScriptExecutionContext ctx("[12, 34]");
    Value value = ctx.runScript();
    v8::Local<v8::Value> val;
}

TEST(ValueTest, CreateUndefinedValue_isUndefinedReturnsTrue)
{
    ScriptExecutionContext ctx("undefined");
    auto result = ctx.runScript();

    EXPECT_TRUE(result.isUndefined());
}
