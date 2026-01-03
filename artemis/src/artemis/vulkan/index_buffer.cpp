#include "index_buffer.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {
IndexBuffer::IndexBuffer(VulkanContext* context, void* indices, size_t size) {
    Buffer staging_buffer(context, size, vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent);
    void* mapped_staging = staging_buffer.map();
    memcpy(mapped_staging, indices, size);
    staging_buffer.unmap();

    buffer_ = Buffer(context, size,
                     vk::BufferUsageFlagBits::eIndexBuffer |
                         vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal);

    staging_buffer.copy_into(buffer_);
}
} // namespace artemis
