set(SOURCES MemoryMap.cpp)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SOURCES ${SOURCES} MemoryMapLinux.cpp)
endif()

add_library(memory-map STATIC ${SOURCES})
add_library(cpp-utils::memory-map ALIAS memory-map)

target_include_directories(memory-map PUBLIC "${HEADER_DIR}/MemoryMap")
target_link_libraries(memory-map PRIVATE defer)
target_compile_features(memory-map PUBLIC cxx_std_20)
