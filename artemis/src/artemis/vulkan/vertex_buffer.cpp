#include "vertex_buffer.hpp"
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {
VertexBuffer::VertexBuffer(VulkanContext* context,
                           DeferredQueue* deferred_queue, size_t size)
    : context_(context), deferred_queue_(deferred_queue) {
    buffer_ = Buffer(context, deferred_queue, size,
                     vk::BufferUsageFlagBits::eVertexBuffer |
                         vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal);
}
VertexBuffer::VertexBuffer(VulkanContext* context,
                           DeferredQueue* deferred_queue,
                           CommandBuffer* cmd_buf, void* vertices,
                           size_t size) {
    VertexBuffer(context, deferred_queue, size);
    insert(cmd_buf, vertices, size);
}
void VertexBuffer::insert(CommandBuffer* cmd_buf, void* vertices, size_t size) {
    Buffer staging_buffer(context_, deferred_queue_, size,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent);
    void* mapped_staging = staging_buffer.map();
    memcpy(mapped_staging, vertices, size);
    staging_buffer.unmap();

    staging_buffer.copy_into(cmd_buf, buffer_);
}
} // namespace artemis
