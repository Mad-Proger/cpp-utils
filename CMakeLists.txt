cmake_minimum_required(VERSION 3.24)

project(cpp-utils CXX ASM)
set(HEADER_DIR "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(tests)
