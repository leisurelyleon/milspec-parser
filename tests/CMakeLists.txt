include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(milspec_tests
    field_test.cpp
    parser_test.cpp
    validator_test.cpp
    result_test.cpp)

target_link_libraries(milspec_tests PRIVATE milspec GTest::gtest_main)

# Strict warnings are deliberately NOT applied here: GoogleTest's headers do
# not satisfy our -Werror flags. The library and CLI remain fully strict.

include(GoogleTest)
gtest_discover_tests(milspec_tests)
