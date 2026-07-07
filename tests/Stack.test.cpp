#include <gtest/gtest.h>

#include "Stack.hpp"

TEST(StackTest, NewStackIsEmpty) {
    Stack stack;

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0u);
    EXPECT_EQ(stack.begin(), stack.end());
}

TEST(StackTest, PushIncreasesSize) {
    Stack stack;
    stack.push(1.0);
    stack.push(2.0);

    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 2u);
}

TEST(StackTest, PopReturnsValuesInLifoOrder) {
    Stack stack;
    stack.push(10.0);
    stack.push(20.0);
    stack.push(30.0);

    EXPECT_DOUBLE_EQ(stack.pop(), 30.0);
    EXPECT_DOUBLE_EQ(stack.pop(), 20.0);
    EXPECT_DOUBLE_EQ(stack.pop(), 10.0);
    EXPECT_TRUE(stack.empty());
}

TEST(StackTest, ResetClearsStack) {
    Stack stack;
    stack.push(1.0);
    stack.push(2.0);

    stack.reset();

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0u);
}

TEST(StackTest, ResetAllowsReuse) {
    Stack stack;
    stack.push(99.0);
    stack.reset();

    stack.push(42.0);

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_DOUBLE_EQ(stack.pop(), 42.0);
}

TEST(StackTest, GrowsBeyondInitialCapacity) {
    Stack stack;

    for (std::size_t i = 0; i < STACK_INITIAL_CAPACITY + 1; ++i) {
        stack.push(static_cast<Value>(i));
    }

    ASSERT_EQ(stack.size(), STACK_INITIAL_CAPACITY + 1);

    for (std::size_t i = STACK_INITIAL_CAPACITY; i > 0; --i) {
        EXPECT_DOUBLE_EQ(stack.pop(), static_cast<Value>(i));
    }
    EXPECT_DOUBLE_EQ(stack.pop(), 0.0);
    EXPECT_TRUE(stack.empty());
}

TEST(StackTest, BeginEndReflectPushedValues) {
    Stack stack;
    stack.push(1.5);
    stack.push(2.5);
    stack.push(3.5);

    std::size_t index = 0;
    const Value expected[] = {1.5, 2.5, 3.5};

    for (Value* it = stack.begin(); it < stack.end(); ++it) {
        ASSERT_LT(index, 3u);
        EXPECT_DOUBLE_EQ(*it, expected[index]);
        ++index;
    }

    EXPECT_EQ(index, 3u);
}
