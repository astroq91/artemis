#include "vulkan_utils.hpp"
#include <fcntl.h>
#include <limits>
#include <memory>
#include <optional>
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
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> device_extensions = {
    vk::KHRSwapchainExtensionName,
    vk::KHRSpirv14ExtensionName,
    vk::KHRSynchronization2ExtensionName,
    vk::KHRCreateRenderpass2ExtensionName,
    vk::KHRShaderDrawParametersExtensionName,
};

#ifdef DEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

namespace artemis {

std::unique_ptr<vk::Instance> VulkanUtils::create_instance() {
    if (enable_validation_layers && !check_validation_layer_support()) {
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

    return std::make_unique<vk::Instance>(
        vk::createInstance(create_info, nullptr));
}

std::tuple<std::unique_ptr<vk::Device>, std::unique_ptr<vk::Queue>,
           std::unique_ptr<vk::Queue>>
VulkanUtils::create_device_and_queues(
    const std::unique_ptr<vk::Instance>& instance,
    const std::unique_ptr<vk::SurfaceKHR>& surface) {
    auto physical_device = choose_physical_device(instance, surface);
    auto indices = find_queue_families(physical_device, surface);

    auto features = physical_device.getFeatures2();
    vk::PhysicalDeviceVulkan13Features vulkan_13_features;
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
        extended_dynamic_state_features;
    vulkan_13_features.dynamicRendering = vk::True;
    vulkan_13_features.pNext = &extended_dynamic_state_features;
    features.pNext = &vulkan_13_features;

    float queue_priority = 1.0f;
    vk::DeviceQueueCreateInfo queue_create_info({}, indices.graphics.value(), 1,
                                                &queue_priority);
    vk::DeviceCreateInfo create_info{};
    create_info.pNext = &features;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.enabledExtensionCount = device_extensions.size();
    create_info.ppEnabledExtensionNames = device_extensions.data();

    if (enable_validation_layers) {
        create_info.enabledLayerCount =
            static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    }

    auto device = physical_device.createDevice(create_info);
    auto graphics_queue = device.getQueue(indices.graphics.value(), 0);
    auto present_queue = device.getQueue(indices.present.value(), 0);
    return {std::make_unique<vk::Device>(std::move(device)),
            std::make_unique<vk::Queue>(std::move(graphics_queue)),
            std::make_unique<vk::Queue>(std::move(present_queue))};
}

std::unique_ptr<vk::SurfaceKHR>
VulkanUtils::create_surface(const std::unique_ptr<vk::Instance>& instance,
                            GLFWwindow* window) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(*instance, window, nullptr, &surface) !=
        VK_SUCCESS) {
        throw std::runtime_error("(vk) Failed to create window surface.");
    }
    return std::make_unique<vk::SurfaceKHR>(surface);
}

vk::PhysicalDevice VulkanUtils::choose_physical_device(
    const std::unique_ptr<vk::Instance>& instance,
    const std::unique_ptr<vk::SurfaceKHR>& surface) {
    auto devices = instance->enumeratePhysicalDevices();
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
    return vk::PhysicalDevice(std::move(devices[best_device]));
}

bool VulkanUtils::is_device_suitable(
    const vk::PhysicalDevice& device,
    const std::unique_ptr<vk::SurfaceKHR>& surface) {
    auto surface_capabilites = device.getSurfaceCapabilitiesKHR(*surface);
    auto available_formats = device.getSurfaceFormatsKHR(*surface);
    auto available_present_modes = device.getSurfacePresentModesKHR(*surface);

    auto indices = find_queue_families(device, surface);
    return indices.is_complete();
}
QueueFamilyIndices VulkanUtils::find_queue_families(
    const vk::PhysicalDevice& device,
    const std::unique_ptr<vk::SurfaceKHR>& surface) {
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

bool VulkanUtils::check_validation_layer_support() {
    auto properties = vk::enumerateInstanceLayerProperties();

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

std::unique_ptr<vk::DebugUtilsMessengerEXT> VulkanUtils::create_debug_messenger(
    const std::unique_ptr<vk::Instance>& instance,
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

    return std::make_unique<vk::DebugUtilsMessengerEXT>(
        instance->createDebugUtilsMessengerEXT(create_info));
}

vk::SurfaceFormatKHR VulkanUtils::choose_swap_surface_format(
    const std::vector<vk::SurfaceFormatKHR>& available_formats) {
    for (const auto& available_format : available_formats) {
        if (available_format.format == vk::Format::eB8G8R8A8Srgb &&
            available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return available_format;
        }
    }
    return available_formats[0];
}
vk::PresentModeKHR VulkanUtils::choose_present_mode(
    const std::vector<vk::PresentModeKHR>& available_present_modes) {
    for (const auto& available_present_mode : available_present_modes) {
        if (available_present_mode == vk::PresentModeKHR::eMailbox) {
            return available_present_mode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D
VulkanUtils::choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities,
                                GLFWwindow* window) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    return {
        std::clamp<uint32_t>(width, capabilities.minImageExtent.width,
                             capabilities.maxImageExtent.width),
        std::clamp<uint32_t>(height, capabilities.minImageExtent.height,
                             capabilities.maxImageExtent.height),
    };
}

void VulkanUtils::transition_image(vk::Image* image,
                                   vk::CommandBuffer* command_buffer,
                                   const TransitionImageInfo& info) {
    vk::ImageMemoryBarrier2 barrier(
        info.srcStageMask, info.srcAccessMask, info.dstStageMask,
        info.dstAccessMask, info.oldLayout, info.newLayout,
        VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, *image,
        vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    vk::DependencyInfo dependency_info({}, 0, {}, 0, {}, 1, &barrier);
    command_buffer->pipelineBarrier2(dependency_info);
}
} // namespace artemis
