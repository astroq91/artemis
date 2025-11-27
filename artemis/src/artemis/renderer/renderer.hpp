#pragma once

#include "artemis/vulkan/vulkan_context.hpp"
namespace artemis {
class Renderer {
    Renderer() = default;
    Renderer(VulkanContext* context);
    void begin_frame();
    void end_frame();

  private:
    VulkanContext* context_;
};
} // namespace artemis
