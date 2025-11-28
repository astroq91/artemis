#include "sandbox.hpp"
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
    logger_ = utils::Logger::create("SANDBOX");

    try {
        swap_chain_ = std::make_unique<SwapChain>(
            context->vulkan, context->window->get_handle());
        shader_ = std::make_unique<Shader>(
            RESOURCES_DIR "/shaders/bin/triangle.spv",
            ShaderType::Vertex | ShaderType::Fragment, context->vulkan,
            context->deferred_queue.get());
        pipeline_ = std::make_unique<Pipeline>(
            PipelineInfo{
                .vertex_shader = shader_.get(),
                .fragment_shader = shader_.get(),
                .swap_chain_image_format = swap_chain_->get_image_format(),
            },
            context->vulkan, context->deferred_queue.get());
        command_buffer_ = std::make_unique<CommandBuffer>(context->vulkan);
        draw_fence_ = std::make_unique<Fence>(&context->vulkan,
                                              context->deferred_queue.get());
        render_semaphore_ = std::make_unique<Semaphore>(
            &context->vulkan, context->deferred_queue.get());
        present_semaphore_ = std::make_unique<Semaphore>(
            &context->vulkan, context->deferred_queue.get());
    } catch (const std::exception& e) {
        logger_->error(e.what());
    }

    logger_->info(
        "Successfully initialized: Swap chain, shaders, and pipeline!");
}

void Sandbox::on_update(float ts) {
    auto result = swap_chain_->acquire_next_image(
        UINT64_MAX, present_semaphore_.get(), nullptr);
    if (result.result != vk::Result::eSuccess) {
        logger_->warn("Could not acquire next image.");
        return;
    }
    image_index_ = result.value;
    draw_fence_->reset_fence();
    command_buffer_->begin();
    VulkanUtils::transition_image(
        &swap_chain_->get_image(image_index_),
        &command_buffer_->get_vk_command_buffer(),
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
        swap_chain_->get_image_view(image_index_),
        vk::ImageLayout::eColorAttachmentOptimal, {}, {}, {},
        vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        clear_color);
    vk::RenderingInfo rendering_info(
        {}, vk::Rect2D({0, 0}, swap_chain_->get_extent()), 1, {}, 1,
        &attachment_info, {}, {});

    auto& cb = command_buffer_->get_vk_command_buffer();
    cb.beginRendering(&rendering_info);
    {
        cb.bindPipeline(vk::PipelineBindPoint::eGraphics,
                        pipeline_->get_vk_pipeline());
        cb.setViewport(
            0,
            vk::Viewport(0.0f, 0.0f,
                         static_cast<float>(swap_chain_->get_extent().width),
                         static_cast<float>(swap_chain_->get_extent().height)));
        cb.setScissor(
            0, vk::Rect2D(vk::Offset2D(0, 0), swap_chain_->get_extent()));
        cb.draw(3, 1, 0, 0);
    }
    cb.endRendering();

    VulkanUtils::transition_image(
        &swap_chain_->get_image(image_index_),
        &command_buffer_->get_vk_command_buffer(),
        {
            .oldLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .newLayout = vk::ImageLayout::ePresentSrcKHR,
            .srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
            .dstAccessMask = {},
            .srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
        });
    command_buffer_->end();

    vk::PipelineStageFlags wait_dest_stage_mask(
        vk::PipelineStageFlagBits::eColorAttachmentOutput);

    vk::SubmitInfo submit_info(1, &present_semaphore_->get_vk_semaphore(),
                               &wait_dest_stage_mask, 1,
                               &command_buffer_->get_vk_command_buffer(), 1,
                               &render_semaphore_->get_vk_semaphore());
    auto res = app_context_->vulkan.graphics_queue->submit(
        1, &submit_info, draw_fence_->get_vk_fence());

    vk::PresentInfoKHR present_info(1, &render_semaphore_->get_vk_semaphore(),
                                    1, &swap_chain_->get_vk_swapchain(),
                                    &image_index_);
    res = app_context_->vulkan.present_queue->presentKHR(&present_info);

    while (draw_fence_->wait(UINT64_MAX) == vk::Result::eTimeout)
        ;
}

void Sandbox::on_window_event(const WindowEvent& event) {
    logger_->info("Window event: {}", event.some_data);
}
