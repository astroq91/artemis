#pragma once
#include "buffer.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
namespace artemis {
Buffer::Buffer(VulkanContext* context, size_t size, vk::BufferUsageFlags usage,
               vk::MemoryPropertyFlags properties)
    : context_(context), buffer_size_(size) {
    vk::BufferCreateInfo create_info({}, size, usage);
    if (context->device->createBuffer(&create_info, nullptr, &buffer_) !=
        vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to create buffer.");
    }

    auto mem_requirements =
        context->device->getBufferMemoryRequirements(buffer_);
    memory_size_ = mem_requirements.size;
    vk::MemoryAllocateInfo alloc_info(
        mem_requirements.size,
        VulkanUtils::find_memory_type(context->physical_device.get(),
                                      mem_requirements.memoryTypeBits,
                                      properties));
    if (context->device->allocateMemory(
            &alloc_info, nullptr, &buffer_memory_) != vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to allocate buffer.");
    }

    context->device->bindBufferMemory(buffer_, buffer_memory_, 0);
}

void Buffer::copy_into(Buffer& other, CommandBuffer* command_buffer) const {
    bool command_buf_alloc = false;
    if (command_buffer == nullptr) {
        command_buf_alloc = true;

        command_buffer = new CommandBuffer(context_);
        command_buffer->begin();
    }

    auto vk_cmd_buf = command_buffer->get_vk_command_buffer();
    vk_cmd_buf.copyBuffer(buffer_, other.buffer_,
                          vk::BufferCopy(0, 0, buffer_size_));

    if (command_buf_alloc) {
        context_->graphics_queue->submit(
            vk::SubmitInfo({}, {}, {}, 1, &vk_cmd_buf));
        context_->graphics_queue->waitIdle();
        command_buffer->end();
        delete command_buffer;
    }
}
} // namespace artemis
