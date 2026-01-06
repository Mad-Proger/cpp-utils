#pragma once

#include "defer.hpp"

#include <concepts>
#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace util {

template <typename T>
class ManualStorage {
public:
    ManualStorage() noexcept = default;

    ManualStorage(const ManualStorage&) = delete;
    ManualStorage& operator=(const ManualStorage&) = delete;
    ManualStorage(ManualStorage&&) = delete;
    ManualStorage& operator=(ManualStorage&&) = delete;

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    void Emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        std::construct_at(GetPointer(), std::forward<Args>(args)...);
    }

    void Destroy() noexcept(std::is_nothrow_destructible_v<T>) {
        std::destroy_at(GetPointer());
    }

    T Take() noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>) {
        Defer destroy{[this]() noexcept(std::is_nothrow_destructible_v<T>) { Destroy(); }};
        return std::move(Get());
    }

    T& Get() noexcept {
        return *GetPointer();
    }

    const T& Get() const noexcept {
        return *GetPointer();
    }

    T& operator*() noexcept {
        return Get();
    }

    const T& operator*() const noexcept {
        return Get();
    }

    T* operator->() noexcept {
        return GetPointer();
    }

    const T* operator->() const noexcept {
        return GetPointer();
    }

private:
    T* GetPointer() noexcept {
        return std::launder(reinterpret_cast<T*>(m_objectStorage));
    }

    const T* GetPointer() const noexcept {
        return std::launder(reinterpret_cast<const T*>(m_objectStorage));
    }

private:
    alignas(T) std::byte m_objectStorage[sizeof(T)];
};

}// namespace util
