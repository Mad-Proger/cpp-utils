#include "coroutine.hpp"

#include <cstdint>

namespace {

extern "C" {
void SwitchContext(uint8_t* new_stack, uint8_t** old_stack);
void CreateContext(uint8_t* new_stack, void* data, uint8_t* (*trampoline)(uint8_t*, void*) );
}

}// namespace

Coroutine::Handle::Handle(Coroutine& coro) noexcept: m_coro{coro} {}

void Coroutine::Handle::Yield() {
    m_coro.Switch();
}

void Coroutine::Run() {
    Switch();
}

bool Coroutine::IsDone() const noexcept {
    return m_finished;
}

void Coroutine::SetupStack(void* data, uint8_t* (*trampoline)(uint8_t*, void*) ) {
    CreateContext(m_stack.get(), data, trampoline);
}

void Coroutine::Switch() {
    SwitchContext(m_target_stack, &m_target_stack);
}
