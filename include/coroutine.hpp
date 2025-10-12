#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>

class Coroutine {
public:
    class Handle {
    public:
        explicit Handle(Coroutine& coro) noexcept;
        void Yield();

    private:
        Coroutine& m_coro;
    };

    Coroutine() noexcept = default;
    template <std::invocable<Handle> Body>
    explicit Coroutine(Body&& body);
    ~Coroutine();

    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;

    Coroutine(Coroutine&&) noexcept;
    Coroutine& operator=(Coroutine&&) noexcept;

    void Swap(Coroutine& other) noexcept;

    void Run();
    bool IsDone() const noexcept;

private:
    uint8_t* m_stack{nullptr};
    bool m_finished{true};
    uint8_t* m_target_stack{nullptr};

    uint8_t* AllocateStack();
    void SetupStack(void* data, uint8_t* (*trampoline)(uint8_t*, void*) );
    void Switch();

    static constexpr size_t STACK_SIZE = 8 * 1024 * 1024;
};

template <std::invocable<Coroutine::Handle> Body>
inline Coroutine::Coroutine(Body&& body): m_stack{AllocateStack()}
                                        , m_finished{false}
                                        , m_target_stack{m_stack} {
    struct TrampolineData {
        Body& body;
        Coroutine& coro;
    } trampoline_data{body, *this};
    SetupStack(&trampoline_data, [](uint8_t* old_stack, void* data_raw) {
        auto data = reinterpret_cast<TrampolineData*>(data_raw);
        auto func = std::forward<Body>(data->body);
        Coroutine& coro = data->coro;

        coro.m_target_stack = old_stack;
        coro.Switch();
        std::invoke(std::move(func), Handle{coro});
        coro.m_finished = true;
        return coro.m_target_stack;
    });
}
