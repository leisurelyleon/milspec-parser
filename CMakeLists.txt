cmake_minimum_required(VERSION 3.20)

project(milspec_parser
    VERSION 0.1.0
    DESCRIPTION "Strict fixed-format data parser and validator for federal records"
    LANGUAGES CXX)

# --- Language standard (pinned; fail loudly on too-old compilers) ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)

# --- The parsing/validation library ---
add_library(milspec
    src/field.cpp
    src/schema.cpp
    src/parser.cpp
    src/validator.cpp
    src/result.cpp)

target_include_directories(milspec PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(milspec PUBLIC cxx_std_20)
set_project_warnings(milspec)

# --- The CLI binary ---
add_executable(milspec-cli src/cli/main.cpp)
target_link_libraries(milspec-cli PRIVATE milspec)
set_project_warnings(milspec-cli)

# --- Tests (on by default) ---
option(MILSPEC_BUILD_TESTS "Build the test suite" ON)
if(MILSPEC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
