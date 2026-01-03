#pragma once
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class IndexBuffer {
  public:
    IndexBuffer();
    IndexBuffer(VulkanContext* context, void* indices, size_t index_count,
                size_t index_size);

    Buffer& get_buffer() { return buffer_; }
    size_t get_index_count() const { return index_count_; }

  private:
    Buffer buffer_;
    size_t index_count_;
};
} // namespace artemis
