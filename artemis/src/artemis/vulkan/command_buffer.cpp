#include "command_buffer.hpp"

namespace artemis {
CommandBuffer::CommandBuffer(const VulkanContext& context) {
    vk::CommandBufferAllocateInfo alloc_info(
        *context.command_pool, vk::CommandBufferLevel::ePrimary, 1);
    command_buffer_ = std::move(
        vk::raii::CommandBuffers(*context.device, alloc_info).front());
}

void CommandBuffer::begin() { command_buffer_.begin({}); }
void CommandBuffer::end() { command_buffer_.end(); }
} // namespace artemis
