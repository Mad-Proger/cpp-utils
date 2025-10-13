#include "windows_context.hpp"

#include <windows.h>

#include <algorithm>
#include <system_error>

WindowsContext::WindowsContext(): m_fiber{GetCurrentFiber()} {
    if (IsThreadAFiber()) return;
    m_fiber = ConvertThreadToFiber(nullptr);
    if (!m_fiber) {
        DWORD error_code = GetLastError();
        throw std::system_error{static_cast<int>(error_code), std::system_category(),
                                "current thread is not a fiber and could not be converted into one"};
    }
}

WindowsContext::WindowsContext(size_t stack_size)
    : m_fiber_data{new FiberData{}}
    , m_fiber{CreateFiber(stack_size, &FiberMain, m_fiber_data.get())} {
    if (!m_fiber) {
        DWORD error_code = GetLastError();
        throw std::system_error{static_cast<int>(error_code), std::system_category(), "could not create fiber"};
    }
}

WindowsContext::~WindowsContext() {
    if (!m_fiber_data) return;
    DeleteFiber(m_fiber);
}

WindowsContext::WindowsContext(WindowsContext&& other) noexcept: WindowsContext() {
    Swap(other);
}

WindowsContext& WindowsContext::operator=(WindowsContext&& other) noexcept {
    WindowsContext moved = std::move(other);
    Swap(moved);
    return *this;
}

void WindowsContext::Swap(WindowsContext& other) noexcept {
    m_fiber_data.swap(other.m_fiber_data);
    std::swap(m_fiber, other.m_fiber);
}

void WindowsContext::Enter(WindowsContext& target_context, void* data, void (*trampoline)(void*)) noexcept {
    target_context.m_fiber_data->trampoline = trampoline;
    target_context.m_fiber_data->trampoline_data = data;
    SwitchToFiber(target_context.m_fiber);
}

void WindowsContext::SwitchTo(WindowsContext& target_context) noexcept {
    SwitchToFiber(target_context.m_fiber);
}

void WindowsContext::FiberMain(void* data) {
    auto fiber_data = reinterpret_cast<FiberData*>(data);
    fiber_data->trampoline(fiber_data->trampoline_data);
}
