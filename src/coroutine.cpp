#include "coroutine.hpp"

#include <sys/mman.h>

#include <cassert>
#include <cstdint>
#include <exception>
#include <new>
#include <stdexcept>
#include <utility>

namespace {

extern "C" {
void SwitchContext(uint8_t* new_stack, uint8_t** old_stack);
void CreateContext(uint8_t* new_stack, void* data, uint8_t* (*trampoline)(uint8_t*, void*) );
}

}// namespace

Coroutine::Handle::Handle(Coroutine& coro) noexcept: m_coro{coro} {}

Coroutine::~Coroutine() {
    assert(munmap(m_stack, STACK_SIZE) == 0 && "could not unmap stack");
}

Coroutine::Coroutine(Coroutine&& other) noexcept: Coroutine() {
    Swap(other);
}

Coroutine& Coroutine::operator=(Coroutine&& other) noexcept {
    Coroutine moved = std::move(other);
    Swap(moved);
    return *this;
}

void Coroutine::Swap(Coroutine& other) noexcept {
    std::swap(m_stack, other.m_stack);
    std::swap(m_finished, other.m_finished);
    std::swap(m_target_stack, other.m_target_stack);
}

void Coroutine::Handle::Yield() noexcept {
    m_coro.Switch();
}

void Coroutine::Run() {
    if (IsDone()) throw std::runtime_error{"trying to run finished coroutine"};
    Switch();
    if (m_exception) std::rethrow_exception(m_exception);
}

bool Coroutine::IsDone() const noexcept {
    return m_finished;
}

uint8_t* Coroutine::AllocateStack() {
    void* stack = mmap(nullptr, STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) throw std::bad_alloc{};
    return reinterpret_cast<uint8_t*>(stack);
}

void Coroutine::SetupStack(void* data, uint8_t* (*trampoline)(uint8_t*, void*) ) noexcept {
    CreateContext(m_stack + STACK_SIZE, data, trampoline);
}

void Coroutine::Switch() noexcept {
    SwitchContext(m_target_stack, &m_target_stack);
}
