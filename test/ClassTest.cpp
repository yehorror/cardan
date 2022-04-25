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

TEST_F(ClassTest, MakeClass_AddMethodWithParametersAndReturnValue_CreateInstance_CallMethod_MethodIsCalled)
{
    class MockSummator : public Mock
    {
    public:
        MockSummator()
        {
            // Is it normal?..
            EXPECT_CALL(*this, add(2, 3)).WillOnce(Return(5));
        }

        MOCK_METHOD2(add, int(int, int));
    };

    cardan::Class<MockSummator> summator;
    summator.method("add", &MockSummator::add);

    cardan::Context ctx;
    ctx.set("Summator", summator);

    auto result = ctx.runScript(R"JS(
        var summator = new Summator();
        summator.add(2, 3);
    )JS");

    EXPECT_EQ(2 + 3, result.as<int>());
}

TEST_F(ClassTest, MakeIncrementerClassWithInternalState_AddClass_CallIncrementMethodTwice_ValueOfAClassIsTwo)
{
    class Incrementer
    {
    public:
        void increment()
        {
            ++m_value;
        }

        int value()
        {
            return m_value;
        }

    private:
        int m_value = 0;
    };

    cardan::Class<Incrementer> incrementer;
    incrementer.method("increment", &Incrementer::increment);
    incrementer.method("value", &Incrementer::value);

    cardan::Context ctx;
    ctx.set("Incrementer", incrementer);

    auto initialState = ctx.runScript(R"JS(
        var incrementer = new Incrementer();
        incrementer.value();
    )JS");

    ASSERT_EQ(0, initialState.as<int>());

    auto stateAfterTwoIncrements = ctx.runScript(R"JS(
        incrementer.increment();
        incrementer.increment();

        incrementer.value();
    )JS");

    EXPECT_EQ(2, stateAfterTwoIncrements.as<int>());
}

TEST_F(ClassTest, ClassHasGetterAndSetter_AddThemAsPropertyToClass_CreateInstanceOfAClass_GetValue_GetterCalled)
{
    class ValueStorage : public Mock
    {
    public:
        ValueStorage()
        {
            EXPECT_CALL(*this, get()).WillOnce(Return(41));
        }

        MOCK_METHOD0(get, int());
        MOCK_METHOD1(set, void(int));
    };

    cardan::Class<ValueStorage> valueClass;
    valueClass.property("value", &ValueStorage::get, &ValueStorage::set);

    cardan::Context ctx;
    ctx.set("ValueStorage", valueClass);

    auto value = ctx.runScript(R"JS(
        let storage = new ValueStorage();
        storage.value;
    )JS");

    EXPECT_EQ(41, value.as<int>());
}
