#include "vulkan_utils.hpp"
#include "GLFW/glfw3.h"
#include "artemis/core/log.hpp"
#include <fcntl.h>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef DEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

namespace artemis {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageType,
               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
               void* pUserData) {

    if (messageType & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        Log::get()->info("(vk): {}", pCallbackData->pMessage);
    } else if (messageType & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        Log::get()->warn("(vk): {}", pCallbackData->pMessage);
    } else if (messageType & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        Log::get()->error("(vk): {}", pCallbackData->pMessage);
    }

    return VK_FALSE;
}
std::unique_ptr<vk::raii::Instance> VulkanUtils::create_instance(
    const std::unique_ptr<vk::raii::Context>& context) {
    if (enable_validation_layers && !check_validation_layer_support(context)) {
        Log::get()->critical(
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
    const std::unique_ptr<vk::raii::Instance>& instance) {
    auto physical_device = choose_physical_device(instance);
    auto indices = find_queue_families(physical_device);

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

vk::raii::PhysicalDevice VulkanUtils::choose_physical_device(
    const std::unique_ptr<vk::raii::Instance>& instance) {
    vk::raii::PhysicalDevices devices(*instance);
    int best_device = -1;
    for (int i = 0; i < devices.size(); ++i) {
        if (is_device_suitable(devices[i])) {
            best_device = i;
        }
    }
    if (best_device == -1) {
        Log::get()->critical("(vk) Failed to find a suitable physical device");
    }
    return vk::raii::PhysicalDevice(std::move(devices[best_device]));
}

bool VulkanUtils::is_device_suitable(const vk::raii::PhysicalDevice& device) {
    auto indices = find_queue_families(device);
    return indices.is_complete();
}
QueueFamilyIndices
VulkanUtils::find_queue_families(const vk::raii::PhysicalDevice& device) {
    QueueFamilyIndices indices;
    auto queue_families = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& family : queue_families) {
        if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphics = i;
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
            Log::get()->error("(vk) Validation layer support not found.");
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
    const std::unique_ptr<vk::raii::Instance>& instance) {
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
        debug_callback);

    return std::make_unique<vk::raii::DebugUtilsMessengerEXT>(
        instance->createDebugUtilsMessengerEXT(create_info));
}
} // namespace artemis
