#pragma once
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include "vulkan/vulkan.hpp"
namespace artemis {
class Buffer {
  public:
    Buffer() = default;
    Buffer(VulkanContext* context, size_t size, vk::BufferUsageFlags usage,
           vk::MemoryPropertyFlags properties);

    vk::Buffer get_vk_buffer() { return buffer_; }
    vk::DeviceMemory get_vk_buffer_memory() { return buffer_memory_; }

    void* map() {
        return context_->device->mapMemory(buffer_memory_, 0, memory_size_);
    }
    void unmap() { context_->device->unmapMemory(buffer_memory_); }

    /**
     * Copy this buffer to other.
     * @param other The other buffer.
     * @param command_buffer The command buffer (or nullptr if none is active).
     */
    void copy_into(Buffer& other,
                   CommandBuffer* command_buffer = nullptr) const;

  private:
    VulkanContext* context_;
    vk::Buffer buffer_;
    vk::DeviceMemory buffer_memory_;
    vk::DeviceSize buffer_size_;
    vk::DeviceSize memory_size_;
};
} // namespace artemis
