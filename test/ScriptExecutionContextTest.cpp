#include <gtest/gtest.h>
#include <gmock/gmock.h>
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

TEST(ScriptExecutionContext, AddFunctionWithNoArguments_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JSON = R"( testFunction1(); )";

    ScriptExecutionContext ctx(JSON);

    MockFunction<void()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction1", stdFunction);

    EXPECT_CALL(mockFunction, Call());

    EXPECT_NO_THROW(
        ctx.runScript()
    );
}

TEST(ScriptExecutionContext, AddFunctionWithOneArgument_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JSON = R"( testFunction(123); )";

    ScriptExecutionContext ctx(JSON);

    MockFunction<void(int)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call(123));

    EXPECT_NO_THROW(
        ctx.runScript()
    );
}

TEST(ScriptExecutionContext, AddFunctionWithTwoArguments_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JSON = R"( testFunction(123, 456); )";

    ScriptExecutionContext ctx(JSON);

    MockFunction<void(int, int)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call(123, 456));

    EXPECT_NO_THROW(
        ctx.runScript()
    );
}

TEST(ScriptExecutionContext, AddFunctionWithStringArgument_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JSON = R"( testFunction('hello'); )";

    ScriptExecutionContext ctx(JSON);

    MockFunction<void(std::string)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call("hello"));

    EXPECT_NO_THROW(
        ctx.runScript()
    );
}

TEST(ScriptExecutionContext, AddFunctionWithStringAndIntArguments_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JSON = R"( testFunction('hello', 123); )";

    ScriptExecutionContext ctx(JSON);

    MockFunction<void(std::string, int)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call("hello", 123));

    EXPECT_NO_THROW(
        ctx.runScript()
    );
}

TEST(ScriptExecutionContext, AddTwoFunctions_CallThemFromJavascript_BothFunctionsCalled)
{
    const std::string JSON = R"(
        firstFunction('hello');
        secondFunction(123);
    )";

    ScriptExecutionContext ctx(JSON);

    MockFunction<void(std::string)> mockFunction1;
    MockFunction<void(int)> mockFunction2;

    auto stdFunction1 = mockFunction1.AsStdFunction();
    auto stdFunction2 = mockFunction2.AsStdFunction();

    ctx.addFunction("firstFunction", stdFunction1);
    ctx.addFunction("secondFunction", stdFunction2);

    {
        InSequence sequenceGuard;
        EXPECT_CALL(mockFunction1, Call("hello"));
        EXPECT_CALL(mockFunction2, Call(123));
    }

    EXPECT_NO_THROW(
        ctx.runScript()
    );
}

TEST(ScriptExecutionContext, AddFunctionWhichReturnsIntValue_RunScriptWhichCallsThisFunction_ValueIsReturned)
{
    const std::string JSON = R"( testFunction(); )";

    const int RETURNED_VALUE = 123;

    ScriptExecutionContext ctx(JSON);

    MockFunction<int()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call()).WillOnce(Return(RETURNED_VALUE));

    auto result = ctx.runScript();

    EXPECT_EQ(
        RETURNED_VALUE,
        std::get<int>(result)
    );
}

TEST(ScriptExecutionContext, AddFunctionWhichReturnsString_RunScriptWhichCallsThisFunction_StringIsReturned)
{
    const std::string JSON = R"( testFunction(); )";

    const std::string RETURNED_VALUE = "some test value";

    ScriptExecutionContext ctx(JSON);

    MockFunction<std::string()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.addFunction("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call()).WillOnce(Return(RETURNED_VALUE));

    auto result = ctx.runScript();

    EXPECT_EQ(
        RETURNED_VALUE,
        std::get<std::string>(result)
    );
}

TEST(ScriptExecutionContext, RunScriptWhichAddsTwoRealNumbers_DoubleIsReturned)
{
    const std::string JSON = R"( 0.1 + 0.2 )";

    ScriptExecutionContext ctx(JSON);

    auto result = ctx.runScript();

    EXPECT_DOUBLE_EQ(0.3, std::get<double>(result));
}

TEST(ScriptExecutionContext, RunScriptWithUndefinedResult_UndefinedIsReturned)
{
    const std::string JSON = R"()";

    ScriptExecutionContext ctx(JSON);

    auto result = ctx.runScript();

    EXPECT_TRUE(
        std::holds_alternative<ScriptExecutionContext::Undefined>(result)
    );
}

TEST(ScriptExecutionContext, RunScriptWithInvalidJSCode_ThrowException)
{
    const std::string JSON = R"( definitely invalid js code )";

    ScriptExecutionContext ctx(JSON);

    EXPECT_THROW(
        ctx.runScript(),
        InvalidJSCodeException
    );
}
