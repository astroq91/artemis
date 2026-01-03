#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
#include <cstddef>
namespace artemis {
class IndexBuffer {
  public:
    IndexBuffer() = default;
    /**
     * Creates, and allocates, a new index buffer with given data.
     * @param context The vulkan context.
     * @param cmd_buf The current command buffer (or nullptr if none).
     * @param indices The indices.
     * @param index_count The amount of indices.
     * @param index_size The size of a single index, in bytes.
     */
    IndexBuffer(VulkanContext* context, DeferredQueue* deferred_queue,
                CommandBuffer* cmd_buf, void* indices, size_t index_count,
                size_t index_size);

    Buffer& get_buffer() { return buffer_; }
    size_t get_index_count() const { return index_count_; }

  private:
    Buffer buffer_;
    size_t index_count_;
};
} // namespace artemis
