#pragma once

#include "artemis/core/window.hpp"
#include "artemis/events/event_bus.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>
namespace artemis {
struct ApplicationContext {
    std::unique_ptr<Window> window;
    std::unique_ptr<EventBus> event_bus;
    VulkanContext vulkan;
};
} // namespace artemis
