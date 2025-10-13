add_library(coro
    coroutine.cpp
    unix/context_switch.s
    unix/linux_context.cpp
)
target_include_directories(coro PUBLIC ${HEADER_DIR}/coroutine)
target_compile_features(coro PUBLIC cxx_std_20)
add_library(cpp-utils::coro ALIAS defer)
