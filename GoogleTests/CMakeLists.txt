include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(manpass_tests
        testoftest.cpp
)

target_link_libraries(manpass_tests
        GTest::gtest_main
        manpass_core
)

include(GoogleTest)
gtest_discover_tests(manpass_tests)