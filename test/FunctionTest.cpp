#include <gtest/gtest.h>
#include "Context.hpp"
#include "Value/Function.hpp"

using namespace testing;
using namespace cardan;

namespace
{
    // Values are valid objects untill context is destroyed
    // So, we need to keep it alive during testing values
    struct FunctionWithContext
    {
        Context context;
        Function function;

        FunctionWithContext(const std::string& jsCode)
            : function(context.runScript(jsCode).asFunction())
        {
        }
    };

    FunctionWithContext makeFunctionFromJSCode(const std::string& jsCode)
    {
        return FunctionWithContext(jsCode);
    }
}

TEST(FunctionTest, CreateJsFunctionWhichReturns42_CallIt_ReturnedValueIs42)
{
    auto [ctx, function] = makeFunctionFromJSCode("function test() { return 42; } test;");

    auto result = function.call();

    EXPECT_EQ(42, result.asInt());
}

TEST(FunctionTest, CreateJsFunctionWhichReturnsSumOf1And2_CallIt_ReturnedValueIs3)
{
    auto [ctx, function] = makeFunctionFromJSCode("function test() { return 1 + 2; } test;");

    auto result = function.call();

    EXPECT_EQ(1 + 2, result.asInt());
}

TEST(FunctionTest, CreateJsFunctionWhichReturnsSumOfArguments_CallFunctionWith1And2_ReturnedValueIs3)
{
    auto [ctx, function] = makeFunctionFromJSCode("function test(a, b) { return a + b; } test;");

    auto result = function.call(1, 2);

    EXPECT_EQ(1 + 2, result.asInt());
}

TEST(FunctionTest, CreateJsFunctionWhichReturnsSumOfArguments_CallFunctionWithTwoStrings_ReturnConcatenatedStrings)
{
    auto [ctx, function] = makeFunctionFromJSCode("function test(a, b) { return a + b; } test;");

    auto result = function.call("hello, ", "world");

    EXPECT_EQ("hello, world", result.asString());
}
