#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class VertexBuffer {
  public:
    VertexBuffer() = default;
    VertexBuffer(VulkanContext* context, DeferredQueue* deferred_queue,
                 size_t size);
    VertexBuffer(VulkanContext* context, DeferredQueue* deferred_queue,
                 CommandBuffer* cmd_buf, void* vertices, size_t size);

    Buffer& get_buffer() { return buffer_; }
    /**
     * Inserts data into the vertex buffer.
     * @param cmd_buf The command buffer (or nullptr if none is active).
     * @param vertices The vertex data.
     * @param size The size of the vertex data in bytes.
     */
    void insert(CommandBuffer* cmd_buf, void* vertices, size_t size);

  private:
    VulkanContext* context_;
    DeferredQueue* deferred_queue_;
    Buffer buffer_;
};
} // namespace artemis
