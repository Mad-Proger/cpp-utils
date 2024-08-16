add_library(defer INTERFACE ${HEADER_DIR}/Defer.hpp)
add_library(cpp-utils::defer ALIAS defer)

target_compile_features(defer INTERFACE cxx_std_20)
target_include_directories(defer INTERFACE ${HEADER_DIR})

add_library(memory-map STATIC MemoryMap.cpp)
target_include_directories(memory-map PUBLIC ${HEADER_DIR})
target_link_libraries(memory-map PRIVATE defer)
