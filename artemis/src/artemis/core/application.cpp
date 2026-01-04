#include "application.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>

namespace artemis {

uint32_t k_max_frames_in_flight = 3;

void Application::run() {
    Log::init();
    context_.max_frames_in_flight = k_max_frames_in_flight;
    context_.deferred_queue =
        std::make_unique<DeferredQueue>(k_max_frames_in_flight);
    context_.event_bus = std::make_unique<EventBus>();
    context_.window = std::make_unique<Window>();
    context_.vulkan.init(context_.window.get());
    context_.resource_library = std::make_unique<ResourceLibrary>();
    context_.renderer = std::make_unique<Renderer>(
        &context_.vulkan, context_.deferred_queue.get(), context_.window.get(),
        context_.resource_library.get(), k_max_frames_in_flight);

    listener_init();
    while (running_) {
        context_.renderer->begin_frame();
        // Important to flush the queue after the frame has begun.
        // This ensures the command buffer is not in use
        context_.deferred_queue->flush();
        listener_->on_update(1.0f);
        context_.window->on_update();
        context_.renderer->end_frame();
        context_.deferred_queue->update_frame_index();
        if (context_.window->should_close()) {
            running_ = false;
        }
    }

    // Wait for all outstanding queue operations to complete before deleting
    // resources
    context_.vulkan.device->waitIdle();
}

void Application::listener_init() {
    if (listener_ == nullptr) {
        Log::get()->critical("Application listener not specified.");
    }
    listener_->on_start(&context_);
}
} // namespace artemis
