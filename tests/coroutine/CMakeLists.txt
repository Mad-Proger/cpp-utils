add_executable(coro_tests
    basic.cpp
    body_forwarding.cpp
)
target_compile_features(coro_tests PUBLIC cxx_std_20)
target_link_libraries(coro_tests
    PUBLIC GTest::gtest_main
    PRIVATE coro
)
gtest_discover_tests(coro_tests)
