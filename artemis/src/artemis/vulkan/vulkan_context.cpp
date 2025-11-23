#include "vulkan_context.hpp"
#include "artemis/core/log.hpp"
#include "vulkan_utils.hpp"
#include <memory>
#include <vulkan/vulkan.hpp>

namespace vk {
namespace detail {
DispatchLoaderDynamic defaultDispatchLoaderDynamic;
}
} // namespace vk

namespace artemis {
static bool s_initialized = false;

void VulkanContext::init() {
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
        debug_messenger = VulkanUtils::create_debug_messenger(instance);
    } catch (const vk::SystemError& err) {
        log->error("(vk) Failed to create debug messenger.");
    }

    log->debug("(vk) Creating device.");
    try {
        device = VulkanUtils::create_device(instance);
    } catch (const vk::SystemError& err) {
        log->error("(vk) Failed to create device.");
    }
}
} // namespace artemis
