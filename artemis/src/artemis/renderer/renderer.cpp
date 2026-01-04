#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "artemis/core/log.hpp"
#include "artemis/renderer/instancer.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <stddef.h>
#include <memory>
#include <vulkan/vulkan_structs.hpp>
namespace artemis {

constexpr uint32_t k_max_forward_instances = 10000;

struct ForwardPipelineVertex {
    alignas(16) glm::vec3 position;
};

static void framebuffer_resized_callback(GLFWwindow* window, int width,
                                         int height) {
    *reinterpret_cast<bool*>(glfwGetWindowUserPointer(window)) = true;
}

Renderer::Renderer(VulkanContext* context, DeferredQueue* deferred_queue,
                   Window* window, ResourceLibrary* resource_library,
                   uint32_t max_frames_in_flight)
    : context_(context), deferred_queue_(deferred_queue), window_(window),
      resource_library_(resource_library), max_fif_(max_frames_in_flight) {
    create_resources();
    create_default_meshes();
    create_pipelines();

    current_cmd_buf_ = command_buffers_[frame_idx_].get();
    glfwSetWindowUserPointer(window->get_handle(), &frame_buffer_resized_);
    glfwSetFramebufferSizeCallback(window_->get_handle(),
                                   framebuffer_resized_callback);
    Log::get()->debug("Renderer initialized.");
}
void Renderer::begin_frame() {
    current_cmd_buf_ = command_buffers_[frame_idx_].get();
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

    instancer_.clear();

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

    // Clear the screen
    begin_rendering({});
    end_rendering();
}
void Renderer::end_frame() {
    draw_forward_instances();

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

void Renderer::begin_rendering(const BeginRenderingInfo& info) {
    vk::ImageView image_view = info.image_view;
    if (image_view == nullptr) {
        image_view = swap_chain_->get_image_view(image_idx_);
    }
    vk::RenderingAttachmentInfo attachment_info(
        image_view, info.image_layout, {}, {}, {}, info.load_op, info.store_op,
        info.clear_color);
    auto viewport_size = vk::Rect2D({0, 0}, swap_chain_->get_extent());
    vk::Viewport viewport(0, 0, swap_chain_->get_extent().width,
                          swap_chain_->get_extent().height, 0.0f, 1.0f);
    current_cmd_buf_->get_vk_command_buffer().setViewport(0, viewport);
    current_cmd_buf_->get_vk_command_buffer().setScissor(0, viewport_size);

    vk::RenderingInfo rendering_info({}, viewport_size, 1, {}, 1,
                                     &attachment_info, {}, {});
    current_cmd_buf_->begin_rendering(rendering_info);
}
void Renderer::end_rendering() { current_cmd_buf_->end_rendering(); }

void Renderer::draw_cube(Transform& transform) {
    instancer_.add_forward_instance(cube_mesh_handle_, transform.get_mat4());
}

void Renderer::create_default_meshes() {
    std::vector<ForwardPipelineVertex> cube_vertices = {
        // Front
        {{-0.5f, 0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}},
        {{0.5f, -0.5f, 0.5f}},
        {{-0.5f, -0.5f, 0.5f}},
        // Back
        {{-0.5f, 0.5f, -0.5f}},
        {{0.5f, 0.5f, -0.5f}},
        {{0.5f, -0.5f, -0.5f}},
        {{-0.5f, -0.5f, -0.5f}},
    };
    std::vector<uint32_t> cube_indices = {
        // Front
        0,
        1,
        2,
        2,
        3,
        0,
        // Back
        5,
        4,
        7,
        7,
        6,
        5,
        // Left
        4,
        0,
        3,
        3,
        7,
        4,
        // Right
        1,
        5,
        6,
        6,
        2,
        1,
        // Top
        3,
        2,
        6,
        6,
        7,
        3,
        // Bottom
        4,
        5,
        1,
        1,
        0,
        4,
    };

    auto cube = std::make_unique<Mesh>(
        context_, deferred_queue_, current_cmd_buf_, cube_vertices.data(),
        sizeof(ForwardPipelineVertex) * cube_vertices.size(),
        cube_indices.data(), cube_indices.size(), sizeof(uint32_t));
    cube_mesh_handle_ =
        resource_library_->get_mesh_pool().insert(std::move(cube));

    /* Default vertex buffer descs */
    forward_pipeline_mesh_desc_ = VertexBufferDescription(
        0, sizeof(ForwardPipelineVertex), vk::VertexInputRate::eVertex,
        {
            VertexAttributeDescription{
                .type = VertexAttributeType::Float3,
                .location = 0,
                .offset = offsetof(ForwardPipelineVertex, position),
            },
        });
}

void Renderer::create_resources() {
    swap_chain_ = std::make_unique<SwapChain>(context_, window_);

    command_buffers_.resize(max_fif_);
    render_semaphores_.resize(swap_chain_->get_image_count());
    present_semaphores_.resize(max_fif_);
    fences_.resize(max_fif_);
    forward_instance_buffers_.resize(max_fif_);

    for (size_t i = 0; i < max_fif_; i++) {
        command_buffers_[i] = std::make_unique<CommandBuffer>(context_);
        fences_[i] = std::make_unique<Fence>(
            context_, deferred_queue_, vk::FenceCreateFlagBits::eSignaled);
        present_semaphores_[i] =
            std::make_unique<Semaphore>(context_, deferred_queue_);

        forward_instance_buffers_[i] = std::make_unique<VertexBuffer>(
            context_, deferred_queue_,
            k_max_forward_instances * sizeof(MeshInstance));
    }
    for (size_t i = 0; i < swap_chain_->get_image_count(); i++) {
        render_semaphores_[i] =
            std::make_unique<Semaphore>(context_, deferred_queue_);
    }
}
void Renderer::create_pipelines() {
    Shader forward_shader(context_, deferred_queue_,
                          RESOURCES_DIR "/shaders/bin/forward.spv",
                          ShaderType::Vertex | ShaderType::Fragment);

    forward_pipeline_ = std::make_unique<Pipeline>(
        context_, deferred_queue_,
        PipelineCreateInfo{
            .vertex_shader = &forward_shader,
            .fragment_shader = &forward_shader,
            .swap_chain_image_format = swap_chain_->get_image_format(),
            .vertex_buffer_descs = {forward_pipeline_mesh_desc_, // 0
                                    k_mesh_instance_desc},       // 1
        });
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

void Renderer::draw_forward_instances() {
    instancer_.sort();
    auto forward_instances = instancer_.get_forward_instances();
    if (forward_instances.instances.empty()) {
        return;
    }

    forward_instance_buffers_[frame_idx_]->insert(
        current_cmd_buf_, forward_instances.instances.data(),
        sizeof(MeshInstance) * forward_instances.instances.size());
    auto cmd_buf = current_cmd_buf_->get_vk_command_buffer();

    begin_rendering({});
    // Bind the instance buffer
    vk::DeviceSize instance_offset = 0;
    cmd_buf.bindVertexBuffers(
        k_mesh_instance_binding,
        forward_instance_buffers_[frame_idx_]->get_buffer().get_vk_buffer(),
        instance_offset);

    // Bind the pipeline
    cmd_buf.bindPipeline(vk::PipelineBindPoint::eGraphics,
                         forward_pipeline_->get_vk_pipeline());

    auto draw_batch = [&](ResourceHandle<Mesh> mesh_handle,
                          size_t first_instance, size_t instance_count) {
        if (instance_count == 0)
            return;

        auto mesh = resource_library_->get_mesh_pool().get(mesh_handle);

        vk::DeviceSize vb_offset = sizeof(MeshInstance) * first_instance;

        cmd_buf.bindVertexBuffers(
            0, mesh->get_vertex_buffer().get_buffer().get_vk_buffer(),
            vb_offset);
        cmd_buf.bindIndexBuffer(
            mesh->get_index_buffer().get_buffer().get_vk_buffer(), 0,
            vk::IndexType::eUint32);

        cmd_buf.drawIndexed(mesh->get_index_buffer().get_index_count(),
                            static_cast<uint32_t>(instance_count), 0, 0, 0);
    };

    ResourceHandle<Mesh> current_mesh = forward_instances.order[0].mesh_handle;

    size_t batch_start = 0;

    for (size_t i = 1; i < forward_instances.instances.size(); ++i) {
        if (forward_instances.order[i].mesh_handle.index !=
            current_mesh.index) {
            draw_batch(current_mesh, batch_start, i - batch_start);
            current_mesh = forward_instances.order[i].mesh_handle;
            batch_start = i;
        }
    }

    // draw final batch
    draw_batch(current_mesh, batch_start,
               forward_instances.instances.size() - batch_start);
    end_rendering();
}

} // namespace artemis
