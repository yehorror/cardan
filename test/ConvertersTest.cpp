#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Context.hpp"
#include "Converters/ToV8.hpp"

using namespace testing;
/*
class ConvertersTest : public Test
{
protected:
    cardan::Context m_ctx;
    v8::Isolate* m_isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> m_v8Context = m_isolate->GetCurrentContext();
};

TEST_F(ConvertersTest, convertInt_v8ValueStoresInteger)
{
    static const int integerValue = 1613;

    auto v8Integer = cardan::ToV8::convert(m_v8Context, integerValue);

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ(integerValue, v8Integer->IntegerValue(m_v8Context).ToChecked());
}

TEST_F(ConvertersTest, convertStdString_v8ValueStoresString)
{
    static const std::string stdStringValue = "test string";

    auto v8String = cardan::ToV8::convert(m_v8Context, stdStringValue);

    ASSERT_TRUE(v8String->IsString());
    EXPECT_EQ(stdStringValue, *v8::String::Utf8Value(m_v8Context->GetIsolate(), v8String));
}

TEST_F(ConvertersTest, convertDouble_v8ValueStoresDouble)
{
    static const double doubleValue = 3.1415;

    auto v8Double = cardan::ToV8::convert(m_v8Context, doubleValue);

    ASSERT_TRUE(v8Double->IsNumber());
    EXPECT_EQ(doubleValue, v8Double->NumberValue(m_v8Context).ToChecked());
}

TEST_F(ConvertersTest, convertStdFunction_v8ValueStoresFunction)
{
    MockFunction<void()> mockFunction;
    std::function<void()> stdFunction = mockFunction.AsStdFunction();

    auto v8Function = cardan::ToV8::convert(m_v8Context, stdFunction);

    ASSERT_TRUE(v8Function->IsFunction());
    EXPECT_CALL(mockFunction, Call());

    v8Function.As<v8::Function>()->Call(m_v8Context, m_v8Context->Global(), 0, nullptr).ToLocalChecked();
}

TEST_F(ConvertersTest, v8Integer_convertToInt)
{
    static const int integerValue = 1613;

    auto v8Integer = v8::Integer::New(m_isolate, integerValue);

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ(integerValue, cardan::FromV8::convert(m_v8Context, v8Integer, cardan::FromV8::To<int>{}));
}

TEST_F(ConvertersTest, v8ValueIsNotInt_convertToInt_ThrowsException)
{
    static const std::string stringValue = "hello";

    auto v8String = v8::String::NewFromUtf8(m_isolate, stringValue.c_str()).ToLocalChecked();

    ASSERT_FALSE(v8String->IsNumber());
    EXPECT_THROW(cardan::FromV8::convert(m_v8Context, v8String, cardan::FromV8::To<int>{}), std::exception);
}

TEST_F(ConvertersTest, v8ValueIsDouble_convertToDouble)
{
    static const double doubleValue = 1.4142;

    auto v8Double = v8::Number::New(m_isolate, doubleValue);

    ASSERT_TRUE(v8Double->IsNumber());
    EXPECT_EQ(doubleValue, cardan::FromV8::convert(m_v8Context, v8Double, cardan::FromV8::To<double>{}));
}

TEST_F(ConvertersTest, v8ValueIsNotDouble_convertToDouble_ThrowsException)
{
    static const std::string stringValue = "definitely not a double";

    auto v8String = v8::String::NewFromUtf8(m_isolate, stringValue.c_str()).ToLocalChecked();

    ASSERT_FALSE(v8String->IsNumber());
    EXPECT_THROW(cardan::FromV8::convert(m_v8Context, v8String, cardan::FromV8::To<double>{}), std::exception);
}

TEST_F(ConvertersTest, v8ValueIsString_convertToString)
{
    static const std::string stringValue = "definitely a string";

    auto v8String = v8::String::NewFromUtf8(m_isolate, stringValue.c_str()).ToLocalChecked();

    ASSERT_TRUE(v8String->IsString());
    EXPECT_EQ(stringValue, cardan::FromV8::convert(m_v8Context, v8String, cardan::FromV8::To<std::string>{}));
}

// TODO Should we throw an exception, or allow conversion to a string?
TEST_F(ConvertersTest, v8ValueIsNumber_convertToString_valueIsConvertedToString)
{
    static const int integerValue = 288;

    auto v8Integer = v8::Number::New(m_isolate, integerValue);

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ("288", cardan::FromV8::convert(m_v8Context, v8Integer, cardan::FromV8::To<std::string>{}));
}
*/
