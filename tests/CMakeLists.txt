cmake_minimum_required(VERSION 3.5)
project(tests)

add_executable(artemis_tests
  artemis/test_event_bus.cpp
)

target_link_libraries(artemis_tests
  PRIVATE
    artemis
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(artemis_tests)
