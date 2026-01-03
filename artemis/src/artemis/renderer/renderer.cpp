#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <vulkan/vulkan_structs.hpp>
namespace artemis {

std::unique_ptr<Pipeline> mesh_pipeline;
std::unique_ptr<Pipeline> ui_pipeline;

static void framebuffer_resized_callback(GLFWwindow* window, int width,
                                         int height) {
    *reinterpret_cast<bool*>(glfwGetWindowUserPointer(window)) = true;
}

Renderer::Renderer(VulkanContext* context, DeferredQueue* deferred_queue,
                   Window* window, ResourceLibrary* resource_library,
                   uint32_t max_frames_in_flight)
    : context_(context), deferred_queue_(deferred_queue), window_(window),
      resource_library_(resource_library), max_fif_(max_frames_in_flight) {
    initialize_resources();
    glfwSetWindowUserPointer(window->get_handle(), &frame_buffer_resized_);
    glfwSetFramebufferSizeCallback(window_->get_handle(),
                                   framebuffer_resized_callback);
}
void Renderer::begin_frame() {
    fences_[frame_idx_]->wait(UINT64_MAX);
    auto [result, image_idx] = swap_chain_->acquire_next_image(
        UINT64_MAX, present_semaphores_[frame_idx_].get(), nullptr);
    image_idx_ = image_idx;

    if (result == vk::Result::eErrorOutOfDateKHR) {
        recreate_swap_chain();
        return;
    }
    if (result != vk::Result::eSuccess &&
        result != vk::Result::eSuboptimalKHR) {
        Log::get()->error("Failed to acquire swap chain image.");
        return;
    }
    fences_[frame_idx_]->reset_fence();

    command_buffers_[frame_idx_]->begin();
    VulkanUtils::transition_image(
        &swap_chain_->get_image(image_idx_),
        &command_buffers_[frame_idx_]->get_vk_command_buffer(),
        {
            .oldLayout = vk::ImageLayout::eUndefined,
            .newLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .srcAccessMask = {},
            .dstAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
            .srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .dstStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        });

    vk::ClearValue clear_color = vk::ClearColorValue(0.0f, 0.0f, 1.0f, 1.0f);
    vk::RenderingAttachmentInfo attachment_info(
        swap_chain_->get_image_view(image_idx_),
        vk::ImageLayout::eColorAttachmentOptimal, {}, {}, {},
        vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        clear_color);
    vk::RenderingInfo rendering_info(
        {}, vk::Rect2D({0, 0}, swap_chain_->get_extent()), 1, {}, 1,
        &attachment_info, {}, {});
    command_buffers_[frame_idx_]->begin_rendering(rendering_info);
    command_buffers_[frame_idx_]->end_rendering();
}
void Renderer::end_frame() {
    VulkanUtils::transition_image(
        &swap_chain_->get_image(image_idx_),
        &command_buffers_[frame_idx_]->get_vk_command_buffer(),
        {
            .oldLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .newLayout = vk::ImageLayout::ePresentSrcKHR,
            .srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
            .dstAccessMask = {},
            .srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            .dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
        });

    command_buffers_[frame_idx_]->end();

    vk::PipelineStageFlags wait_dest_stage_mask(
        vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo submit_info(
        1, &present_semaphores_[frame_idx_]->get_vk_semaphore(),
        &wait_dest_stage_mask, 1,
        &command_buffers_[frame_idx_]->get_vk_command_buffer(), 1,
        &render_semaphores_[image_idx_]->get_vk_semaphore());
    auto result = context_->graphics_queue->submit(
        1, &submit_info, fences_[frame_idx_]->get_vk_fence());

    vk::PresentInfoKHR present_info(
        1, &render_semaphores_[image_idx_]->get_vk_semaphore(), 1,
        &swap_chain_->get_vk_swapchain(), &image_idx_);
    result = context_->present_queue->presentKHR(&present_info);

    if (result == vk::Result::eErrorOutOfDateKHR ||
        result == vk::Result::eSuboptimalKHR || frame_buffer_resized_) {
        frame_buffer_resized_ = false;
        recreate_swap_chain();
    } else if (result != vk::Result::eSuccess) {
        Log::get()->error("Failed to present swap chain image.");
    }

    frame_idx_ = (frame_idx_ + 1) % max_fif_;
}

void Renderer::draw_cube(const Transform& transform) {}

void Renderer::create_default_meshes() {}

void Renderer::initialize_resources() {
    swap_chain_ = std::make_unique<SwapChain>(context_, window_);

    command_buffers_.resize(max_fif_);
    render_semaphores_.resize(swap_chain_->get_image_count());
    present_semaphores_.resize(max_fif_);
    fences_.resize(max_fif_);

    for (size_t i = 0; i < max_fif_; i++) {
        command_buffers_[i] = std::make_unique<CommandBuffer>(context_);
        fences_[i] = std::make_unique<Fence>(
            context_, deferred_queue_, vk::FenceCreateFlagBits::eSignaled);
        present_semaphores_[i] =
            std::make_unique<Semaphore>(context_, deferred_queue_);
    }
    for (size_t i = 0; i < swap_chain_->get_image_count(); i++) {
        render_semaphores_[i] =
            std::make_unique<Semaphore>(context_, deferred_queue_);
    }

    Log::get()->debug("Renderer initialized.");
}

void Renderer::recreate_swap_chain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window_->get_handle(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window_->get_handle(), &width, &height);
        glfwWaitEvents();
    }
    context_->device->waitIdle();
    swap_chain_.reset();
    swap_chain_ = std::make_unique<SwapChain>(context_, window_);

    // Recreate render semaphores
    render_semaphores_.clear();
    render_semaphores_.resize(swap_chain_->get_image_count());
    for (size_t i = 0; i < swap_chain_->get_image_count(); i++) {
        render_semaphores_[i] =
            std::make_unique<Semaphore>(context_, deferred_queue_);
    }
}
} // namespace artemis
