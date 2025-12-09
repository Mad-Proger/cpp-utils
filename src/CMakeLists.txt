set(UTIL_DIR "${HEADER_DIR}/util")
add_library(util INTERFACE
    "${UTIL_DIR}/defer.hpp"
    "${UTIL_DIR}/manual_storage.hpp"
)
target_compile_features(util INTERFACE cxx_std_20)
target_include_directories(util INTERFACE "${UTIL_DIR}")
add_library(cpp-utils::util ALIAS util)

add_subdirectory(coroutine)
