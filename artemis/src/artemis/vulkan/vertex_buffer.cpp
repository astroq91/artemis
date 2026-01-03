#include "vertex_buffer.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {
VertexBuffer::VertexBuffer(VulkanContext* context, size_t size)
    : context_(context) {
    buffer_ = Buffer(context, size,
                     vk::BufferUsageFlagBits::eVertexBuffer |
                         vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal);
}
VertexBuffer::VertexBuffer(VulkanContext* context, void* vertices,
                           size_t size) {
    VertexBuffer(context, size);
    Buffer staging_buffer(context, size, vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent);
    void* mapped_staging = staging_buffer.map();
    memcpy(mapped_staging, vertices, size);
    staging_buffer.unmap();

    staging_buffer.copy_into(buffer_);
}
void VertexBuffer::insert(void* vertices, size_t size) {
    Buffer staging_buffer(context_, size, vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent);
    void* mapped_staging = staging_buffer.map();
    memcpy(mapped_staging, vertices, size);
    staging_buffer.unmap();

    staging_buffer.copy_into(buffer_);
}
} // namespace artemis
