#pragma once
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace artemis {
struct VulkanContext {
    void init();

    std::unique_ptr<vk::raii::Context> context;
    std::unique_ptr<vk::raii::Instance> instance;
    std::unique_ptr<vk::raii::Device> device;
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debug_messenger;
};
} // namespace artemis
