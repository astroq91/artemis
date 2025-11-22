cmake_minimum_required(VERSION 3.5)
project(artemis)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("artemis")
add_subdirectory("sandbox")

option(BUILD_TESTS "Build the test suite" OFF)

if(BUILD_TESTS)
enable_testing()
add_subdirectory("vendor/googletest")
add_subdirectory("tests/artemis")
endif()
