#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Context.hpp"
#include "Converters/ToV8.hpp"
#include "Converters/FromV8.hpp"

using namespace testing;

class ConvertersTest : public Test
{
protected:
    cardan::Context m_ctx;
};

TEST_F(ConvertersTest, convertInt_v8ValueStoresInteger)
{
    static const int integerValue = 1613;

    auto v8Integer = cardan::ToV8::convert(m_ctx, integerValue, cardan::ToV8::ADLTag{});

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ(integerValue, v8Integer->IntegerValue(m_ctx.getContext()).ToChecked());
}

TEST_F(ConvertersTest, convertStdString_v8ValueStoresString)
{
    static const std::string stdStringValue = "test string";

    auto v8String = cardan::ToV8::convert(m_ctx, stdStringValue, cardan::ToV8::ADLTag{});

    ASSERT_TRUE(v8String->IsString());
    EXPECT_EQ(stdStringValue, *v8::String::Utf8Value(m_ctx.getIsolate(), v8String));
}

TEST_F(ConvertersTest, convertDouble_v8ValueStoresDouble)
{
    static const double doubleValue = 3.1415;

    auto v8Double = cardan::ToV8::convert(m_ctx, doubleValue, cardan::ToV8::ADLTag{});

    ASSERT_TRUE(v8Double->IsNumber());
    EXPECT_EQ(doubleValue, v8Double->NumberValue(m_ctx.getContext()).ToChecked());
}

TEST_F(ConvertersTest, convertStdFunction_v8ValueStoresFunction)
{
    MockFunction<void()> mockFunction;
    std::function<void()> stdFunction = mockFunction.AsStdFunction();

    auto v8Function = cardan::ToV8::convert(m_ctx, stdFunction, cardan::ToV8::ADLTag{});

    ASSERT_TRUE(v8Function->IsFunction());
    EXPECT_CALL(mockFunction, Call());

    v8Function.As<v8::Function>()->Call(m_ctx.getContext(), m_ctx.getContext()->Global(), 0, nullptr).ToLocalChecked();
}

TEST_F(ConvertersTest, convertWrappedValue_ReturnsV8ValueItWraps)
{
    v8::Local<v8::Value> someValue = v8::Integer::New(m_ctx.getIsolate(), 37);
    cardan::Value wrappedSomeValue(someValue, m_ctx);

    auto unwrappedValue = cardan::ToV8::convert(m_ctx, wrappedSomeValue, cardan::ToV8::ADLTag{});

    EXPECT_TRUE(someValue->SameValue(unwrappedValue));
}

TEST_F(ConvertersTest, v8Integer_convertToInt)
{
    static const int integerValue = 1613;

    auto v8Integer = v8::Integer::New(m_ctx.getIsolate(), integerValue);

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ(integerValue, cardan::FromV8::convert(m_ctx, v8Integer, cardan::FromV8::To<int>{}));
}

TEST_F(ConvertersTest, v8ValueIsNotInt_convertToInt_ThrowsException)
{
    static const std::string stringValue = "hello";

    auto v8String = v8::String::NewFromUtf8(m_ctx.getIsolate(), stringValue.c_str()).ToLocalChecked();

    ASSERT_FALSE(v8String->IsNumber());
    EXPECT_THROW(cardan::FromV8::convert(m_ctx, v8String, cardan::FromV8::To<int>{}), std::exception);
}

TEST_F(ConvertersTest, v8ValueIsDouble_convertToDouble)
{
    static const double doubleValue = 1.4142;

    auto v8Double = v8::Number::New(m_ctx.getIsolate(), doubleValue);

    ASSERT_TRUE(v8Double->IsNumber());
    EXPECT_EQ(doubleValue, cardan::FromV8::convert(m_ctx, v8Double, cardan::FromV8::To<double>{}));
}

TEST_F(ConvertersTest, v8ValueIsNotDouble_convertToDouble_ThrowsException)
{
    static const std::string stringValue = "definitely not a double";

    auto v8String = v8::String::NewFromUtf8(m_ctx.getIsolate(), stringValue.c_str()).ToLocalChecked();

    ASSERT_FALSE(v8String->IsNumber());
    EXPECT_THROW(cardan::FromV8::convert(m_ctx, v8String, cardan::FromV8::To<double>{}), std::exception);
}

TEST_F(ConvertersTest, v8ValueIsString_convertToString)
{
    static const std::string stringValue = "definitely a string";

    auto v8String = v8::String::NewFromUtf8(m_ctx.getIsolate(), stringValue.c_str()).ToLocalChecked();

    ASSERT_TRUE(v8String->IsString());
    EXPECT_EQ(stringValue, cardan::FromV8::convert(m_ctx, v8String, cardan::FromV8::To<std::string>{}));
}

// TODO Should we throw an exception, or allow conversion to a string?
TEST_F(ConvertersTest, v8ValueIsNumber_convertToString_valueIsConvertedToString)
{
    static const int integerValue = 288;

    auto v8Integer = v8::Number::New(m_ctx.getIsolate(), integerValue);

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ("288", cardan::FromV8::convert(m_ctx, v8Integer, cardan::FromV8::To<std::string>{}));
}
