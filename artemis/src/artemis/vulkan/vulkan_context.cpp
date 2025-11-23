#include "vulkan_context.hpp"
#include "artemis/core/log.hpp"
#include "vulkan_utils.hpp"
#include <memory>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>

namespace vk {
namespace detail {
DispatchLoaderDynamic defaultDispatchLoaderDynamic;
}
} // namespace vk

namespace artemis {
static bool s_initialized = false;

static VKAPI_ATTR vk::Bool32 VKAPI_CALL
debug_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               vk::DebugUtilsMessageTypeFlagsEXT messageType,
               const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
               void* pUserData) {

    if (messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo) {
        Log::get()->info("(vk): {}", pCallbackData->pMessage);
    } else if (messageSeverity &
               vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {
        Log::get()->warn("(vk): {}", pCallbackData->pMessage);
    } else if (messageSeverity &
               vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) {
        Log::get()->error("(vk): {}", pCallbackData->pMessage);
    }

    return VK_FALSE;
}

void VulkanContext::init(const std::unique_ptr<Window>& window) {
    auto log = Log::get();
    log->debug("(vk) Initializing vulkan context.");
    if (s_initialized) {
        log->critical("(vk) Vulkan has already been initialized.");
    }
    // Load a minimal set of functions
    vk::detail::defaultDispatchLoaderDynamic.init();

    context = std::make_unique<vk::raii::Context>();
    log->debug("(vk) Creating instance.");
    try {
        instance = VulkanUtils::create_instance(context);
    } catch (const vk::SystemError& err) {
        log->critical("(vk) Failed to create vulkan instance: {}", err.what());
    }

    log->debug("(vk) Creating debug messenger.");
    try {
        debug_messenger =
            VulkanUtils::create_debug_messenger(instance, debug_callback);
    } catch (const vk::SystemError& err) {
        log->error("(vk) Failed to create debug messenger.");
    }

    log->debug("(vk) Creating surface.");
    try {
        surface = VulkanUtils::create_surface(instance, window->get_handle());
    } catch (const vk::SystemError& err) {
        log->critical("(vk) Failed to create surface");
    }

    log->debug("(vk) Creating device and queues.");
    try {
        auto&& [dev, graphics, present] =
            VulkanUtils::create_device_and_queues(instance, surface);
        device = std::move(dev);
        graphics_queue = std::move(graphics);
        present_queue = std::move(present);
    } catch (const vk::SystemError& err) {
        log->critical("(vk) Failed to create device.");
    }
}
} // namespace artemis
