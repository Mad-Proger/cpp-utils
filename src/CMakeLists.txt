add_library(defer INTERFACE ${HEADER_DIR}/Defer.hpp)
add_library(cpp-utils::defer ALIAS defer)

target_compile_features(defer INTERFACE cxx_std_20)
target_include_directories(defer INTERFACE ${HEADER_DIR})

add_library(memory-map STATIC MemoryMap.cpp)
add_library(cpp-utils::memory-map ALIAS memory-map)
target_include_directories(memory-map PUBLIC ${HEADER_DIR})
target_link_libraries(memory-map PRIVATE defer)
target_compile_features(memory-map PUBLIC cxx_std_20)
