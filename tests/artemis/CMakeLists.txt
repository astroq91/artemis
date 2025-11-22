cmake_minimum_required(VERSION 3.5)

add_executable(artemis_tests
  test_event_bus.cpp
)

target_link_libraries(artemis_tests
  PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(artemis_tests)
