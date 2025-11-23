#pragma once
#include "artemis/core/window.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace artemis {
struct VulkanContext {
    void init(const std::unique_ptr<Window>& window);

    std::unique_ptr<vk::raii::Context> context;
    std::unique_ptr<vk::raii::Instance> instance;
    std::unique_ptr<vk::raii::Device> device;
    std::unique_ptr<vk::raii::PhysicalDevice> physical_device;
    std::unique_ptr<vk::raii::SurfaceKHR> surface;
    std::unique_ptr<vk::raii::Queue> graphics_queue;
    std::unique_ptr<vk::raii::Queue> present_queue;
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debug_messenger;
    QueueFamilyIndices queue_family_indices;
};
} // namespace artemis
