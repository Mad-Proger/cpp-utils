#pragma once

#include "os_context.hpp"

#include <concepts>
#include <cstddef>
#include <exception>
#include <functional>

class Coroutine {
public:
    class Handle {
    public:
        explicit Handle(Coroutine& coro) noexcept;
        void Yield() noexcept;

    private:
        Coroutine& m_coro;
    };

    Coroutine() noexcept = default;
    template <std::invocable<Handle> Body>
    explicit Coroutine(Body&& body);

    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;

    Coroutine(Coroutine&&) noexcept;
    Coroutine& operator=(Coroutine&&) noexcept;

    void Swap(Coroutine& other) noexcept;

    void Run();
    bool IsDone() const noexcept;

private:
    void Yield() noexcept;

private:
    OsContext m_coro_context{};
    OsContext m_return_context{};
    bool m_finished{true};
    std::exception_ptr m_exception{};

    static constexpr size_t STACK_SIZE = 8 * 1024 * 1024;
};

template <std::invocable<Coroutine::Handle> Body>
inline Coroutine::Coroutine(Body&& body): m_coro_context{STACK_SIZE}
                                        , m_finished{false} {
    struct TrampolineData {
        Body& body;
        Coroutine& coro;
    } trampoline_data{body, *this};

    m_return_context.Enter(m_coro_context, &trampoline_data, [](void* data_raw) {
        auto data = reinterpret_cast<TrampolineData*>(data_raw);
        Coroutine& coro = data->coro;

        try {
            auto func = std::forward<Body>(data->body);
            coro.Yield();
            std::invoke(std::move(func), Handle{coro});
        } catch (...) {
            coro.m_exception = std::current_exception();
        }

        coro.m_finished = true;
        coro.Yield();
    });

    if (m_exception) std::rethrow_exception(m_exception);
}
