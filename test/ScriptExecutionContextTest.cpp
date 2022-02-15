#include <gtest/gtest.h>
#include "ScriptExecutionContext.hpp"

using namespace testing;
using namespace cardan;

TEST(ScriptExecutionContext, CtorWithEmptyScriptDoesntThrow)
{
    const std::string JSON = "";

    EXPECT_NO_THROW(
        ScriptExecutionContext ctx(JSON)
    );
}

TEST(ScriptExecutionContext, RunScriptWhichReturnsEmptyString_ReturnEmptyString)
{
    const std::string JSON = R"( "" )";

    ScriptExecutionContext ctx(JSON);

    auto result = ctx.runScript();

    EXPECT_TRUE(
        std::get<std::string>(result).empty()
    );
}
