cmake_minimum_required(VERSION 3.5)

set(SOURCES
  main.cpp
)

add_executable(sandbox
  "${SOURCES}"
)

target_link_libraries(sandbox
  PRIVATE
    engine
)
