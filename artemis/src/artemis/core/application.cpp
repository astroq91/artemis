#include "application.hpp"
#include "artemis/core/log.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>

namespace artemis {

void Application::run() {
    Log::init();
    event_bus_ = std::make_shared<EventBus>();
    window_ = std::make_unique<Window>();
    vulkan_context_ = std::make_shared<VulkanContext>();
    vulkan_context_->init();

    listener_init();

    WindowEvent test;
    test.some_data = "testing";
    event_bus_->publish(test);
    event_bus_->flush();

    while (running_) {
        if (window_->should_close()) {
            running_ = false;
        }
        listener_->on_update(1.0f);
        window_->on_update();
    }
}

void Application::listener_init() {
    if (listener_ == nullptr) {
        Log::get()->critical("Application listener not specified.");
    }
    listener_->on_start();
    listener_->register_events(event_bus_);
}
} // namespace artemis
