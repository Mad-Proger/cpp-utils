add_executable(coro_basic_tests basic.cpp)
target_link_libraries(coro_basic_tests
    PUBLIC GTest::gtest_main
    PRIVATE coro
)
gtest_discover_tests(coro_basic_tests)
