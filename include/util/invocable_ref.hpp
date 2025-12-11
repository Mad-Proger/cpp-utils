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
            return std::invoke(*reinterpret_cast<F*>(func_body), std::move(args)...);
        }}
        , m_data{std::addressof(func)} {}

    explicit InvocableRef(R (*fptr)(Args...)) noexcept
        : m_call_thunk{[](R (*fptr)(Args...), Args... args) { return fptr(std::move(args)...); }}
        , m_data{fptr} {}

    R operator()(Args... args) {
        return m_call_thunk(m_data, std::move(args)...);
    }

    void Swap(InvocableRef& other) {
        std::swap(m_call_thunk, other.m_call_thunk);
        std::swap(m_data, other.m_data);
    }

private:
    R (*m_call_thunk)(void*, Args...);
    void* m_data;
};

}// namespace util
