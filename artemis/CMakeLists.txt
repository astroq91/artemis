cmake_minimum_required(VERSION 3.5)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)

set(SRC 
  src/artemis
)

set(SOURCES
  "${SRC}/core/application.cpp"
  "${SRC}/core/window.cpp"
  "${SRC}/utils/logger.cpp"
  "${SRC}/core/log.cpp"
  "${SRC}/events/event_bus.cpp"
)

add_library(artemis
  "${SOURCES}"
)

target_include_directories(artemis
  PUBLIC
    src
)

target_link_libraries(artemis
  PUBLIC
    glfw
    spdlog
)
