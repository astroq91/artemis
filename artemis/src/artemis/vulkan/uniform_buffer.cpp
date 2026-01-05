#include "uniform_buffer.hpp"
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/buffer.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {
UniformBuffer::UniformBuffer(VulkanContext* context,
                             DeferredQueue* deferred_queue, size_t n)
    : context_(context), deferred_queue_(deferred_queue),
      buffer_(context, deferred_queue, n,
              vk::BufferUsageFlagBits::eUniformBuffer,
              vk::MemoryPropertyFlagBits::eHostVisible |
                  vk::MemoryPropertyFlagBits::eHostCoherent) {
    mapped_data_ = buffer_.map();
}

UniformBuffer::UniformBuffer(VulkanContext* context,
                             DeferredQueue* deferred_queue, void* data,
                             size_t n)
    : UniformBuffer(context, deferred_queue, n) {
    insert(data, n);
}

void UniformBuffer::insert(void* data, size_t n) {
    memcpy(mapped_data_, data, n);
}
} // namespace artemis
