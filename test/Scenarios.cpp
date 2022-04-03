#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "v8.h"

struct Person;

namespace cardan::ToV8
{
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, const Person& person);
}

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

    Context context;
    context.runScript(JS);

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

    Context context;
    context.set("cppFunction", stdFunction);
    context.runScript(JS);

    Function jsFunction = context.get("callCppFunction").asFunction();

    EXPECT_CALL(cppMockFunction, Call());

    jsFunction.call();
}

TEST(Scenarios, CppFunctionCallsBackToJS_CallThisFunctionFromJSCode_CallbackFunctionCalled)
{
    const std::string JS = R"JS(

        var functionWasCalled = false;

        function callback() {
            functionWasCalled = true;
        }

        cppFunctionWithCallback(callback);

    )JS";

    std::function<void(Function)> cppFunction = [] (Function callback)
    {
        callback.call();
    };

    Context context;
    context.set("cppFunctionWithCallback", cppFunction);
    context.runScript(JS);

    auto wasFunctionCalled = context.get("functionWasCalled");

    EXPECT_EQ(true, wasFunctionCalled.asBool());
}
