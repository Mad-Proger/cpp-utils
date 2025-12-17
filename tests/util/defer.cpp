#include "defer.hpp"

#include <gtest/gtest.h>

#include <type_traits>

struct NoThrow {
    void operator()() noexcept {};
};
static_assert(std::is_nothrow_destructible_v<util::Defer<NoThrow>>,
              "Defer must be nothrow destructible when stored function is noexcept");

struct MayThrow {
    void operator()() {}
};
static_assert(!std::is_nothrow_destructible_v<util::Defer<MayThrow>>,
              "Defer must not be nothrow destructible when stored function may throw");


TEST(DeferBasic, Noexcept) {
    bool called = false;
    {
        util::Defer d{[&]() noexcept { called = true; }};
    }
    ASSERT_TRUE(called);
}

TEST(DeferBasic, MayThrow) {
    bool called = false;
    {
        util::Defer d{[&]() { called = true; }};
    }
    ASSERT_TRUE(called);
}

TEST(DeferBasic, Cancellation) {
    bool called = false;
    {
        util::Defer d{[&]() noexcept { called = true; }};
        d.Cancel();
    }
    ASSERT_FALSE(called);
}
