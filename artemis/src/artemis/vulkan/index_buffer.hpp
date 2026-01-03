#pragma once
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class IndexBuffer {
  public:
    IndexBuffer();
    IndexBuffer(VulkanContext* context, void* indices, size_t size);

  private:
    Buffer buffer_;
};
} // namespace artemis
