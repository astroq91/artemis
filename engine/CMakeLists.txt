cmake_minimum_required(VERSION 3.5)

set(SOURCES
  core/application.cpp 
)

add_library(engine
  "${SOURCES}"
)

target_include_directories(engine
  PUBLIC
    .
)
