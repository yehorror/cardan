#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Context.hpp"

using namespace testing;
using namespace cardan;

TEST(ContextTest, RunScriptWhichAddsTwoNumbers_ReturnSumOfNumbers)
{
    const std::string JS = R"( 1 + 2; )";

    Context ctx;

    auto result = ctx.runScript(JS);

    EXPECT_EQ(
        1 + 2,
        result.asInt()
    );
}

TEST(ContextTest, RunScriptWhichThrowsException_ThrowExceptionInCpp)
{
    // TODO Add test for exception description, maybe other info
    const std::string JS = R"( throw 'This is exception'; )";

    Context ctx;

    EXPECT_THROW(
        ctx.runScript(JS),
        JSException
    );
}
/*
TEST(ContextTest, AddFunctionWithNoArguments_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JS = R"( testFunction1(); )";

    Context ctx;

    MockFunction<void()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction1", stdFunction);

    EXPECT_CALL(mockFunction, Call());

    EXPECT_NO_THROW(
        ctx.runScript(JS)
    );
}

TEST(ContextTest, AddFunctionWithOneArgument_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JS = R"( testFunction(123); )";

    Context ctx;

    MockFunction<void(int)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call(123));

    EXPECT_NO_THROW(
        ctx.runScript(JS)
    );
}

TEST(ContextTest, AddFunctionWithTwoArguments_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JS = R"( testFunction(123, 456); )";

    Context ctx;

    MockFunction<void(int, int)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call(123, 456));

    EXPECT_NO_THROW(
        ctx.runScript(JS)
    );
}

TEST(ContextTest, AddFunctionWithStringArgument_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JS = R"( testFunction('hello'); )";

    Context ctx;

    MockFunction<void(std::string)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call("hello"));

    EXPECT_NO_THROW(
        ctx.runScript(JS)
    );
}

TEST(ContextTest, AddFunctionWithStringAndIntArguments_RunScriptWhichCallsThisFunction_FunctionWasCalled)
{
    const std::string JS = R"( testFunction('hello', 123); )";

    Context ctx;

    MockFunction<void(std::string, int)> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call("hello", 123));

    EXPECT_NO_THROW(
        ctx.runScript(JS)
    );
}

TEST(ContextTest, AddTwoFunctions_CallThemFromJavascript_BothFunctionsCalled)
{
    const std::string JS = R"(
        firstFunction('hello');
        secondFunction(123);
    )";

    Context ctx;

    MockFunction<void(std::string)> mockFunction1;
    MockFunction<void(int)> mockFunction2;

    auto stdFunction1 = mockFunction1.AsStdFunction();
    auto stdFunction2 = mockFunction2.AsStdFunction();

    ctx.set("firstFunction", stdFunction1);
    ctx.set("secondFunction", stdFunction2);

    {
        InSequence sequenceGuard;
        EXPECT_CALL(mockFunction1, Call("hello"));
        EXPECT_CALL(mockFunction2, Call(123));
    }

    EXPECT_NO_THROW(
        ctx.runScript(JS)
    );
}

TEST(ContextTest, AddFunctionWhichReturnsIntValue_RunScriptWhichCallsThisFunction_ValueIsReturned)
{
    const std::string JS = R"( testFunction(); )";

    const int RETURNED_VALUE = 123;

    Context ctx;

    MockFunction<int()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call()).WillOnce(Return(RETURNED_VALUE));

    auto result = ctx.runScript(JS);

    EXPECT_EQ(
        RETURNED_VALUE,
        result.asInt()
    );
}

TEST(ContextTest, AddFunctionWhichReturnsString_RunScriptWhichCallsThisFunction_StringIsReturned)
{
    const std::string JS = R"( testFunction(); )";

    const std::string RETURNED_VALUE = "some test value";

    Context ctx;

    MockFunction<std::string()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call()).WillOnce(Return(RETURNED_VALUE));

    auto result = ctx.runScript(JS);

    EXPECT_EQ(
        RETURNED_VALUE,
        result.asString()
    );
}

TEST(ContextTest, AddFunctionWhichReturnsDouble_RunScriptWhichCallsThisFunction_DoubleIsReturned)
{
    const std::string JS = R"( testFunction(); )";

    const double RETURNED_VALUE = 0.5;

    Context ctx;

    MockFunction<double()> mockFunction;

    auto stdFunction = mockFunction.AsStdFunction();
    ctx.set("testFunction", stdFunction);

    EXPECT_CALL(mockFunction, Call()).WillOnce(Return(RETURNED_VALUE));

    auto result = ctx.runScript(JS);

    EXPECT_DOUBLE_EQ(
        RETURNED_VALUE,
        result.asDouble()
    );
}
*/

TEST(ContextTest, RunScriptWhichAddsTwoRealNumbers_DoubleIsReturned)
{
    const std::string JS = R"( 0.1 + 0.2 )";

    Context ctx;

    auto result = ctx.runScript(JS);

    EXPECT_DOUBLE_EQ(0.3, result.asDouble());
}

TEST(ContextTest, RunScriptWithInvalidJSCode_ThrowException)
{
    const std::string JS = R"( definitely invalid js code )";

    Context ctx;

    EXPECT_THROW(
        ctx.runScript(JS),
        InvalidJSCodeException
    );
}

TEST(ContextTest, RunScriptWhichCreatesVariable_get_ReturnValueOfThisVariable)
{
    const std::string JS = R"(var variable = 43;)";

    Context ctx;

    ctx.runScript(JS);

    cardan::Value variableValue = ctx.get("variable");

    EXPECT_EQ(43, variableValue.asInt());
}

TEST(ContextTest, SetVariableWithIntValue_ScriptReturnsThisVariable_ReturnedValueIsValueWhichWasSet)
{
    const std::string JS = R"( answer; )";

    Context ctx;

    ctx.set("answer", 42);

    cardan::Value result = ctx.runScript(JS);

    EXPECT_EQ(42, result.asInt());
}

TEST(ContextTest, SetVariableWithStringValue_ScriptReturnsThisVariable_ReturnedValueIsValueWhichWasSet)
{
    const std::string JS = R"( question; )";

    Context ctx;

    ctx.set("question", "The Ultimate Question of Life, the Universe, and Everything");

    cardan::Value result = ctx.runScript(JS);

    EXPECT_EQ("The Ultimate Question of Life, the Universe, and Everything", result.asString());
}

TEST(ContextTest, SetVariableWithDoubleValue_ScriptReturnsThisVariable_ReturnedValueIsValueWhichWasSet)
{
    const std::string JS = R"( pi; )";

    Context ctx;

    ctx.set("pi", 3.1415);

    cardan::Value result = ctx.runScript(JS);

    EXPECT_DOUBLE_EQ(3.1415, result.asDouble());
}

TEST(ContextTest, ExecuteTwoScriptsWithinOneContextWhichChangesVariable_VariableValueEqualsToResultOfSecondScript)
{
    const std::string JS1 = R"( var state = 'first script executed'; )";
    const std::string JS2 = R"( state = 'second script executed'; )";

    Context ctx;

    ctx.runScript(JS1);
    ctx.runScript(JS2);

    cardan::Value state = ctx.get("state");

    EXPECT_EQ("second script executed", state.asString());
}
/*
TEST(ContextTest, AddCppFunction_CallThisFunctionFromJSCodeWithWrongArgumentsCount_ExceptionIsThrown)
{
    const std::string JS = R"( cppFunction(1, 2); )";

    Context ctx;

    std::function<void(int, int, int)> function = [] (int, int, int) {   };

    ctx.set("cppFunction", function);

    EXPECT_THROW(ctx.runScript(JS), JSException);
}

TEST(ContextTest, AddCppFunctionWhichThrowsException_CallFunctionFromJSCode_ExceptionWithinJSCodeIsThrown)
{
    const std::string JS = R"(
        try {
            cppFunction();
        } catch (exception) {
            var exceptionCaught = true;
        }
    )";

    Context ctx;

    MockFunction<void()> mockFunction;
    auto stdMockFunction = mockFunction.AsStdFunction();

    ctx.set("cppFunction", stdMockFunction);

    EXPECT_CALL(mockFunction, Call()).WillOnce(Throw(std::runtime_error("Smth bad happened")));

    ctx.runScript(JS);

    cardan::Value value = ctx.get("exceptionCaught");

    EXPECT_TRUE(value.asBool());
}
*/
TEST(ContextTest, MakeIntegerValue_asInt_ReturnsValueItWasMadeWith)
{
    const int VALUE = 52375;

    Context ctx;
    cardan::Value value = ctx.makeValue(VALUE);

    ASSERT_TRUE(value.isInt());
    EXPECT_EQ(VALUE, value.asInt());
}

TEST(ContextTest, MakeStringValue_asString_ReturnsValueItWasMadeWith)
{
    const std::string VALUE = "some value";

    Context ctx;
    cardan::Value value = ctx.makeValue(VALUE);

    ASSERT_TRUE(value.isString());
    EXPECT_EQ(VALUE, value.asString());
}

TEST(ContextTest, MakeDoubleValue_asDouble_ReturnsValueItWasMadeWith)
{
    const double VALUE = 2.71828;

    Context ctx;
    cardan::Value value = ctx.makeValue(VALUE);

    ASSERT_TRUE(value.isDouble());
    EXPECT_DOUBLE_EQ(VALUE, value.asDouble());
}
/*
TEST(ContextTest, MakeFunctionValue_asFunction_ReturnsFunctionWhichCanBeCalled)
{
    MockFunction<void(int)> mockFunction;

    auto mockStdFunction = mockFunction.AsStdFunction();

    Context ctx;
    cardan::Value value = ctx.makeValue(mockStdFunction);

    ASSERT_TRUE(value.isFunction());

    EXPECT_CALL(mockFunction, Call(42));

    value.asFunction().call(42);
}
*/
TEST(ContextTest, CreateEmptyContext_global_ReturnsEmptyObject)
{
    Context ctx;
    Object global = ctx.global();

    EXPECT_EQ(0, global.getKeys().length());
}

TEST(ContextTest, JSCodeThrowsError_ExceptionRethrowedInCppWithMessageFromJSCode)
{
    Context ctx;

    try
    {
        ctx.runScript("throw new TypeError('Out of creativity exception')");
    }
    catch (const JSException& jsException)
    {
        EXPECT_STREQ("Out of creativity exception", jsException.what());
    }
    catch (...)
    {
        FAIL() << "Wrong type of exception was thrown";
    }
}

TEST(ContextTest, SetStdFunctionWhichIsDestroyedLater_FunctionIsCopiedSoItStillCanBeCalledFromJSCode)
{
    Context ctx;

    MockFunction<void()> mockFunction;

    {
        // On scope leave stdFunction will be destroyed
        // Test checks that even if added function is destroyed, it still can be called

        auto stdFunction = mockFunction.AsStdFunction();
        ctx.set("cppFunction", stdFunction);
    }

    EXPECT_CALL(mockFunction, Call());
    ctx.runScript("cppFunction()");
}

TEST(ContextTest, SetLambdaFunction_FunctionCanBeCalledFromJSCode)
{
    Context ctx;

    MockFunction<void()> mockFunction;
    ctx.set("lambda", [&] () { mockFunction.Call(); });

    EXPECT_CALL(mockFunction, Call());
    ctx.runScript("lambda()");
}

