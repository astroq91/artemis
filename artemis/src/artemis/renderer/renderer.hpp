#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/core/transform.hpp"
#include "artemis/renderer/instancer.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/fence.hpp"
#include "artemis/vulkan/semaphore.hpp"
#include "artemis/vulkan/swap_chain.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>
namespace artemis {
class Renderer {
  public:
    Renderer() = default;
    Renderer(VulkanContext* context, DeferredQueue* deferred_queue,
             Window* window, ResourceLibrary* resource_library,
             uint32_t max_frames_in_flight);
    void begin_frame();
    void end_frame();

    void draw_cube(const Transform& transform);

  private:
    void create_default_meshes();
    void initialize_resources();
    void recreate_swap_chain();

  private:
    VulkanContext* context_;
    DeferredQueue* deferred_queue_;
    Window* window_;
    ResourceLibrary* resource_library_;

    Instancer instancer_;

    uint32_t frame_idx_ = 0;
    uint32_t image_idx_ = 0;
    bool frame_buffer_resized_ = false;
    /* Frames in flight*/
    uint32_t max_fif_;
    std::vector<std::unique_ptr<CommandBuffer>> command_buffers_;
    std::vector<std::unique_ptr<Semaphore>> render_semaphores_;
    std::vector<std::unique_ptr<Semaphore>> present_semaphores_;
    std::vector<std::unique_ptr<Fence>> fences_;
    std::unique_ptr<SwapChain> swap_chain_;
};
} // namespace artemis
