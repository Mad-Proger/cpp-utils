add_library(defer INTERFACE ${HEADER_DIR}/Defer.hpp)
add_library(cpp-utils::defer ALIAS defer)

target_compile_features(defer INTERFACE cxx_std_20)
target_include_directories(defer INTERFACE ${HEADER_DIR})
