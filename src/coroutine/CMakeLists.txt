if(UNIX)
    set(CORO_IMPL
        unix/context_switch.s
        unix/unix_context.cpp
    )
elseif(WIN32)
    set(CORO_IMPL windows_context.cpp)
else()
    message(FATAL_ERROR "Unsupported platform for coroutines")
endif()

add_library(coro
    coroutine.cpp
    ${CORO_IMPL}
)
target_include_directories(coro PUBLIC ${HEADER_DIR}/coroutine)
target_compile_features(coro PUBLIC cxx_std_20)
add_library(cpp-utils::coro ALIAS defer)
