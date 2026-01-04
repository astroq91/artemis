#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include "vulkan/vulkan.hpp"
namespace artemis {
class Buffer {
  public:
    Buffer() = default;
    Buffer(VulkanContext* context, DeferredQueue* deferred_queue, size_t size,
           vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    vk::Buffer get_vk_buffer() { return buffer_; }
    vk::DeviceMemory get_vk_buffer_memory() { return buffer_memory_; }

    void* map() {
        return context_->device->mapMemory(buffer_memory_, 0, memory_size_);
    }
    void unmap() { context_->device->unmapMemory(buffer_memory_); }

    /**
     * Copy this buffer to other.
     * @param command_buffer The command buffer (or nullptr if none is active).
     * @param other The other buffer.
     */
    void copy_into(CommandBuffer* cmd_buf, Buffer& other) const;

  private:
    VulkanContext* context_;
    DeferredQueue* deferred_queue_;
    vk::Buffer buffer_{nullptr};
    vk::DeviceMemory buffer_memory_;
    vk::DeviceSize buffer_size_;
    vk::DeviceSize memory_size_;
};
} // namespace artemis
