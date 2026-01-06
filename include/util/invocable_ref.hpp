#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace util {

template <typename T>
class InvocableRef {};

template <typename R, typename... Args>
class InvocableRef<R(Args...)> {
public:
    template <typename F>
        requires std::is_invocable_r_v<R, F&, Args...>
    explicit InvocableRef(F& func) noexcept
        : m_call_thunk{[](void* func_body, Args... args) -> R {
            return std::invoke(*reinterpret_cast<F*>(func_body), std::forward<Args>(args)...);
        }}
        , m_data{std::addressof(func)} {}

    explicit InvocableRef(R (*fptr)(Args...)) noexcept
        : m_call_thunk{[](void* fptr_raw, Args... args) {
            auto fptr = static_cast<R (*)(Args...)>(fptr_raw);
            return fptr(std::forward<Args>(args)...);
        }}
        , m_data{fptr} {}

    R operator()(Args... args) {
        return m_call_thunk(m_data, std::forward<Args>(args)...);
    }

    void Swap(InvocableRef& other) noexcept {
        std::swap(m_call_thunk, other.m_call_thunk);
        std::swap(m_data, other.m_data);
    }

private:
    R (*m_call_thunk)(void*, Args...);
    void* m_data;
};

template <typename R, typename... Args>
class InvocableRef<R(Args...) noexcept> {
public:
    template <typename F>
        requires std::is_nothrow_invocable_r_v<R, F&, Args...>
    explicit InvocableRef(F& func) noexcept
        : m_call_thunk{[](void* func_body, Args... args) noexcept -> R {
            return std::invoke(*static_cast<F*>(func_body), std::forward<Args>(args)...);
        }}
        , m_data{std::addressof(func)} {}

    explicit InvocableRef(R (*fptr)(Args...) noexcept) noexcept
        : m_call_thunk{[](void* fptr_raw, Args... args) {
            auto fptr = static_cast<R (*)(Args...) noexcept>(fptr_raw);
            return fptr(std::forward<Args>(args)...);
        }}
        , m_data{fptr} {}

    R operator()(Args... args) noexcept {
        return m_call_thunk(m_data, std::forward<Args>(args)...);
    }

    void Swap(InvocableRef& other) noexcept {
        std::swap(m_call_thunk, other.m_call_thunk);
        std::swap(m_data, other.m_data);
    }

private:
    R (*m_call_thunk)(void*, Args...) noexcept;
    void* m_data;
};

}// namespace util
