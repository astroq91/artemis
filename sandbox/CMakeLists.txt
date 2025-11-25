cmake_minimum_required(VERSION 3.5)

set(SOURCES
  src/main.cpp
  src/sandbox.cpp
)

set(SHADERS_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders"
)

add_executable(sandbox
  "${SOURCES}"
)

target_link_libraries(sandbox
  PRIVATE
    artemis 
)

target_compile_definitions(sandbox
  PRIVATE
    RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

target_add_slang_shaders(sandbox_shaders 
  SOURCES
    "${SHADERS_DIR}/triangle.slang"
)

add_dependencies(sandbox sandbox_shaders)
