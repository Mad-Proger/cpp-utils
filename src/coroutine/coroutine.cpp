#include "coroutine.hpp"

#include <cassert>
#include <exception>
#include <stdexcept>
#include <utility>

Coroutine::Handle::Handle(Coroutine& coro) noexcept: m_coro{coro} {}

Coroutine::Coroutine(Coroutine&& other) noexcept: Coroutine() {
    Swap(other);
}

void Coroutine::Handle::Yield() noexcept {
    m_coro.Yield();
}

void Coroutine::Handle::SwitchTo(Handle target) noexcept {
    m_coro.SwitchTo(target.m_coro);
}

Coroutine& Coroutine::operator=(Coroutine&& other) noexcept {
    Coroutine moved = std::move(other);
    Swap(moved);
    return *this;
}

void Coroutine::Swap(Coroutine& other) noexcept {
    m_coro_context.Swap(other.m_coro_context);
    m_return_context.Swap(other.m_return_context);
    std::swap(m_finished, other.m_finished);
    std::swap(m_exception, other.m_exception);
}

void Coroutine::Run() {
    if (IsDone()) throw std::runtime_error{"trying to run finished coroutine"};
    m_return_context.SwitchTo(m_coro_context);
    if (m_exception) std::rethrow_exception(m_exception);
}

bool Coroutine::IsDone() const noexcept {
    return m_finished;
}

void Coroutine::Yield() noexcept {
    m_coro_context.SwitchTo(m_return_context);
}

void Coroutine::SwitchTo(Coroutine& target) noexcept {
    m_return_context.Swap(target.m_return_context);
    m_coro_context.SwitchTo(target.m_coro_context);
}
