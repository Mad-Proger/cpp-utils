add_executable(util_tests
    defer.cpp
    invocable_ref.cpp
    manual_storage.cpp
)

target_link_libraries(util_tests
    PUBLIC GTest::gtest_main
    PRIVATE cpp-utils::util
)
gtest_discover_tests(util_tests)
