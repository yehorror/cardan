#include <gtest/gtest.h>
#include "Context.hpp"
#include "Value/Value.hpp"

using namespace testing;
using namespace cardan;

TEST(ValueTest, CreateStringValue_ValueStoresString)
{
    Context ctx;
    Value value = ctx.runScript(" 'string value' ");

    EXPECT_TRUE(value.isString());
}

TEST(ValueTest, CreateStringvalue_asStringReturnsValueWhichItWasCreatedWith)
{
    Context ctx;
    Value value = ctx.runScript(" 'string value' ");

    ASSERT_TRUE(value.isString());
    EXPECT_EQ("string value", value.asString());
}

TEST(ValueTest, CreateIntegerValue_ValueStoresInt)
{
    Context ctx;
    Value value = ctx.runScript("123");

    EXPECT_TRUE(value.isInt());
}

TEST(ValueTest, CreateIntegerValue_asIntReturnsValueWhichItWasCreatedWith)
{
    Context ctx;
    Value value = ctx.runScript("123");

    ASSERT_TRUE(value.isInt());
    EXPECT_EQ(123, value.asInt());
}

TEST(ValueTest, CreateDoubleValue_ValueStoresDouble)
{
    Context ctx;
    Value value = ctx.runScript("123.456");

    EXPECT_TRUE(value.isDouble());
}

TEST(ValueTest, CreateDoubleValue_asDoubleReturnsValueWhichItWasCreatedWith)
{
    Context ctx;
    Value value = ctx.runScript("123.456");

    ASSERT_TRUE(value.isDouble());
    EXPECT_DOUBLE_EQ(123.456, value.asDouble());
}

TEST(ValueTest, CreateStringValue_TryToGetDouble_ExceptionOfWrongTypeIsThrown)
{
    Context ctx;
    Value value = ctx.runScript(" 'definitely not a double' ");

    ASSERT_FALSE(value.isDouble());
    EXPECT_THROW(value.asDouble(), std::runtime_error);
}

TEST(ValueTest, CreateStringValue_TryToGetInt_ExceptionOfWrongTypeIsThrown)
{
    Context ctx;
    Value value = ctx.runScript(" 'definitely not an integer' ");

    ASSERT_FALSE(value.isInt());
    EXPECT_THROW(value.asInt(), std::runtime_error);
}

TEST(ValueTest, CreateIntValue_TryToGetString_ExceptionOfWrongTypeIsThrown)
{
    Context ctx;
    Value value = ctx.runScript("123");

    ASSERT_FALSE(value.isString());
    EXPECT_THROW(value.asString(), std::runtime_error);
}

TEST(ValueTest, CreateArray_isArrayReturnsTrue)
{
    Context ctx;
    Value value = ctx.runScript("[12, 34]");

    EXPECT_TRUE(value.isArray());
}

TEST(ValueTest, CreateArray_asArray_DoesntThrow)
{
    Context ctx;
    Value value = ctx.runScript("[12, 34]");

    ASSERT_TRUE(value.isArray());
    EXPECT_NO_THROW(value.asArray());
}

TEST(ValueTest, CreateIntValue_TryToGetArray_ExceptionOfWrongTypeIsThrown)
{
    Context ctx;
    Value value = ctx.runScript("123");

    ASSERT_FALSE(value.isArray());
    EXPECT_THROW(value.asArray(), std::runtime_error);
}

TEST(ValueTest, CreateObject_isObjectReturnsTrue)
{
    Context ctx;
    Value value = ctx.runScript(R"( JSON.parse("{\"some_key\": \"some_value\"}") )");

    EXPECT_TRUE(value.isObject());
}

TEST(ValueTest, CreateObject_asObject_DoesntThrow)
{
    Context ctx;
    Value value = ctx.runScript(R"( JSON.parse("{\"some_key\": \"some_value\"}") )");

    ASSERT_TRUE(value.isObject());
    EXPECT_NO_THROW(value.asObject());
}

TEST(ValueTest, CreateIntValue_TryToGetObject_ExceptionOfWrongTypeIsThrown)
{
    Context ctx;
    Value value = ctx.runScript("123");

    ASSERT_FALSE(value.isObject());
    EXPECT_THROW(value.asObject(), std::runtime_error);
}

TEST(ValueTest, CreateFunction_isFunctionReturnsTrue)
{
    Context ctx;
    Value value = ctx.runScript("function test() { return 'test function'; }; test");

    EXPECT_TRUE(value.isFunction());
}

TEST(ValueTest, CreateFunction_asFunction_DoesntThrow)
{
    Context ctx;
    Value value = ctx.runScript("function test() { return 'test function'; }; test");

    ASSERT_TRUE(value.isFunction());
    EXPECT_NO_THROW(value.asFunction());
}

TEST(ValueTest, CreateIntObject_TryToGetFunction_ExceptionOfWrongTypeIsThrown)
{
    Context ctx;
    Value value = ctx.runScript("123");

    ASSERT_FALSE(value.isFunction());
    EXPECT_THROW(value.asFunction(), std::runtime_error);
}

TEST(ValueTest, CreateUndefinedValue_isUndefinedReturnsTrue)
{
    Context ctx;
    auto result = ctx.runScript("undefined");

    EXPECT_TRUE(result.isUndefined());
}
