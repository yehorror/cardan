#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Context.hpp"
#include "Converters/ToV8.hpp"

using namespace testing;

TEST(ToV8ConvertersTest, convertInt_v8ValueStoresInteger)
{
    cardan::Context ctx;
    auto v8Context = v8::Isolate::GetCurrent()->GetCurrentContext();

    static const int integerValue = 1613;

    auto v8Integer = cardan::ToV8::convert(v8Context, integerValue);

    ASSERT_TRUE(v8Integer->IsNumber());
    EXPECT_EQ(integerValue, v8Integer->IntegerValue(v8Context).ToChecked());
}

TEST(ToV8ConvertersTest, convertStdString_v8ValueStoresString)
{
    cardan::Context ctx;
    auto v8Context = v8::Isolate::GetCurrent()->GetCurrentContext();

    static const std::string stdStringValue = "test string";

    auto v8String = cardan::ToV8::convert(v8Context, stdStringValue);

    ASSERT_TRUE(v8String->IsString());
    EXPECT_EQ(stdStringValue, *v8::String::Utf8Value(v8Context->GetIsolate(), v8String));
}

TEST(ToV8ConvertersTest, convertDouble_v8ValueStoresDouble)
{
    cardan::Context ctx;
    auto v8Context = v8::Isolate::GetCurrent()->GetCurrentContext();

    static const double doubleValue = 3.1415;

    auto v8Double = cardan::ToV8::convert(v8Context, doubleValue);

    ASSERT_TRUE(v8Double->IsNumber());
    EXPECT_EQ(doubleValue, v8Double->NumberValue(v8Context).ToChecked());
}

TEST(ToV8ConvertersTest, convertStdFunction_v8ValueStoresFunction)
{
    cardan::Context ctx;
    auto v8Context = v8::Isolate::GetCurrent()->GetCurrentContext();

    MockFunction<void()> mockFunction;
    std::function<void()> stdFunction = mockFunction.AsStdFunction();

    auto v8Function = cardan::ToV8::convert(v8Context, stdFunction);

    ASSERT_TRUE(v8Function->IsFunction());
    EXPECT_CALL(mockFunction, Call());

    v8Function.As<v8::Function>()->Call(v8Context, v8Context->Global(), 0, nullptr).ToLocalChecked();
}
