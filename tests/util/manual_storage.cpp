#include "manual_storage.hpp"

#include <gtest/gtest.h>

#include <cstddef>

struct alignas(256) Overaligned {};
static_assert(alignof(util::ManualStorage<Overaligned>) >= alignof(Overaligned));

struct Large {
    std::byte arr[1024];
};
static_assert(sizeof(util::ManualStorage<Large>) >= sizeof(Large));

struct LifetimeTracker {
    static inline int constructed = 0;
    static inline int destroyed = 0;

    LifetimeTracker() {
        ++constructed;
    }

    LifetimeTracker(LifetimeTracker&&) noexcept: LifetimeTracker() {}

    ~LifetimeTracker() {
        ++destroyed;
    }

    static void ResetCounters() {
        constructed = 0;
        destroyed = 0;
    }
};

TEST(ManualStorageLifetime, EmplaceDestroy) {
    LifetimeTracker::ResetCounters();

    util::ManualStorage<LifetimeTracker> s;
    ASSERT_EQ(LifetimeTracker::constructed, 0);

    s.Emplace();
    ASSERT_EQ(LifetimeTracker::constructed, 1);
    ASSERT_EQ(LifetimeTracker::destroyed, 0);

    s.Destroy();
    ASSERT_EQ(LifetimeTracker::destroyed, 1);
}

TEST(ManualStorageLifetime, Take) {
    LifetimeTracker::ResetCounters();

    util::ManualStorage<LifetimeTracker> s;
    s.Emplace();

    {
        auto v = s.Take();
        ASSERT_EQ(LifetimeTracker::constructed - LifetimeTracker::destroyed, 1);
    }

    ASSERT_EQ(LifetimeTracker::constructed, LifetimeTracker::destroyed);
}
