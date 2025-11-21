cmake_minimum_required(VERSION 3.5)

set(SOURCES
  src/main.cpp
  src/sandbox.cpp
  src/log.cpp
)

add_executable(sandbox
  "${SOURCES}"
)

target_link_libraries(sandbox
  PRIVATE
    artemis 
)
