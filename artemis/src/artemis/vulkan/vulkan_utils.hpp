#pragma once
#include <memory>
#include <optional>
#include <vulkan/vulkan_raii.hpp>

namespace artemis {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;

    bool is_complete() { return graphics.has_value(); }
};

class VulkanUtils {
  public:
    static std::unique_ptr<vk::raii::Instance>
    create_instance(const std::unique_ptr<vk::raii::Context>& context);

    static std::unique_ptr<vk::raii::Device>
    create_device(const std::unique_ptr<vk::raii::Instance>& instance);

    static vk::raii::PhysicalDevice
    choose_physical_device(const std::unique_ptr<vk::raii::Instance>& instance);

    static bool is_device_suitable(const vk::raii::PhysicalDevice& device);
    static QueueFamilyIndices
    find_queue_families(const vk::raii::PhysicalDevice& device);

    static bool check_validation_layer_support(
        const std::unique_ptr<vk::raii::Context>& context);

    static std::vector<const char*> get_required_extensions();

    static std::unique_ptr<vk::raii::DebugUtilsMessengerEXT>
    create_debug_messenger(const std::unique_ptr<vk::raii::Instance>& instance);
};
}; // namespace artemis
