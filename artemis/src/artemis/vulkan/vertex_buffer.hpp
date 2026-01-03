#pragma once
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class VertexBuffer {
  public:
    VertexBuffer(VulkanContext* context, void* data, size_t n);

  private:
    vk::Buffer buffer_;
    vk::DeviceMemory buffer_memory_;
};
} // namespace artemis
