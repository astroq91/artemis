#include "vulkan_utils.hpp"
#include <fcntl.h>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <GLFW/glfw3.h>

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef DEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

namespace artemis {

std::unique_ptr<vk::raii::Instance> VulkanUtils::create_instance(
    const std::unique_ptr<vk::raii::Context>& context) {
    if (enable_validation_layers && !check_validation_layer_support(context)) {
        throw std::runtime_error(
            "(vk) Validation layers enabled but not available.");
    }
    vk::ApplicationInfo app_info("Artemis", VK_MAKE_VERSION(1, 0, 0), "Artemis",
                                 VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_3);

    auto extensions = get_required_extensions();

    vk::InstanceCreateInfo create_info({}, &app_info, 0, {},
                                       static_cast<uint32_t>(extensions.size()),
                                       extensions.data());

    if (enable_validation_layers) {
        // Override if layers enabled
        create_info.enabledLayerCount =
            static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    }

    return std::make_unique<vk::raii::Instance>(*context, create_info);
}

std::unique_ptr<vk::raii::Device> VulkanUtils::create_device(
    const std::unique_ptr<vk::raii::Instance>& instance,
    const std::unique_ptr<vk::raii::SurfaceKHR>& surface) {
    auto physical_device = choose_physical_device(instance, surface);
    auto indices = find_queue_families(physical_device, surface);

    float queue_priority = 1.0f;
    vk::DeviceQueueCreateInfo queue_create_info({}, indices.graphics.value(), 1,
                                                &queue_priority);
    vk::PhysicalDeviceFeatures device_features;
    vk::DeviceCreateInfo create_info({}, 1, &queue_create_info, 0, {}, 0, {},
                                     &device_features);
    if (enable_validation_layers) {
        create_info.enabledLayerCount =
            static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    }

    return std::make_unique<vk::raii::Device>(physical_device, create_info);
}

std::unique_ptr<vk::raii::SurfaceKHR>
VulkanUtils::create_surface(const std::unique_ptr<vk::raii::Instance>& instance,
                            GLFWwindow* window) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(**instance, window, nullptr, &surface) !=
        VK_SUCCESS) {
        throw std::runtime_error("(vk) Failed to create window surface.");
    }
    return std::make_unique<vk::raii::SurfaceKHR>(*instance, surface);
}

vk::raii::PhysicalDevice VulkanUtils::choose_physical_device(
    const std::unique_ptr<vk::raii::Instance>& instance,
    const std::unique_ptr<vk::raii::SurfaceKHR>& surface) {
    vk::raii::PhysicalDevices devices(*instance);
    int best_device = -1;
    for (int i = 0; i < devices.size(); ++i) {
        if (is_device_suitable(devices[i], surface)) {
            best_device = i;
        }
    }
    if (best_device == -1) {
        throw std::runtime_error(
            "(vk) Failed to find a suitable physical device");
    }
    return vk::raii::PhysicalDevice(std::move(devices[best_device]));
}

bool VulkanUtils::is_device_suitable(
    const vk::raii::PhysicalDevice& device,
    const std::unique_ptr<vk::raii::SurfaceKHR>& surface) {
    auto indices = find_queue_families(device, surface);
    return indices.is_complete();
}
QueueFamilyIndices VulkanUtils::find_queue_families(
    const vk::raii::PhysicalDevice& device,
    const std::unique_ptr<vk::raii::SurfaceKHR>& surface) {
    QueueFamilyIndices indices;
    auto queue_families = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& family : queue_families) {
        if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphics = i;
        }

        if (device.getSurfaceSupportKHR(i, *surface)) {
            indices.present = i;
        }

        if (indices.is_complete()) {
            break;
        }
        i++;
    }

    return indices;
}

bool VulkanUtils::check_validation_layer_support(
    const std::unique_ptr<vk::raii::Context>& context) {
    auto properties = context->enumerateInstanceLayerProperties();

    for (auto layer_name : validation_layers) {
        bool layer_found = false;
        for (auto& layer_props : properties) {
            if (strcmp(layer_name, layer_props.layerName.data()) == 0) {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) {
            throw std::runtime_error(
                "(vk) Validation layer support not found.");
            return false;
        }
    }

    return true;
}

std::vector<const char*> VulkanUtils::get_required_extensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions,
                                        glfwExtensions + glfwExtensionCount);

    if (enable_validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

std::unique_ptr<vk::raii::DebugUtilsMessengerEXT>
VulkanUtils::create_debug_messenger(
    const std::unique_ptr<vk::raii::Instance>& instance,
    vk::PFN_DebugUtilsMessengerCallbackEXT callback) {
    if (!enable_validation_layers) {
        return nullptr;
    }
    using SeverityFlags = vk::DebugUtilsMessageSeverityFlagBitsEXT;
    using TypeFlags = vk::DebugUtilsMessageTypeFlagBitsEXT;

    vk::DebugUtilsMessengerCreateInfoEXT create_info(
        {},
        SeverityFlags::eVerbose | SeverityFlags::eError |
            SeverityFlags::eWarning,
        TypeFlags::eGeneral | TypeFlags::ePerformance | TypeFlags::eValidation,
        callback);

    return std::make_unique<vk::raii::DebugUtilsMessengerEXT>(
        instance->createDebugUtilsMessengerEXT(create_info));
}
} // namespace artemis
