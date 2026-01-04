#include "index_buffer.hpp"
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {
IndexBuffer::IndexBuffer(VulkanContext* context, DeferredQueue* deferred_queue,
                         CommandBuffer* cmd_buf, void* indices,
                         size_t index_count, size_t index_size)
    : buffer_(context, deferred_queue, index_count * index_size,
              vk::BufferUsageFlagBits::eIndexBuffer |
                  vk::BufferUsageFlagBits::eTransferDst,
              vk::MemoryPropertyFlagBits::eDeviceLocal),
      index_count_(index_count) {
    auto size = index_count * index_size;
    Buffer staging_buffer(context, deferred_queue, size,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent);
    void* mapped_staging = staging_buffer.map();
    memcpy(mapped_staging, indices, size);
    staging_buffer.unmap();

    staging_buffer.copy_into(cmd_buf, buffer_);
}
} // namespace artemis
