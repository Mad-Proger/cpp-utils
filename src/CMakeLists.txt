add_library(defer INTERFACE ${HEADER_DIR}/defer.hpp)
target_compile_features(defer INTERFACE cxx_std_20)
target_include_directories(defer INTERFACE ${HEADER_DIR})
add_library(cpp-utils::defer ALIAS defer)

add_subdirectory(coroutine)
