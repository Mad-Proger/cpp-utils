#pragma once

#include <cstddef>
#include <cstdint>

class UnixContext {
public:
    UnixContext() noexcept = default;
    explicit UnixContext(size_t stack_size);
    ~UnixContext();

    UnixContext(const UnixContext&) = delete;
    UnixContext& operator=(const UnixContext&) = delete;

    UnixContext(UnixContext&&) noexcept;
    UnixContext& operator=(UnixContext&&) noexcept;
    void Swap(UnixContext& other) noexcept;

    void Enter(UnixContext& target_context, void* data, void (*trampoline)(void*)) noexcept;
    void SwitchTo(UnixContext& target_context) noexcept;

private:
    uint8_t* m_stack{nullptr};
    uint8_t* m_stack_top{nullptr};
    size_t m_stack_size{0};
};
