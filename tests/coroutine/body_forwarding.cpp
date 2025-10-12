#include "coroutine.hpp"

#include <gtest/gtest.h>

#include <concepts>

namespace {

struct MoveOnly {
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;

    void operator()(Coroutine::Handle) {}
};

}// namespace

static_assert(std::constructible_from<Coroutine, MoveOnly>, "coroutine must be able to construct from move only body");

TEST(Coroutine, MovePreferred) {
    static bool moved = false;
    struct PreferMove {
        PreferMove() = default;

        PreferMove(const PreferMove&) {
            []() { FAIL(); }();
        }
        PreferMove& operator=(const PreferMove&) {
            []() { FAIL(); }();
            return *this;
        }

        PreferMove(PreferMove&&) {
            moved = true;
        }
        PreferMove& operator=(PreferMove&&) {
            []() { FAIL(); }();
            return *this;
        }

        void operator()(Coroutine::Handle) {}
    };

    Coroutine coro{PreferMove{}};
    ASSERT_TRUE(moved);
}
