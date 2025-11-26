#include "sandbox.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/vulkan/command_buffer.hpp"
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
        swap_chain_ = SwapChain(context->vulkan, context->window->get_handle());
        shader_ = Shader(RESOURCES_DIR "/shaders/bin/triangle.spv",
                         ShaderType::Vertex | ShaderType::Fragment,
                         context->vulkan, context->deferred_queue.get());
        pipeline_ = Pipeline(
            PipelineInfo{
                .vertex_shader = &shader_,
                .fragment_shader = &shader_,
                .swap_chain_image_format = swap_chain_.get_image_format(),
            },
            context->vulkan, context->deferred_queue.get());
        command_buffer_ = CommandBuffer(context->vulkan);
    } catch (const std::exception& e) {
        logger_->error(e.what());
    }

    logger_->info(
        "Successfully initialized: Swap chain, shaders, and pipeline!");
}

void Sandbox::on_update(float ts) {
    command_buffer_.begin();
    VulkanUtils::transition_image(
        &swap_chain_.get_image(image_index_),
        &command_buffer_.get_vk_command_buffer(),
        {
            .oldLayout = vk::ImageLayout::eUndefined,
            .newLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .srcAccessMask = {},
            .dstAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
            .srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .dstStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        });

    vk::ClearValue clear_color = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
    vk::RenderingAttachmentInfo attachment_info(
        swap_chain_.get_image_view(image_index_),
        vk::ImageLayout::eColorAttachmentOptimal, {}, {}, {},
        vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        clear_color);
    vk::RenderingInfo rendering_info(
        {}, vk::Rect2D({0, 0}, swap_chain_.get_extent()), 1, {}, 1,
        &attachment_info, {}, {});

    auto& cb = command_buffer_.get_vk_command_buffer();
    cb.beginRendering(&rendering_info);
    {
        cb.bindPipeline(vk::PipelineBindPoint::eGraphics,
                        pipeline_.get_vk_pipeline());
        cb.setViewport(
            0,
            vk::Viewport(0.0f, 0.0f,
                         static_cast<float>(swap_chain_.get_extent().width),
                         static_cast<float>(swap_chain_.get_extent().height)));
        cb.setScissor(0,
                      vk::Rect2D(vk::Offset2D(0, 0), swap_chain_.get_extent()));
        cb.draw(3, 1, 0, 0);
    }
    cb.endRendering();

    VulkanUtils::transition_image(
        &swap_chain_.get_image(image_index_),
        &command_buffer_.get_vk_command_buffer(),
        {
            .oldLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .newLayout = vk::ImageLayout::ePresentSrcKHR,
            .srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
            .dstAccessMask = {},
            .srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
        });
    command_buffer_.end();
}

void Sandbox::on_window_event(const WindowEvent& event) {
    logger_->info("Window event: {}", event.some_data);
}
