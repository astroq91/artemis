#include "sandbox.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/swap_chain.hpp"
#include <memory>
#include <print>

using namespace artemis;

void Sandbox::on_start(ApplicationContext* context) {
    app_context_ = context;
    logger_ = utils::Logger::create("SANDBOX");

    try {
        SwapChain swap_chain(context->vulkan, context->window->get_handle());
        Shader shader(context->vulkan,
                      RESOURCES_DIR "/shaders/bin/triangle.spv",
                      ShaderType::Vertex | ShaderType::Fragment);
        Pipeline pipeline(
            context->vulkan,
            PipelineInfo{
                .vertex_shader = &shader,
                .fragment_shader = &shader,
                .swap_chain_image_format = swap_chain.get_image_format(),
            });
    } catch (const std::exception& e) {
        logger_->error(e.what());
    }

    logger_->info(
        "Successfully initialized: Swap chain, shaders, and pipeline!");
}

void Sandbox::on_update(float ts) {}

void Sandbox::on_window_event(const WindowEvent& event) {
    logger_->info("Window event: {}", event.some_data);
}
