#include "application.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>

namespace artemis {

void Application::run() {
    Log::init();
    context_.max_frames_in_flight = 3;
    context_.deferred_queue =
        std::make_unique<DeferredQueue>(context_.max_frames_in_flight);
    context_.event_bus = std::make_unique<EventBus>();
    context_.window = std::make_unique<Window>();
    context_.vulkan.init(context_.window);

    listener_init();
    while (running_) {
        // Important to flush the queue before letting anything first enqueue
        // anything
        context_.deferred_queue->flush();
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
