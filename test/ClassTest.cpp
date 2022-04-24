#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Context.hpp"
#include "Class.hpp"

using namespace testing;

TEST(ClassTest, MakeClass_addParameterlessMethod_AddClassToContext_CreateInstance_CallParameterlessMethod_MethodCalled)
{
    // I know, global state in tests is bad manners, but there's no other way now
    static bool methodWasCalled = false;

    class StubClass
    {
    public:
        void parameterlessMethod()
        {
            methodWasCalled = true;
        }
    };

    cardan::Class<StubClass> stubClass;

    stubClass.method("parameterlessMethod", &StubClass::parameterlessMethod);

    cardan::Context ctx;
    ctx.set("StubClass", stubClass);

    ASSERT_FALSE(methodWasCalled);

    ctx.runScript(R"JS(
        var obj = new StubClass();
        obj.parameterlessMethod();
    )JS");

    EXPECT_TRUE(methodWasCalled);
}
