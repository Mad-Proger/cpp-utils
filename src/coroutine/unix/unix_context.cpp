#include "unix_context.hpp"

#include <sys/mman.h>

#include <cassert>
#include <cstddef>
#include <new>
#include <utility>

extern "C" {
void SwitchContext(uint8_t* new_stack, uint8_t** old_stack);
void SetupContext(uint8_t* new_stack, uint8_t** old_stack, void* data, void (*trampoline)(void*));
}

UnixContext::UnixContext(size_t stack_size): m_stack{nullptr}, m_stack_size{stack_size} {
    void* stack = mmap(nullptr, stack_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) throw std::bad_alloc{};
    m_stack = reinterpret_cast<uint8_t*>(stack);
    m_stack_top = m_stack + m_stack_size;
}

UnixContext::~UnixContext() {
    if (m_stack == nullptr) return;
    assert(munmap(m_stack, m_stack_size) == 0 && "could not unmap stack");
}

UnixContext::UnixContext(UnixContext&& other) noexcept: UnixContext() {
    Swap(other);
}

UnixContext& UnixContext::operator=(UnixContext&& other) noexcept {
    UnixContext moved = std::move(other);
    Swap(moved);
    return *this;
}

void UnixContext::Swap(UnixContext& other) noexcept {
    std::swap(m_stack, other.m_stack);
    std::swap(m_stack_top, other.m_stack_top);
    std::swap(m_stack_size, other.m_stack_size);
}

void UnixContext::Enter(UnixContext& target_context, void* data, void (*trampoline)(void*)) noexcept {
    SetupContext(target_context.m_stack_top, &m_stack_top, data, trampoline);
}

void UnixContext::SwitchTo(UnixContext& target_context) noexcept {
    SwitchContext(target_context.m_stack_top, &m_stack_top);
}
