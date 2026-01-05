#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class UniformBuffer {
  public:
    UniformBuffer() = default;
    UniformBuffer(VulkanContext* context, DeferredQueue* deferred_queue,
                  size_t n);

    /**
     * Creates, and allocates, a new index buffer with given data.
     * @param context The vulkan context.
     * @param data The data.
     * @param n The amount of bytes.
     */
    UniformBuffer(VulkanContext* context, DeferredQueue* deferred_queue,
                  void* data, size_t n);

    Buffer& get_buffer() { return buffer_; }

    /**
     * Inserts data into the uniform buffer.
     * @param cmd_buf The command buffer (or nullptr if none is active).
     * @param vertices The data.
     * @param size The amount of bytes.
     */
    void insert(void* data, size_t n);

  private:
    VulkanContext* context_;
    DeferredQueue* deferred_queue_;
    Buffer buffer_;
    void* mapped_data_;
};
} // namespace artemis
