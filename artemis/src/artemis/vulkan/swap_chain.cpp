#include "swap_chain.hpp"
#include "GLFW/glfw3.h"
#include <vulkan/vulkan_raii.hpp>

namespace artemis {
SwapChain::SwapChain(const VulkanContext& context, GLFWwindow* window) {
    auto surface_capabilities =
        context.physical_device->getSurfaceCapabilitiesKHR(*context.surface);
    surface_format_ = VulkanUtils::choose_swap_surface_format(
        context.physical_device->getSurfaceFormatsKHR(*context.surface));
    extent_ = VulkanUtils::choose_swap_extent(surface_capabilities, window);
    uint32_t image_count = surface_capabilities.minImageCount + 1;
    if (surface_capabilities.maxImageCount > 0 &&
        image_count > surface_capabilities.maxImageCount) {
        image_count = surface_capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR create_info(
        {}, *context.surface, image_count, surface_format_.format,
        surface_format_.colorSpace, extent_, 1,
        vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive,
        {}, {}, surface_capabilities.currentTransform,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        VulkanUtils::choose_present_mode(
            context.physical_device->getSurfacePresentModesKHR(
                *context.surface)),
        vk::True);

    uint32_t queue_family_indices[] = {
        context.queue_family_indices.graphics.value(),
        context.queue_family_indices.present.value()};

    if (queue_family_indices[0] != queue_family_indices[1]) {
        create_info.imageSharingMode = vk::SharingMode::eConcurrent;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = vk::SharingMode::eExclusive;
    }

    swap_chain_ = vk::raii::SwapchainKHR(*context.device, create_info);
    images_ = swap_chain_.getImages();
    image_format_ = surface_format_.format;

    for (const auto& image : images_) {
        vk::ImageViewCreateInfo create_info(
            {}, image, vk::ImageViewType::e2D, image_format_, {},
            {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
        image_views_.emplace_back(*context.device, create_info);
    }
}
} // namespace artemis
