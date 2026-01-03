cmake_minimum_required(VERSION 3.5)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)

set(SRC 
  src/artemis
)

set(SOURCES
  "${SRC}/core/application.cpp"
  "${SRC}/core/window.cpp"
  "${SRC}/core/log.cpp"
  "${SRC}/renderer/renderer.cpp"
  "${SRC}/renderer/instancer.cpp"
  "${SRC}/vulkan/vulkan_utils.cpp"
  "${SRC}/vulkan/vulkan_context.cpp"
  "${SRC}/vulkan/swap_chain.cpp"
  "${SRC}/vulkan/shader.cpp"
  "${SRC}/vulkan/pipeline.cpp"
  "${SRC}/vulkan/command_buffer.cpp"
  "${SRC}/vulkan/buffer.cpp"
  "${SRC}/vulkan/index_buffer.cpp"
  "${SRC}/vulkan/vertex_buffer.cpp"
  "${SRC}/vulkan/vertex_buffer_description.cpp"
  "${SRC}/events/event_bus.cpp"
  "${SRC}/utils/logger.cpp"
  "${SRC}/utils/io.cpp"
  "${SRC}/assets/deferred_queue.cpp"
)

set(SHADERS_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders"
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
    glm
)

target_compile_definitions(artemis
  PRIVATE
    GLFW_INCLUDE_VULKAN
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
    RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
  PUBLIC
    VK_NO_PROTOTYPES
    GLM_ENABLE_EXPERIMENTAL 
)

target_add_slang_shaders(artemis_shaders 
  SOURCES
    "${SHADERS_DIR}/triangle.slang"
    "${SHADERS_DIR}/forward.slang"
)

add_dependencies(artemis artemis_shaders)
