cmake_minimum_required(VERSION 3.5)

set(SRC 
  src/artemis
)

set(SOURCES
  "${SRC}/core/application.cpp"
)

add_library(artemis
  "${SOURCES}"
)

target_include_directories(artemis
  PUBLIC
    src
)
