include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    ON
    GIT_PROGRESS   ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_subdirectory(coroutine)
add_subdirectory(util)
