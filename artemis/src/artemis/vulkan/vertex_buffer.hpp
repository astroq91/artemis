#pragma once
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class VertexBuffer {
  public:
    VertexBuffer() = default;
    VertexBuffer(VulkanContext* context, size_t size);
    VertexBuffer(VulkanContext* context, void* vertices, size_t size);

    Buffer& get_buffer() { return buffer_; }
    void insert(void* vertices, size_t size);

  private:
    VulkanContext* context_;
    Buffer buffer_;
};
} // namespace artemis
