#include "application.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>

namespace artemis {

void Application::run() {
    Log::init();
    context_.event_bus = std::make_unique<EventBus>();
    context_.window = std::make_unique<Window>();
    context_.vulkan.init(context_.window);

    listener_init();
    while (running_) {
        if (context_.window->should_close()) {
            running_ = false;
        }
        listener_->on_update(1.0f);
        context_.window->on_update();
    }
}

void Application::listener_init() {
    if (listener_ == nullptr) {
        Log::get()->critical("Application listener not specified.");
    }
    listener_->on_start(&context_);
}
} // namespace artemis
