#pragma once

#include <memory>

class WindowsContext {
public:
    WindowsContext();
    explicit WindowsContext(size_t stack_size);
    ~WindowsContext();

    WindowsContext(const WindowsContext&) = delete;
    WindowsContext& operator=(const WindowsContext&) = delete;

    WindowsContext(WindowsContext&&) noexcept;
    WindowsContext& operator=(WindowsContext&&) noexcept;
    void Swap(WindowsContext& other) noexcept;

    void Enter(WindowsContext& target_context, void* data, void (*trampoline)(void*)) noexcept;
    void SwitchTo(WindowsContext& target_context) noexcept;

private:
    static void FiberMain(void* data);

private:
    struct FiberData {
        void (*trampoline)(void*);
        void* trampoline_data;
    };
    std::unique_ptr<FiberData> m_fiber_data{nullptr};
    using LPVOID = void*;
    LPVOID m_fiber{nullptr};
};
