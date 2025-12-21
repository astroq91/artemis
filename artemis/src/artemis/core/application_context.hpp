#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/core/window.hpp"
#include "artemis/events/event_bus.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include "artemis/renderer/renderer.hpp"
#include <memory>
namespace artemis {
struct ApplicationContext {
    VulkanContext vulkan;
    std::unique_ptr<Window> window;
    std::unique_ptr<EventBus> event_bus;
    std::unique_ptr<DeferredQueue> deferred_queue;
    std::unique_ptr<Renderer> renderer;
    uint32_t max_frames_in_flight;
};
} // namespace artemis
