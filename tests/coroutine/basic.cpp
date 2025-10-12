#include "coroutine.hpp"

#include <gtest/gtest.h>

#include <exception>
#include <stdexcept>

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

TEST(Coroutine, CantRunWhenDone) {
    Coroutine coro{[](auto) {}};

    ASSERT_FALSE(coro.IsDone());
    coro.Run();
    ASSERT_TRUE(coro.IsDone());
    ASSERT_THROW(coro.Run(), std::runtime_error);
}

TEST(Coroutine, ExceptionForwarding) {
    struct TestException: std::exception {};
    Coroutine coro{[](auto) { throw TestException{}; }};
    ASSERT_THROW(coro.Run(), TestException);
    ASSERT_TRUE(coro.IsDone());
}
