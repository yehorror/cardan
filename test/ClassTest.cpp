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

TEST_F(ClassTest, ClassHasGetterAndSetter_AddThemAsPropertyToClass_GetValue_ReturnsValueFromGetter)
{
    class ValueStorage
    {
    public:
        int get()
        {
            return 41;
        }

        void set(int)
        {
        }
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

TEST_F(ClassTest, ClassHasGetterAndSetter_AddThemAsPropertyToClass_AssignSomeValue_GetValue_ReturnsValueWhichWasSet)
{
    class ValueStorage
    {
    public:
        void set(int newVal)
        {
            m_val = newVal;
        }

        int get()
        {
            return m_val;
        }
    private:
        int m_val = 0;
    };

    cardan::Class<ValueStorage> valueClass;
    valueClass.property("value", &ValueStorage::get, &ValueStorage::set);

    cardan::Context ctx;
    ctx.set("ValueStorage", valueClass);

    auto result = ctx.runScript(R"JS(
        let storage = new ValueStorage();
        storage.value = 37;

        storage.value;
    )JS");

    EXPECT_EQ(37, result.as<int>());
}

TEST_F(ClassTest, StructHasFieldWithDefaultValue_AddThisFieldAsProperty_GetValue_ValueIsReturned)
{
    struct Employee
    {
        std::string name = "Yehor";
    };

    cardan::Class<Employee> valueClass;
    valueClass.property("name", &Employee::name);

    cardan::Context ctx;
    ctx.set("ValueStorage", valueClass);

    auto result = ctx.runScript(R"JS(
        let storage = new ValueStorage();
        storage.name;
    )JS");

    EXPECT_EQ("Yehor", result.as<std::string>());
}

// TODO This test probably may be not stable
// It is not guaranteed that GC of V8 will be called even in case we create 10k objects in loop
TEST_F(ClassTest, CreateObjectInLoop10kTimes_AtLeastOnceDestructorIsCalled)
{
    static bool destructorWasCalled = false;

    struct Employee
    {
        std::string name;

        ~Employee()
        {
            destructorWasCalled = true;
        }
    };

    cardan::Class<Employee> valueClass;
    valueClass.property("name", &Employee::name);

    cardan::Context ctx;
    ctx.set("Employee", valueClass);

    ASSERT_FALSE(destructorWasCalled);

    ctx.runScript(R"JS(
        for (let i = 0; i < 10000; ++i) {
            let employee = new Employee();
        }
    )JS");

    EXPECT_TRUE(destructorWasCalled);
}

TEST_F(ClassTest, CreateObject_LeaveScopeWithContext_DestructorIsCalled)
{
    static bool destructorWasCalled = false;

    struct Employee
    {
        std::string name;

        ~Employee()
        {
            destructorWasCalled = true;
        }
    };

    cardan::Class<Employee> valueClass;
    valueClass.property("name", &Employee::name);

    ASSERT_FALSE(destructorWasCalled);

    {
        cardan::Context ctx;
        ctx.set("Employee", valueClass);

        ctx.runScript(" let employee = new Employee(); ");
    }
    // On scope leave, Context is destroyed, and, henceforth, all the objects created within this Context

    EXPECT_TRUE(destructorWasCalled);
}

TEST_F(ClassTest, ClassHasNonDefaultConstructor_ParametersFromObjectCreatinInJSPassedToCppConstructor)
{
    class Employee
    {
    public:
        Employee(std::string name)
            : m_name(name)
        {
        }

        std::string getName()
        {
            return m_name;
        }

        void setName(std::string newName)
        {
            m_name = newName;
        }
    private:
        std::string m_name;
    };

    cardan::Class<Employee> employeeClass;
    employeeClass.constructor<std::string>();
    employeeClass.property("name", &Employee::getName, &Employee::setName);

    cardan::Context ctx;
    ctx.set("Employee", employeeClass);

    auto result = ctx.runScript(R"JS(
        let employee = new Employee('Yehor');
        employee.name;
    )JS");

    EXPECT_EQ("Yehor", result.as<std::string>());
}
