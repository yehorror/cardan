#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Context.hpp"

using namespace testing;
using namespace cardan;

TEST(Scenarios, ScriptHasMultipleFunctionsDeclarations_GetEachOfThem_CallEachFunction_ResultsCorrespondsWithReturnValues)
{
    const std::string JS = R"JS(
        function intValueFunc() {
            return 123;
        }

        function stringValueFunc() {
            return 'string';
        }
    )JS";

    Context context(JS);
    context.runScript();

    Function intValueFunc = context.get("intValueFunc").asFunction();
    Function stringValueFunc = context.get("stringValueFunc").asFunction();

    EXPECT_EQ(123,      intValueFunc.call().asInt());
    EXPECT_EQ("string", stringValueFunc.call().asString());
}

TEST(Scenarios, ScriptHasFunctionWhichCallToCppFunction_CallThisFunction_CppFunctionIsCalled)
{
    const std::string JS = R"JS(

        function callCppFunction() {
            cppFunction();
        }

    )JS";

    MockFunction<void()> cppMockFunction;
    auto stdFunction = cppMockFunction.AsStdFunction();

    Context context(JS);
    context.set("cppFunction", stdFunction);
    context.runScript();

    Function jsFunction = context.get("callCppFunction").asFunction();

    EXPECT_CALL(cppMockFunction, Call());

    jsFunction.call();
}
