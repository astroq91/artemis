#include "application.hpp"
#include "artemis/core/log.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/swap_chain.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>

namespace artemis {

void Application::run() {
    Log::init();
    event_bus_ = std::make_shared<EventBus>();
    window_ = std::make_unique<Window>();
    vulkan_context_.init(window_);

    try {
        SwapChain test(vulkan_context_, window_->get_handle());
        Shader shader(vulkan_context_,
                      RESOURCES_DIR "/shaders/bin/triangle.spv",
                      ShaderType::Vertex | ShaderType::Fragment);
        Pipeline pipeline(vulkan_context_, PipelineInfo{
                                               .vertex_shader = &shader,
                                               .fragment_shader = &shader,
                                               .swap_chain_image_format =
                                                   test.get_image_format(),
                                           });
    } catch (const std::exception& e) {
        Log::get()->error(e.what());
    }

    listener_init();
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
