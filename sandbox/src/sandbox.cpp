#include "sandbox.hpp"
#include "artemis/core/transform.hpp"
#include "artemis/events/window_event.hpp"
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
}

void Sandbox::on_update(float ts) {
    Transform transform;
    renderer_->draw_cube(transform);
}

void Sandbox::on_window_event(const WindowEvent& event) {
    logger_->info("Window event: {}", event.some_data);
}
