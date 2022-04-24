#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Context.hpp"
#include "Class.hpp"

using namespace testing;

class ClassTest : public Test
{
protected:

    class StubClass
    {
    public:
        void parameterlessMethod1()
        {
            method1WasCalled = true;
        }

        void parameterlessMethod2()
        {
            method2WasCalled = true;
        }
    };

protected:

    void SetUp() override
    {
        // Before each test run reinitialize field with 'false'
        method1WasCalled = false;
        method2WasCalled = false;
    }

protected:
    inline static bool method1WasCalled = false;
    inline static bool method2WasCalled = false;
};

TEST_F(ClassTest, MakeClass_AddParameterlessMethod_AddClassToContext_CreateInstance_CallParameterlessMethod_MethodCalled)
{
    cardan::Class<StubClass> stubClass;

    stubClass.method("parameterlessMethod", &StubClass::parameterlessMethod1);

    cardan::Context ctx;
    ctx.set("StubClass", stubClass);

    ASSERT_FALSE(method1WasCalled);

    ctx.runScript(R"JS(
        var obj = new StubClass();
        obj.parameterlessMethod();
    )JS");

    EXPECT_TRUE(method1WasCalled);
}

TEST_F(ClassTest, MakeClass_CreateItWithoutNewInJSCode_ExceptionIsntThrown)
{
    cardan::Class<StubClass> stubClass;

    cardan::Context ctx;
    ctx.set("StubClass", stubClass);

    EXPECT_THROW(ctx.runScript(" var obj = StubClass(); "), std::exception);
}

TEST_F(ClassTest, MakeClass_AddTwoParameterlessMethods_CreateInstance_CallTwoMethods_MethodsAreCalled)
{
    cardan::Class<StubClass> stubClass;

    stubClass.method("parameterlessMethod1", &StubClass::parameterlessMethod1);
    stubClass.method("parameterlessMethod2", &StubClass::parameterlessMethod2);

    cardan::Context ctx;
    ctx.set("StubClass", stubClass);

    ASSERT_FALSE(method1WasCalled);
    ASSERT_FALSE(method2WasCalled);

    ctx.runScript(R"JS(
        var obj = new StubClass();
        obj.parameterlessMethod1();
        obj.parameterlessMethod2();
    )JS");

    EXPECT_TRUE(method1WasCalled);
    EXPECT_TRUE(method2WasCalled);
}
