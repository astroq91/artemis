#pragma once

#include "artemis/assets/deferred_queue.hpp"
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
             Window* window, uint32_t max_frames_in_flight);
    void begin_frame();
    void end_frame();

  private:
    void init();

  private:
    struct SyncObject {
        SyncObject(VulkanContext* context, DeferredQueue* deferred_queue,
                   const vk::FenceCreateFlags& flags = {})
            : semaphore(context, deferred_queue),
              fence(context, deferred_queue, flags) {}
        Semaphore semaphore;
        Fence fence;
    };

    VulkanContext* context_;
    DeferredQueue* deferred_queue_;
    Window* window_;
    uint32_t frame_idx_ = 0;
    /* Frames in flight*/
    uint32_t max_fif_;
    std::vector<std::unique_ptr<CommandBuffer>> command_buffers_;
    std::vector<std::unique_ptr<SyncObject>> sync_objects_;
    std::unique_ptr<SwapChain> swap_chain_;
};
} // namespace artemis
