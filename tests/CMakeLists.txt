cmake_minimum_required(VERSION 3.5)
project(tests)

set(RESOURCES_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

add_executable(artemis_tests
  artemis/test_event_bus.cpp
  artemis/test_render_triangle.cpp
)

target_link_libraries(artemis_tests
  PRIVATE
    artemis
    GTest::gtest_main
)

target_compile_definitions(artemis_tests
  PRIVATE
    RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

include(GoogleTest)
gtest_discover_tests(artemis_tests)

target_add_slang_shaders(artemis_tests_shaders 
  SOURCES
  "${RESOURCES_DIR}/shaders/triangle.slang"
)

add_dependencies(artemis_tests artemis_tests_shaders)
