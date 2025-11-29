#include "artemis/core/log.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/fence.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include "artemis/vulkan/semaphore.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/swap_chain.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <artemis/core/application.hpp>
#include <memory>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

using namespace artemis;

template <typename T> using unique = std::unique_ptr<T>;

const uint32_t max_frames_in_flight = 2;

TEST(artemis, render_triangle) {
    // Context setup
    Log::init();
    Window window;
    VulkanContext context;
    DeferredQueue deferred_queue(max_frames_in_flight);
    context.init(&window);

    SwapChain swap_chain(context, &window);
    Shader shader(RESOURCES_DIR "/shaders/bin/triangle.spv",
                  ShaderType::Vertex | ShaderType::Fragment, context,
                  &deferred_queue);
    Pipeline pipeline(
        PipelineCreateInfo{
            .vertex_shader = &shader,
            .fragment_shader = &shader,
            .swap_chain_image_format = swap_chain.get_image_format(),
        },
        context, &deferred_queue);
    std::vector<unique<Semaphore>> render_semaphores;
    std::vector<unique<Semaphore>> present_semaphores;
    std::vector<unique<Fence>> fences;
    std::vector<unique<CommandBuffer>> cbs;
    for (uint32_t i = 0; i < max_frames_in_flight; i++) {

        cbs.emplace_back(std::make_unique<CommandBuffer>(context));
    }
    for (uint32_t i = 0; i < swap_chain.get_image_count(); i++) {
        render_semaphores.emplace_back(
            std::make_unique<Semaphore>(context, &deferred_queue));
        present_semaphores.emplace_back(
            std::make_unique<Semaphore>(context, &deferred_queue));
        fences.emplace_back(std::make_unique<Fence>(
            vk::FenceCreateFlagBits::eSignaled, context, &deferred_queue));
    }

    uint32_t frame_index = 0;
    uint32_t image_index = 0;
    while (!window.should_close()) {
        deferred_queue.flush();

        while (fences[image_index]->wait(UINT64_MAX) == vk::Result::eTimeout)
            ;
        fences[image_index]->reset_fence();

        auto result_value = swap_chain.acquire_next_image(
            UINT64_MAX, present_semaphores[image_index].get(), nullptr);
        ASSERT_EQ(result_value.result, vk::Result::eSuccess);

        image_index = result_value.value;
        cbs[frame_index]->begin();
        VulkanUtils::transition_image(
            &swap_chain.get_image(image_index),
            &cbs[frame_index]->get_vk_command_buffer(),
            {
                .oldLayout = vk::ImageLayout::eUndefined,
                .newLayout = vk::ImageLayout::eColorAttachmentOptimal,
                .srcAccessMask = {},
                .dstAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
                .srcStageMask =
                    vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .dstStageMask =
                    vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            });

        vk::ClearValue clear_color =
            vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
        vk::RenderingAttachmentInfo attachment_info(
            swap_chain.get_image_view(image_index),
            vk::ImageLayout::eColorAttachmentOptimal, {}, {}, {},
            vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
            clear_color);
        vk::RenderingInfo rendering_info(
            {}, vk::Rect2D({0, 0}, swap_chain.get_extent()), 1, {}, 1,
            &attachment_info, {}, {});

        auto& cb_handle = cbs[frame_index]->get_vk_command_buffer();
        cb_handle.beginRendering(&rendering_info);
        {
            cb_handle.bindPipeline(vk::PipelineBindPoint::eGraphics,
                                   pipeline.get_vk_pipeline());
            cb_handle.setViewport(
                0, vk::Viewport(
                       0.0f, 0.0f,
                       static_cast<float>(swap_chain.get_extent().width),
                       static_cast<float>(swap_chain.get_extent().height)));
            cb_handle.setScissor(
                0, vk::Rect2D(vk::Offset2D(0, 0), swap_chain.get_extent()));
            cb_handle.draw(3, 1, 0, 0);
        }
        cb_handle.endRendering();

        VulkanUtils::transition_image(
            &swap_chain.get_image(image_index), &cb_handle,
            {
                .oldLayout = vk::ImageLayout::eColorAttachmentOptimal,
                .newLayout = vk::ImageLayout::ePresentSrcKHR,
                .srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
                .dstAccessMask = {},
                .srcStageMask =
                    vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                .dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
            });
        cbs[frame_index]->end();

        vk::PipelineStageFlags wait_dest_stage_mask(
            vk::PipelineStageFlagBits::eColorAttachmentOutput);

        vk::SubmitInfo submit_info(
            1, &present_semaphores[image_index]->get_vk_semaphore(),
            &wait_dest_stage_mask, 1,
            &cbs[frame_index]->get_vk_command_buffer(), 1,
            &render_semaphores[image_index]->get_vk_semaphore());
        auto res = context.graphics_queue->submit(
            1, &submit_info, fences[image_index]->get_vk_fence());

        vk::PresentInfoKHR present_info(
            1, &render_semaphores[image_index]->get_vk_semaphore(), 1,
            &swap_chain.get_vk_swapchain(), &image_index);
        res = context.present_queue->presentKHR(&present_info);

        window.on_update();

        frame_index = (frame_index + 1) % max_frames_in_flight;
    }
    context.device->waitIdle();
}
