#pragma once
#include "artemis/core/window.hpp"
#include "artemis/vulkan/vulkan_utils.hpp"
#include <memory>
#include <vulkan/vulkan.hpp>

namespace artemis {
struct VulkanContext {
    ~VulkanContext();
    void init(Window* window);

    std::unique_ptr<vk::Instance> instance;
    std::unique_ptr<vk::Device> device;
    std::unique_ptr<vk::PhysicalDevice> physical_device;
    std::unique_ptr<vk::SurfaceKHR> surface;
    std::unique_ptr<vk::Queue> graphics_queue;
    std::unique_ptr<vk::Queue> present_queue;
    std::unique_ptr<vk::CommandPool> command_pool;
    std::unique_ptr<vk::DebugUtilsMessengerEXT> debug_messenger;
    QueueFamilyIndices queue_family_indices;
};
} // namespace artemis
