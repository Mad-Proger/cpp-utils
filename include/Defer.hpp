#pragma once

#include <concepts>
#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

template <std::invocable<> Func>
class Defer {
public:
    Defer(const Func& func) noexcept(std::is_nothrow_copy_constructible_v<Func>);
    Defer(Func&& func) noexcept(std::is_nothrow_move_constructible_v<Func>);
    ~Defer() noexcept(noexcept(std::invoke(std::declval<Func&&>)));

    Defer(const Defer&) = delete;
    Defer(Defer&&) = delete;

    Defer& operator=(const Defer&) = delete;
    Defer& operator=(Defer&&) = delete;

    void cancel() noexcept;

private:
    std::optional<Func> m_func;
};

template <std::invocable<> Func>
inline Defer<Func>::Defer(const Func& func) noexcept(std::is_nothrow_copy_constructible_v<Func>): m_func(func) {}

template <std::invocable<> Func>
inline Defer<Func>::Defer(Func&& func) noexcept(std::is_nothrow_move_constructible_v<Func>): m_func(std::move(func)) {}

template <std::invocable<> Func>
inline Defer<Func>::~Defer() noexcept(noexcept(std::invoke(std::declval<Func&&>))) {
    if (m_func.has_value()) {
        std::invoke(std::move(m_func).value());
    }
}

template <std::invocable<> Func>
inline void Defer<Func>::cancel() noexcept {
    m_func.reset();
}
