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

struct TransitionImageInfo {
    vk::ImageLayout oldLayout;
    vk::ImageLayout newLayout;
    vk::AccessFlags2 srcAccessMask;
    vk::AccessFlags2 dstAccessMask;
    vk::PipelineStageFlags2 srcStageMask;
    vk::PipelineStageFlags2 dstStageMask;
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

    static vk::SurfaceFormatKHR choose_swap_surface_format(
        const std::vector<vk::SurfaceFormatKHR>& available_formats);
    static vk::PresentModeKHR choose_present_mode(
        const std::vector<vk::PresentModeKHR>& available_present_modes);

    static vk::Extent2D
    choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities,
                       GLFWwindow* window);

    static void transition_image(vk::raii::Image* image,
                                 vk::raii::CommandBuffer* command_buffer,
                                 const TransitionImageInfo& info);
};
}; // namespace artemis
