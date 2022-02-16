#include <gtest/gtest.h>
#include "ScriptExecutionContext.hpp"

using namespace testing;
using namespace cardan;

TEST(ScriptExecutionContextTest, CtorWithEmptyScriptDoesntThrow)
{
    const std::string JSON = "";

    EXPECT_NO_THROW(
        ScriptExecutionContext ctx(JSON)
    );
}

TEST(ScriptExecutionContextTest, RunScriptWhichReturnsEmptyString_ReturnEmptyString)
{
    const std::string JSON = R"( "" )";

    ScriptExecutionContext ctx(JSON);

    auto result = ctx.runScript();

    EXPECT_TRUE(
        std::get<std::string>(result).empty()
    );
}

TEST(ScriptExecutionContext, RunScriptWhichAddsTwoNumbers_ReturnSumOfNumbers)
{
    const std::string JSON = R"( 1 + 2; )";

    ScriptExecutionContext ctx(JSON);

    auto result = ctx.runScript();

    EXPECT_EQ(
        1 + 2,
        std::get<int>(result)
    );
}

TEST(ScriptExecutionContext, RunScriptWhichThrowsException_ReturnException)
{
    const std::string JSON = R"( throw 'This is exception'; )";

    ScriptExecutionContext ctx(JSON);

    auto result = ctx.runScript();

    EXPECT_TRUE(
        std::holds_alternative<JSException>(result)
    );
}

TEST(ScriptExecutionContext, RunScriptWhichThrowsException_ConfiguredToRethrowException_ThrowException)
{
    const std::string JSON = R"( throw 'This is exception'; )";

    ScriptExecutionContextConfig config;
    config.rethrowExceptions = true;

    ScriptExecutionContext ctx(JSON, config);

    EXPECT_THROW(
        ctx.runScript(),
        JSException
    );
}
