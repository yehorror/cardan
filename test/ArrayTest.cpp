#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ScriptExecutionContext.hpp"
#include "Value/Array.hpp"

using namespace testing;
using namespace cardan;

namespace
{
    // Values are valid objects untill context is destroyed
    // So, we need to keep it alive during testing values
    struct ArrayWithContext
    {
        ScriptExecutionContext context;
        Array array;

        ArrayWithContext(const std::string& jsCode)
            : context(jsCode)
            , array(context.runScript().asArray())
        {
        }
    };

    ArrayWithContext makeArrayFromJSCode(const std::string& jsCode)
    {
        return ArrayWithContext(jsCode);
    }
}

TEST(ArrayTest, CreateEmtpyArray_LengthIsZero)
{
    auto [ctx, array] = makeArrayFromJSCode("[]");
    EXPECT_EQ(0, array.length());
}

TEST(ArrayTest, CreateArrayWith3Elements_LengthIsEquals3)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");
    EXPECT_EQ(3, array.length());
}

TEST(ArrayTest, CreateArrayWith3Elements_CallAtWithIndexInRange_ReturnedValueByItsIndex)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    auto value = array.at(0);
    EXPECT_EQ(1, value.asInt());
}

TEST(ArrayTest, CreateArrayWith3Elements_CallAtWithIndexOutOfRange_ReturnUndefined)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    auto value = array.at(3);
    EXPECT_TRUE(value.isUndefined());
}

TEST(ArrayTest, CreateArrayWith3Elements_OperatorSquaredBracketsReturnsSameValuesAsMethodAt)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    EXPECT_EQ(array.at(0).asInt(), array[0].asInt());
    EXPECT_EQ(array.at(1).asInt(), array[1].asInt());
    EXPECT_EQ(array.at(2).asInt(), array[2].asInt());
}

TEST(ArrayTest, CreateArrayWith3ElementsOfDifferentTypes_TypesOfTakenValuesCorrespondsToTypesInArray)
{
    auto [ctx, array] = makeArrayFromJSCode("[123,'string',3.14]");

    EXPECT_TRUE(array.at(0).isInt());
    EXPECT_TRUE(array.at(1).isString());
    EXPECT_TRUE(array.at(2).isDouble());
}

TEST(ArrayTest, CreateArrayWith3Elements_CallBeginWithDereference_ReturnedFirstElement)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    auto arrayBegin = array.begin();
    EXPECT_EQ(1, (*arrayBegin).asInt());
}

TEST(ArrayTest, CreateArrayWith3Elements_CallBegin_IncrementIterator_DereferencedValueIsSecondValue)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    auto iterator = array.begin();
    ++iterator;
    EXPECT_EQ(2, (*iterator).asInt());
}

TEST(ArrayTest, CreateArrayWith3Elements_BeginDoesNotEqualsEnd)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    EXPECT_NE(array.begin(), array.end());
}

TEST(ArrayTest, CreateArrayWith3Elements_IterateOverItViaStdFor_each_PredicateCalledWithCorrespondingValues)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    MockFunction<void(const cardan::Value&)> mockPredicate;

    {
        InSequence seq;
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce(([](const cardan::Value& val) { EXPECT_EQ(1, val.asInt()); }));
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce(([](const cardan::Value& val) { EXPECT_EQ(2, val.asInt()); }));
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce(([](const cardan::Value& val) { EXPECT_EQ(3, val.asInt()); }));
    }

    std::for_each(array.begin(), array.end(), mockPredicate.AsStdFunction());
}

TEST(ArrayTest, CreateArrayWith3Elements_IterateOverItViaRangeBasedFor_PredicateCalledWithCorrespondingValues)
{
    auto [ctx, array] = makeArrayFromJSCode("[1,2,3]");

    MockFunction<void(const cardan::Value&)> mockPredicate;

    {
        InSequence seq;
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce(([](const cardan::Value& val) { EXPECT_EQ(1, val.asInt()); }));
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce(([](const cardan::Value& val) { EXPECT_EQ(2, val.asInt()); }));
        EXPECT_CALL(mockPredicate, Call(_)).WillOnce(([](const cardan::Value& val) { EXPECT_EQ(3, val.asInt()); }));
    }

    for (const auto& value : array)
    {
        mockPredicate.Call(value);
    }
}
