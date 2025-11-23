#pragma once
#include "GLFW/glfw3.h"
#include <memory>
#include <optional>
#include <vulkan/vulkan_raii.hpp>

namespace artemis {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;

    bool is_complete() { return graphics.has_value() && present.has_value(); }
};

class VulkanUtils {
  public:
    static std::unique_ptr<vk::raii::Instance>
    create_instance(const std::unique_ptr<vk::raii::Context>& context);

    static std::tuple<std::unique_ptr<vk::raii::Device>,
                      std::unique_ptr<vk::raii::Queue>,
                      std::unique_ptr<vk::raii::Queue>>
    create_device_and_queues(
        const std::unique_ptr<vk::raii::Instance>& instance,
        const std::unique_ptr<vk::raii::SurfaceKHR>& surface);

    static std::unique_ptr<vk::raii::SurfaceKHR>
    create_surface(const std::unique_ptr<vk::raii::Instance>& instance,
                   GLFWwindow* window);

    static vk::raii::PhysicalDevice choose_physical_device(
        const std::unique_ptr<vk::raii::Instance>& instance,
        const std::unique_ptr<vk::raii::SurfaceKHR>& surface);

    static bool
    is_device_suitable(const vk::raii::PhysicalDevice& device,
                       const std::unique_ptr<vk::raii::SurfaceKHR>& surface);
    static QueueFamilyIndices
    find_queue_families(const vk::raii::PhysicalDevice& device,
                        const std::unique_ptr<vk::raii::SurfaceKHR>& surface);

    static bool check_validation_layer_support(
        const std::unique_ptr<vk::raii::Context>& context);

    static std::vector<const char*> get_required_extensions();

    static std::unique_ptr<vk::raii::DebugUtilsMessengerEXT>
    create_debug_messenger(const std::unique_ptr<vk::raii::Instance>& instance,
                           vk::PFN_DebugUtilsMessengerCallbackEXT callback);

    static vk::raii::Queue create_queue();
};
}; // namespace artemis
