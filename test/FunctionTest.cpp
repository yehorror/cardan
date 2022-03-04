#include <gtest/gtest.h>
#include "ScriptExecutionContext.hpp"
#include "Function.hpp"

using namespace testing;
using namespace cardan;

namespace
{
    // Values are valid objects untill context is destroyed
    // So, we need to keep it alive during testing values
    struct FunctionWithContext
    {
        ScriptExecutionContext context;
        Function function;

        FunctionWithContext(const std::string& jsCode)
            : context(jsCode)
            , function(context.runScript().asFunction())
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

TEST(FunctionTest, CreateJsFunctionWhichReturnsSumOf1And2_CallIt_ReturnedValueIs42)
{
    auto [ctx, function] = makeFunctionFromJSCode("function test() { return 1 + 2; } test;");

    auto result = function.call();

    EXPECT_EQ(1 + 2, result.asInt());
}
