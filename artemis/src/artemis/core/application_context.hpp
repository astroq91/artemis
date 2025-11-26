#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/core/window.hpp"
#include "artemis/events/event_bus.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>
namespace artemis {
struct ApplicationContext {
    std::unique_ptr<Window> window;
    std::unique_ptr<EventBus> event_bus;
    std::unique_ptr<DeferredQueue> deferred_queue;
    VulkanContext vulkan;
    uint32_t max_frames_in_flight;
};
} // namespace artemis
