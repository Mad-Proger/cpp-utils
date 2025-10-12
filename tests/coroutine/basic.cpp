#include "coroutine.hpp"

#include <gtest/gtest.h>

TEST(Coroutine, BasicUsage) {
    int sequence_counter = 0;

    Coroutine coro{[&sequence_counter](auto self) {
        ASSERT_EQ(++sequence_counter, 2);
        self.Yield();
        ASSERT_EQ(++sequence_counter, 4);
    }};

    ASSERT_EQ(++sequence_counter, 1);
    coro.Run();
    ASSERT_FALSE(coro.IsDone());
    ASSERT_EQ(++sequence_counter, 3);

    coro.Run();
    ASSERT_TRUE(coro.IsDone());
    ASSERT_EQ(++sequence_counter, 5);
}
