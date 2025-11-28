#pragma once
#include "GLFW/glfw3.h"
#include "artemis/vulkan/fence.hpp"
#include "artemis/vulkan/semaphore.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {
class SwapChain {
  public:
    SwapChain() = default;
    ~SwapChain();

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;
    /**
     * Creates a swap chain.
     * @param context The vulkan context.
     * @param window The window handle.
     */
    SwapChain(const VulkanContext& context, GLFWwindow* window);

    vk::Image& get_image(uint32_t index) { return images_[index]; }
    vk::ImageView& get_image_view(uint32_t index) {
        return image_views_[index];
    }

    const vk::Extent2D& get_extent() const { return extent_; }

    /**
     * Get the image format of the swap chain images.
     * @return The format
     */
    vk::Format get_image_format() const { return image_format_; }

    vk::ResultValue<uint32_t>
    acquire_next_image(uint64_t timeout, Semaphore* semaphore, Fence* fence);

    const vk::SwapchainKHR& get_vk_swapchain() { return swap_chain_; }

  private:
    vk::SwapchainKHR swap_chain_{nullptr};
    vk::SurfaceFormatKHR surface_format_;
    vk::Format image_format_;
    vk::Extent2D extent_;
    std::vector<vk::Image> images_;
    std::vector<vk::ImageView> image_views_;

    vk::Device* device_;
};
} // namespace artemis
