#pragma once

#include <cstddef>
#include <cstdint>

class LinuxContext {
public:
    LinuxContext() noexcept = default;
    explicit LinuxContext(size_t stack_size);
    ~LinuxContext();

    LinuxContext(const LinuxContext&) = delete;
    LinuxContext& operator=(const LinuxContext&) = delete;

    LinuxContext(LinuxContext&&) noexcept;
    LinuxContext& operator=(LinuxContext&&) noexcept;
    void Swap(LinuxContext& other) noexcept;

    void Enter(LinuxContext& target_context, void* data, void (*trampoline)(void*)) noexcept;
    void SwitchTo(LinuxContext& target_context) noexcept;

private:
    uint8_t* m_stack{nullptr};
    uint8_t* m_stack_top{nullptr};
    size_t m_stack_size{0};
};
