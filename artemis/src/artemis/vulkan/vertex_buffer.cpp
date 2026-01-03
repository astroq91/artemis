#include "vertex_buffer.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"

namespace artemis {
VertexBuffer::VertexBuffer(VulkanContext* context, void* data, size_t n) {
    vk::BufferCreateInfo buffer_info({}, n,
                                     vk::BufferUsageFlagBits::eVertexBuffer);
    if (context->device->createBuffer(&buffer_info, nullptr, &buffer_) !=
        vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to create vertex buffer.");
        return;
    }

    auto mem_requirements =
        context->device->getBufferMemoryRequirements(buffer_);
    vk::MemoryAllocateInfo allocate_info(
        mem_requirements.size,
        VulkanUtils::find_memory_type(
            context->physical_device.get(), mem_requirements.memoryTypeBits,
            vk::MemoryPropertyFlagBits::eHostVisible |
                vk::MemoryPropertyFlagBits::eHostCoherent));
    if (context->device->allocateMemory(
            &allocate_info, nullptr, &buffer_memory_) != vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to allocate vertex buffer.");
        return;
    }
    context->device->bindBufferMemory(buffer_, buffer_memory_, 0);

    void* mapped_data = context->device->mapMemory(buffer_memory_, 0, n);
    memcpy(mapped_data, data, n);
    context->device->unmapMemory(buffer_memory_);
}
} // namespace artemis
