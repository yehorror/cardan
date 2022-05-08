#include <gtest/gtest.h>
#include "cardan/Context.hpp"
#include "cardan/Value/Object.hpp"
#include "cardan/Value/Value.hpp"

using namespace testing;
using namespace cardan;

namespace
{
    // Values are valid objects untill context is destroyed
    // So, we need to keep it alive during testing values
    struct ValueWithContext
    {
        Context context;
        Value value;

        ValueWithContext(const std::string& jsCode)
            : value(context.runScript(jsCode))
        {
        }
    };

    ValueWithContext makeValueFromJSCode(const std::string& jsCode)
    {
        return ValueWithContext(jsCode);
    }
}

TEST(ValueTest, CreateStringValue_ValueStoresString)
{
    auto [ctx, value] = makeValueFromJSCode(" 'string value' ");

    EXPECT_TRUE(value.isString());
}

TEST(ValueTest, CreateStringvalue_asStringReturnsValueWhichItWasCreatedWith)
{
    auto [ctx, value] = makeValueFromJSCode(" 'string value' ");

    ASSERT_TRUE(value.isString());
    EXPECT_EQ("string value", value.asString());
}

TEST(ValueTest, CreateIntegerValue_ValueStoresInt)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    EXPECT_TRUE(value.isInt());
}

TEST(ValueTest, CreateIntegerValue_asIntReturnsValueWhichItWasCreatedWith)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    ASSERT_TRUE(value.isInt());
    EXPECT_EQ(123, value.asInt());
}

TEST(ValueTest, CreateDoubleValue_ValueStoresDouble)
{
    auto [ctx, value] = makeValueFromJSCode("123.456");

    EXPECT_TRUE(value.isDouble());
}

TEST(ValueTest, CreateDoubleValue_asDoubleReturnsValueWhichItWasCreatedWith)
{
    auto [ctx, value] = makeValueFromJSCode("123.456");

    ASSERT_TRUE(value.isDouble());
    EXPECT_DOUBLE_EQ(123.456, value.asDouble());
}

TEST(ValueTest, CreateStringValue_TryToGetDouble_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode(" 'definitely not a double' ");

    ASSERT_FALSE(value.isDouble());
    EXPECT_THROW(value.asDouble(), std::runtime_error);
}

TEST(ValueTest, CreateBoolValue_ValueStoresBool)
{
    auto [ctx, value] = makeValueFromJSCode("true");

    EXPECT_TRUE(value.isBool());
}

TEST(ValueTest, CreateTrueBoolValue_asBoolReturnsTrue)
{
    auto [ctx, value] = makeValueFromJSCode("true");

    ASSERT_TRUE(value.isBool());
    EXPECT_EQ(true, value.asBool());
}

TEST(ValueTest, CreateFalseBoolValue_asBoolReturnsFalse)
{
    auto [ctx, value] = makeValueFromJSCode("false");

    ASSERT_TRUE(value.isBool());
    EXPECT_EQ(false, value.asBool());
}

TEST(ValueTest, CreateStringValue_TryToGetBool_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode(" 'definitely not a bool' ");

    ASSERT_FALSE(value.isBool());
    EXPECT_THROW(value.asBool(), std::runtime_error);
}

TEST(ValueTest, CreateStringValue_TryToGetInt_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode(" 'definitely not an integer' ");

    ASSERT_FALSE(value.isInt());
    EXPECT_THROW(value.asInt(), std::runtime_error);
}

TEST(ValueTest, CreateIntValue_TryToGetString_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    ASSERT_FALSE(value.isString());
    EXPECT_THROW(value.asString(), std::runtime_error);
}

TEST(ValueTest, CreateArray_isArrayReturnsTrue)
{
    auto [ctx, value] = makeValueFromJSCode("[12, 34]");

    EXPECT_TRUE(value.isArray());
}

TEST(ValueTest, CreateArray_asArray_DoesntThrow)
{
    auto [ctx, value] = makeValueFromJSCode("[12, 34]");

    ASSERT_TRUE(value.isArray());
    EXPECT_NO_THROW(value.asArray());
}

TEST(ValueTest, CreateIntValue_TryToGetArray_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    ASSERT_FALSE(value.isArray());
    EXPECT_THROW(value.asArray(), std::runtime_error);
}

TEST(ValueTest, CreateObject_isObjectReturnsTrue)
{
    auto [ctx, value] = makeValueFromJSCode(R"( JSON.parse("{\"some_key\": \"some_value\"}") )");

    EXPECT_TRUE(value.isObject());
}

TEST(ValueTest, CreateObject_asObject_DoesntThrow)
{
    auto [ctx, value] = makeValueFromJSCode(R"( JSON.parse("{\"some_key\": \"some_value\"}") )");

    ASSERT_TRUE(value.isObject());
    EXPECT_NO_THROW(value.asObject());
}

TEST(ValueTest, CreateIntValue_TryToGetObject_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    ASSERT_FALSE(value.isObject());
    EXPECT_THROW(value.asObject(), std::runtime_error);
}

TEST(ValueTest, CreateFunction_isFunctionReturnsTrue)
{
    auto [ctx, value] = makeValueFromJSCode("function test() { return 'test function'; }; test");

    EXPECT_TRUE(value.isFunction());
}

TEST(ValueTest, CreateFunction_asFunction_DoesntThrow)
{
    auto [ctx, value] = makeValueFromJSCode("function test() { return 'test function'; }; test");

    ASSERT_TRUE(value.isFunction());
    EXPECT_NO_THROW(value.asFunction());
}

TEST(ValueTest, CreateIntObject_TryToGetFunction_ExceptionOfWrongTypeIsThrown)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    ASSERT_FALSE(value.isFunction());
    EXPECT_THROW(value.asFunction(), std::runtime_error);
}

TEST(ValueTest, CreateUndefinedValue_isUndefinedReturnsTrue)
{
    auto [ctx, value] = makeValueFromJSCode("undefined");

    EXPECT_TRUE(value.isUndefined());
}

TEST(ValueTest, CreateSomeValue_AssignAnotherIntegerValue_ValueIsWhatItWasAssignedTo)
{
    auto [ctx, value] = makeValueFromJSCode("123");

    value = 456;

    EXPECT_EQ(456, value.asInt());
}

TEST(ValueTest, CreateNullValue_isNull_ReturnsTrue)
{
    auto [ctx, value] = makeValueFromJSCode("null");

    EXPECT_TRUE(value.isNull());
}

TEST(ValueTest, CreateIntValue_TemplatedAsInt_ReturnsIntValue)
{
    auto [ctx, value] = makeValueFromJSCode("17");

    ASSERT_TRUE(value.isInt());
    EXPECT_EQ(17, value.as<int>());
}

TEST(ValueTest, CreateDoubleValue_TemplatedAsDouble_ReturnsDoubleValue)
{
    auto [ctx, value] = makeValueFromJSCode("2.71828");

    ASSERT_TRUE(value.isDouble());
    EXPECT_DOUBLE_EQ(2.71828, value.as<double>());
}

TEST(ValueTest, CreateStringValue_TemplatedAsString_ReturnsStringValue)
{
    auto [ctx, value] = makeValueFromJSCode("'some string value'");

    ASSERT_TRUE(value.isString());
    EXPECT_EQ("some string value", value.as<std::string>());
}

TEST(ValueTest, CreateValueViaConstructor_v8_ReturnedValueIsSameToValueWhichWasWrapped)
{
    Context ctx;
    v8::Local<v8::Value> someValue = v8::String::NewFromUtf8(ctx.getIsolate(), "some value").ToLocalChecked();

    Value wrappedValue(someValue, ctx);

    EXPECT_TRUE(someValue->SameValue(wrappedValue.v8()));
}
