#pragma once

#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace artemis {

class CommandBuffer {
  public:
    CommandBuffer() = default;
    CommandBuffer(const VulkanContext& context);

    void begin();
    void end();

    const vk::raii::CommandBuffer& get_vk_command_buffer() const {
        return command_buffer_;
    };

  private:
    vk::raii::CommandBuffer command_buffer_{nullptr};
};

} // namespace artemis
