if(UNIX)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
        set(ARCH_NAME aarch64)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
        set(ARCH_NAME x86_64)
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set(CORO_IMPL
        unix/unix_context.cpp
        unix/context_switch_${ARCH_NAME}.s
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
