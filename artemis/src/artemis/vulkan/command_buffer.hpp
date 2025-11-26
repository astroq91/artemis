#pragma once

#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {

class CommandBuffer {
  public:
    CommandBuffer() = default;
    CommandBuffer(const VulkanContext& context);

    void begin();
    void end();

    vk::CommandBuffer& get_vk_command_buffer() { return command_buffer_; };

  private:
    vk::CommandBuffer command_buffer_{nullptr};
};

} // namespace artemis
