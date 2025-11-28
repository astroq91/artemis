#pragma once

#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {

class CommandBuffer {
  public:
    CommandBuffer() = default;
    CommandBuffer(const VulkanContext& context);

    // Delete copy constructor and assignment
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator=(const CommandBuffer&) = delete;

    void begin(const vk::CommandBufferBeginInfo& info = {});
    void end();

    vk::CommandBuffer& get_vk_command_buffer() { return command_buffer_; };

  private:
    vk::CommandBuffer command_buffer_{nullptr};
};

} // namespace artemis
