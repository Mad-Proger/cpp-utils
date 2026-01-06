#include "invocable_ref.hpp"

#include <gtest/gtest.h>

#include <tuple>
#include <utility>

TEST(InvocableRefBasic, Throwing) {
    auto f = [&]() {};

    util::InvocableRef<void()> func_ref{f};
    func_ref();
}

TEST(InvocableRefBasic, Noexcept) {
    auto f = []() noexcept {};

    util::InvocableRef<void() noexcept> ref_noexcept{f};
    ref_noexcept();

    util::InvocableRef<void()> ref_throwing{f};
    ref_throwing();
}

struct MoveOnly {
    MoveOnly() = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
    ~MoveOnly() = default;
};

static_assert(requires(MoveOnly mo, util::InvocableRef<void(MoveOnly)> func) { func(std::move(mo)); });

struct FailCopyConstruction {
    FailCopyConstruction() = default;
    FailCopyConstruction(const FailCopyConstruction&) {
        [] { FAIL() << "unexpected copy construction"; }();
    }
    FailCopyConstruction& operator=(const FailCopyConstruction&) = default;
    FailCopyConstruction(FailCopyConstruction&&) noexcept = default;
    FailCopyConstruction& operator=(FailCopyConstruction&&) noexcept = default;
};

struct FailCopyAssignment {
    FailCopyAssignment() = default;
    FailCopyAssignment(const FailCopyAssignment&) = default;
    FailCopyAssignment& operator=(const FailCopyAssignment&) {
        [] { FAIL() << "unexpected copy assignment"; }();
        return *this;
    }
    FailCopyAssignment(FailCopyAssignment&&) noexcept = default;
    FailCopyAssignment& operator=(FailCopyAssignment&&) noexcept = default;
};

struct FailMoveConstruction {
    FailMoveConstruction() = default;
    FailMoveConstruction(const FailMoveConstruction&) = default;
    FailMoveConstruction& operator=(const FailMoveConstruction&) = default;
    FailMoveConstruction(FailMoveConstruction&&) noexcept {
        [] { FAIL() << "unexpected move construction"; }();
    }
    FailMoveConstruction& operator=(FailMoveConstruction&&) noexcept = default;
};

struct FailMoveAssignment {
    FailMoveAssignment() = default;
    FailMoveAssignment(const FailMoveAssignment&) = default;
    FailMoveAssignment& operator=(const FailMoveAssignment&) = default;
    FailMoveAssignment(FailMoveAssignment&&) noexcept = default;
    FailMoveAssignment& operator=(FailMoveAssignment&&) noexcept {
        [] { FAIL() << "unexpected move assignment"; }();
        return *this;
    }
};

TEST(InvocableRefForwarding, NoCopy) {
    auto func = [](FailCopyConstruction&& arg) { [[maybe_unused]] auto val = std::move(arg); };

    util::InvocableRef<void(FailCopyConstruction&&)> ref{func};
    ref(FailCopyConstruction{});
}

TEST(InvocableRefForwarding, NoMove) {
    auto func = [](FailMoveConstruction arg) { [[maybe_unused]] auto val = arg; };

    util::InvocableRef<void(FailMoveConstruction&)> ref{func};
    FailMoveConstruction val{};
    ref(val);
}
