cmake_minimum_required(VERSION 3.5)
project(artemis)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SLANGC_EXECUTABLE "slangc")
include("cmake/slang.cmake")
add_subdirectory("artemis")
add_subdirectory("sandbox")

option(BUILD_TESTS "Build the test suite" OFF)

if(BUILD_TESTS)
enable_testing()
add_subdirectory(vendor/googletest)
  add_subdirectory(tests)
endif()
