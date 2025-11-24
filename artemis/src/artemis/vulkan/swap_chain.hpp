#pragma once
#include "GLFW/glfw3.h"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace artemis {
class SwapChain {
  public:
    SwapChain() = default;
    SwapChain(const VulkanContext& context, GLFWwindow* window);

  private:
    vk::raii::SwapchainKHR swap_chain_{nullptr};
    vk::SurfaceFormatKHR surface_format_;
    vk::Format image_format_;
    vk::Extent2D extent_;
    std::vector<vk::Image> images_;
    std::vector<vk::raii::ImageView> image_views_;
};
} // namespace artemis
