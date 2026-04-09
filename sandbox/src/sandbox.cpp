#include "sandbox.hpp"
#include "artemis/core/transform.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/renderer/camera.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/swap_chain.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <print>

using namespace artemis;

void Sandbox::on_start(ApplicationContext* context) {
    app_context_ = context;
    renderer_ = context->renderer.get();
    logger_ = utils::Logger::create("SANDBOX");
    Transform transform;
    transform.set_position({0, 0, 5});
  
    Camera camera(transform,
                  90, app_context_->window->get_aspect_ratio(), 0.1f, 100.0f);
    context->renderer->set_camera(camera);
}

void Sandbox::on_update(float ts) {
    Transform transform;
    transform.set_position({0.0f, 0.0f, 0.0f});
    renderer_->draw_cube(transform);
    transform.set_position({3.0f, 0.0f, 0.0f});
    renderer_->draw_cube(transform);
}

void Sandbox::on_window_event(const WindowEvent& event) {
    logger_->info("Window event: {}", event.some_data);
}
